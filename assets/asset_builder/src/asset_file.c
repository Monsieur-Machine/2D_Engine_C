#include "asset_file.h"

#include <stdio.h>
#include <string.h>
#include <png.h>
#include <stdlib.h>

int parse_png_for_header(const char *file_name, asset_def_t *asset_def);

int parse_file_for_header(const char *file_name, asset_def_t *asset_def)
{
    const char *file_name_dot = strrchr(file_name, '.');

    if(!file_name_dot)
    {
        return -1;  // no extension
    }

    const char *file_name_ext = file_name_dot + 1;

    if(!strcmp(file_name_ext, "png"))
    {
        return parse_png_for_header(file_name, asset_def);
    }
    else
    {
        return -2;  // extension not supported
    }

    return 0;
}

int parse_png_for_header(const char *file_name, asset_def_t *asset_def)
{
    FILE *input_png_file = fopen(file_name, "rb");

    if(!input_png_file)
    {
        fclose(input_png_file);
        return -3;  // failed to open file
    }

    // Check png autenticity.
    unsigned char sig[8];
    fread(sig, 1, 8, input_png_file);
    if(!png_check_sig(sig, 8))
    {
        fclose(input_png_file);
        return -4;   // bad signature
    }

    // PNG structs.
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png_ptr)
    {
        fclose(input_png_file);
        return -5;   // out of memory
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr)
    {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(input_png_file);
        return -5;   // out of memory
    }

    // Fill structs and load data.
    png_init_io(png_ptr, input_png_file);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);

    size_t width = png_get_image_width(png_ptr, info_ptr);
    size_t height = png_get_image_height(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);

    // Print infos.
    printf("%s : %lux%lu -%d ", file_name, width, height, bit_depth);

    // Verify png attributes.
    if(bit_depth != 8)
        return -6;  // png bit depth not supported

    switch(color_type)
    {
      case PNG_COLOR_TYPE_RGB:
        puts("RGB"); break;

      case PNG_COLOR_TYPE_RGB_ALPHA:
        puts("RGBA"); break;

      case PNG_COLOR_TYPE_GRAY:
      case PNG_COLOR_TYPE_GRAY_ALPHA:
      case PNG_COLOR_TYPE_PALETTE:
        return -7;  // png color type not supported
    }

    // Fill asset def.
    const char *last_slash = strrchr(file_name, '/');
    const char *base_name = last_slash ? last_slash + 1 : file_name;

    asset_def->name = malloc(strlen(base_name) - 4);
    strncpy(asset_def->name, base_name, strlen(base_name) - 4);

    asset_def->start = 0;
    asset_def->end = height * width * 3;

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(input_png_file);

    return 0;
}

int parse_png_for_content(const char *file_name, void *buffer)
{
    FILE *input_png_file = fopen(file_name, "rb");

    if(!input_png_file)
    {
        fclose(input_png_file);
        return -3;  // failed to open file
    }

    // Check png autenticity.
    unsigned char sig[8];
    fread(sig, 1, 8, input_png_file);
    if(!png_check_sig(sig, 8))
    {
        fclose(input_png_file);
        return -4;   // bad signature
    }

    // PNG structs.
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png_ptr)
    {
        fclose(input_png_file);
        return -5;   // out of memory
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr)
    {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(input_png_file);
        return -5;   // out of memory
    }

    // Fill structs and load data.
    png_init_io(png_ptr, input_png_file);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);

    size_t width = png_get_image_width(png_ptr, info_ptr);
    size_t height = png_get_image_height(png_ptr, info_ptr);
    size_t row_length = png_get_rowbytes(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);

    // Show infos.
    printf("%s : %lux%lu -%d ", file_name, width, height, bit_depth);

    switch(color_type)
    {
      case PNG_COLOR_TYPE_GRAY :
        puts("grayscale"); break;

      case PNG_COLOR_TYPE_GRAY_ALPHA :
        puts("gray with alpha"); break;

      case PNG_COLOR_TYPE_PALETTE :
        puts("palette"); break;

      case PNG_COLOR_TYPE_RGB :
        puts("RGB"); break;

      case PNG_COLOR_TYPE_RGB_ALPHA :
        puts("RGBA");
    }

    // Prepare storage.
    png_byte pixels[height * row_length];
    png_bytep row_ptrs[height];
    for(png_uint_32 i = 0; i < height; i++)
        row_ptrs[i] = &pixels[i * row_length];

    // Read pixels.
    png_read_image(png_ptr, row_ptrs);

    for(size_t i = 0; i < height; i += 2)
    {
        for(size_t j = 0; j < row_length; j += 4)
        {
            if(pixels[i * row_length + j + 3])
                printf("\033[48;2;%d;%d;%dm", pixels[i * row_length + j], pixels[i * row_length + j + 1], pixels[i * row_length + j + 2]);
            else
                printf("\033[48;2;255;0;255m");

            if(pixels[(i + 1) * row_length + j + 3])
                printf("\033[38;2;%d;%d;%dm", pixels[(i + 1) * row_length + j], pixels[(i + 1) * row_length + j + 1], pixels[(i + 1) * row_length + j + 2]);
            else
                printf("\033[38;2;255;0;255m");

            printf("▄");
        }

        puts("\033[m");
    }

    fflush(stdout);

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(input_png_file);

    return 0;
}
