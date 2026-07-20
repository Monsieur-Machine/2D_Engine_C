#include "asset_file.h"

#include <stdio.h>
#include <string.h>
#include <png.h>
#include <stdlib.h>

int parse_png_for_header(const char *input_file_name, asset_header_def_t *asset_header_def);
int parse_png_for_content(const char *input_file_name, FILE *output_file);

int parse_file_for_header(const char *input_file_name, asset_header_def_t *asset_header_def)
{
    const char *file_name_dot = strrchr(input_file_name, '.');

    if(!file_name_dot)
    {
        return -1;  // no extension
    }

    const char *file_name_ext = file_name_dot + 1;

    if(!strcmp(file_name_ext, "png"))
    {
        return parse_png_for_header(input_file_name, asset_header_def);
    }
    else
    {
        return -2;  // extension not supported
    }

    return 0;
}

int write_header_to_file(asset_file_header_t *asset_file_header, FILE *output_file)
{
    size_t ret = fwrite(&asset_file_header->assets_number, sizeof(size_t), 1, output_file);
    if(ret != 1)
        return -8;  // failed writing output file

    for(size_t i = 0; i < asset_file_header->assets_number; i++)
    {
        size_t name_length = strlen(asset_file_header->assets_header_def[i].name) + 1;
        ret = fwrite(asset_file_header->assets_header_def[i].name, sizeof(unsigned char), name_length, output_file);
        if(ret != name_length)
            return -8;  // failed writing output file

        ret = fwrite(&asset_file_header->assets_header_def[i].start, sizeof(size_t), 1, output_file);
        if(ret != 1)
            return -8;  // failed writing output file
    }

    return 0;
}

int parse_file_for_content_and_write(const char *input_file_name, FILE *output_file)
{
    const char *file_name_dot = strrchr(input_file_name, '.');
    const char *file_name_ext = file_name_dot + 1;

    if(!strcmp(file_name_ext, "png"))
    {
        return parse_png_for_content(input_file_name, output_file);
    }

    return 0;
}

// File specific functions.
int parse_png_for_header(const char *file_name, asset_header_def_t *asset_header_def)
{
    FILE *input_file = fopen(file_name, "rb");

    if(!input_file)
    {
        return -3;  // failed to open file
    }

    // Check png autenticity.
    unsigned char sig[8];
    fread(sig, 1, 8, input_file);
    if(!png_check_sig(sig, 8))
    {
        fclose(input_file);
        return -4;   // bad signature
    }

    // PNG structs.
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png_ptr)
    {
        fclose(input_file);
        return -5;   // out of memory
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr)
    {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(input_file);
        return -5;   // out of memory
    }

    // Fill structs and load data.
    png_init_io(png_ptr, input_file);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);

    size_t width = png_get_image_width(png_ptr, info_ptr);
    size_t height = png_get_image_height(png_ptr, info_ptr);
    uint8_t bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    uint8_t color_type = png_get_color_type(png_ptr, info_ptr);

    // Verify png attributes.
    if(bit_depth != 8)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(input_file);
        return -6;  // png bit depth not supported
    }

    switch(color_type)
    {
      case PNG_COLOR_TYPE_RGB:
      case PNG_COLOR_TYPE_RGB_ALPHA:
        break;

      default:
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(input_file);
        return -7;  // png color type not supported
    }

    // Fill asset def.
    asset_header_def->start = 0;
    asset_header_def->end = height * width * 3;

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(input_file);

    return 0;
}

int parse_png_for_content(const char *input_file_name, FILE *output_file)
{
    FILE *input_file = fopen(input_file_name, "rb");

    if(!input_file)
    {
        fclose(input_file);
        return -3;  // failed to open file
    }

    // Check png autenticity.
    unsigned char sig[8];
    fread(sig, 1, 8, input_file);
    if(!png_check_sig(sig, 8))
    {
        fclose(input_file);
        return -4;   // bad signature
    }

    // PNG structs.
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png_ptr)
    {
        fclose(input_file);
        return -5;   // out of memory
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr)
    {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(input_file);
        return -5;   // out of memory
    }

    // Fill structs and load data.
    png_init_io(png_ptr, input_file);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);

    size_t width = png_get_image_width(png_ptr, info_ptr);
    size_t height = png_get_image_height(png_ptr, info_ptr);
    size_t row_length = png_get_rowbytes(png_ptr, info_ptr);

    // Write image size.
    size_t ret = fwrite(&width, sizeof(size_t), 1, output_file);
    if(!ret)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(input_file);
        return -8;   // failed writing to output file
    }

    ret = fwrite(&height, sizeof(size_t), 1, output_file);
    if(!ret)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(input_file);
        return -8;   // failed writing to output file
    }

    // Prepare storage.
    png_byte pixels[height * row_length];
    png_bytep row_ptrs[height];
    for(png_uint_32 i = 0; i < height; i++)
        row_ptrs[i] = &pixels[i * row_length];

    // Read pixels.
    png_read_image(png_ptr, row_ptrs);

    for(size_t i = 0; i < height; i++)
    {
        for(size_t j = 0; j < row_length; j += 4)
        {
            if(!pixels[i * row_length + j + 3])
            {
                pixels[i * row_length + j]     = 0xFF;
                pixels[i * row_length + j + 1] = 0x00;
                pixels[i * row_length + j + 2] = 0xFF;
            }

            ret = fwrite(&pixels[i * row_length + j], sizeof(png_byte), 3, output_file);
            if(ret != 3)
            {
                png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
                fclose(input_file);
                return -8;   // failed writing to output file
            }
        }
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(input_file);

    return 0;
}
