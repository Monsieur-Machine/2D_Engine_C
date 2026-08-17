#include "image.h"

#include <png.h>
#include <stdlib.h>
#include "asset_file.h"
#include "errors.h"

int parse_png_for_header(const char *file_name, asset_header_def_t *asset_header_def)
{
    FILE *input_file = fopen(file_name, "rb");

    if(!input_file)
    {
        error_printf("Failed to open input file.");
        return EXIT_FAILURE;
    }

    // Check png autenticity.
    unsigned char sig[8];
    if(fread(sig, sizeof(unsigned char), 8, input_file) != 8)
    {
        fclose(input_file);
        error_printf("Failed to read input file.");
        return EXIT_FAILURE;
    }

    if(!png_check_sig(sig, 8))
    {
        fclose(input_file);
        error_printf("Png file is not valid.");
        return EXIT_FAILURE;
    }

    // PNG structs.
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png_ptr)
    {
        fclose(input_file);
        error_printf("Failed to allocate png ptr.");
        return EXIT_FAILURE;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr)
    {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(input_file);
        error_printf("Failed to allocate png info.");
        return EXIT_FAILURE;
    }

    // Fill structs and load data.
    png_init_io(png_ptr, input_file);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);

    uint32_t width = png_get_image_width(png_ptr, info_ptr);
    uint32_t height = png_get_image_height(png_ptr, info_ptr);
    uint8_t bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    uint8_t color_type = png_get_color_type(png_ptr, info_ptr);

    // Verify png attributes.
    if(bit_depth != 8)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(input_file);
        error_printf("Png bit depth not supported.");
        return EXIT_FAILURE;
    }

    switch(color_type)
    {
      case PNG_COLOR_TYPE_RGB:
      case PNG_COLOR_TYPE_RGB_ALPHA:
        break;

      default:
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(input_file);
        error_printf("Png color type not supported.");
        return EXIT_FAILURE;
    }

    // Fill asset def.
    asset_header_def->start = 0;
    asset_header_def->end = sizeof(width) + sizeof(height) + height * width * sizeof(uint16_t);

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(input_file);

    return EXIT_SUCCESS;
}

int parse_png_for_content_and_write(const char *input_file_name, FILE *output_file)
{
    FILE *input_file = fopen(input_file_name, "rb");

    if(!input_file)
    {
        error_printf("Failed to open input file.");
        return EXIT_FAILURE;
    }

    // Check png autenticity.
    unsigned char sig[8];
    if(fread(sig, sizeof(unsigned char), 8, input_file) != 8)
    {
        fclose(input_file);
        error_printf("Failed to read input file.");
        return EXIT_FAILURE;
    }

    if(!png_check_sig(sig, 8))
    {
        fclose(input_file);
        error_printf("Png file is not valid.");
        return EXIT_FAILURE;
    }

    // PNG structs.
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png_ptr)
    {
        fclose(input_file);
        error_printf("Failed to allocate png ptr.");
        return EXIT_FAILURE;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr)
    {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(input_file);
        error_printf("Failed to allocate png info.");
        return EXIT_FAILURE;
    }

    // Fill structs and load data.
    png_init_io(png_ptr, input_file);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);

    uint32_t width = png_get_image_width(png_ptr, info_ptr);
    uint32_t height = png_get_image_height(png_ptr, info_ptr);
    size_t row_length = png_get_rowbytes(png_ptr, info_ptr);

    // Write image size.
    if(fwrite(&width, sizeof(width), 1, output_file) != 1)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(input_file);
        error_printf("Failed to write to output file : %d", errno);
        return EXIT_FAILURE;
    }

    if(fwrite(&height, sizeof(height), 1, output_file) != 1)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(input_file);
        error_printf("Failed to write to output file : %d", errno);
        return EXIT_FAILURE;
    }

    // Prepare storage.
    png_byte src_pixels[height * row_length];
    png_bytep src_row_ptrs[height];
    for(png_uint_32 i = 0; i < height; i++)
        src_row_ptrs[i] = &src_pixels[i * row_length];

    // Read pixels.
    png_read_image(png_ptr, src_row_ptrs);

    uint16_t dst_pixels[width * height * sizeof(uint16_t)];

    for(uint32_t i = 0; i < height; i++)
    {
        for(uint32_t j = 0; j < width; j += 1)
        {
            uint8_t r, g, b;

            if(src_pixels[i * row_length + j * 4 + 3])
            {
                r = src_pixels[i * row_length + j * 4]     >> 3;
                g = src_pixels[i * row_length + j * 4 + 1] >> 2;
                b = src_pixels[i * row_length + j * 4 + 2] >> 3;
            }
            else
            {
                r = 0x00;
                g = 0x00;
                b = 0x01;
            }

            uint16_t pixel = ((r) << 11) | ((g) << 5) | b;

            dst_pixels[i * width + j] = pixel;

//            printf("\033[48;2;%d;%d;%dm ", r << 3, g << 2, b << 3);
        }
//        puts("\033[m");
    }

    if(fwrite(&dst_pixels, sizeof(uint16_t), width * height, output_file) != width * height)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(input_file);
        error_printf("Failed to write to output file : %d", errno);
        return EXIT_FAILURE;
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(input_file);

    return EXIT_SUCCESS;
}
