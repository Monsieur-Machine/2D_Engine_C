#include "image.h"

#include <png.h>
#include <stdlib.h>
#include "utils.h"
#include "errors.h"

int build_image(const char *input_file_name, FILE *output_file, uint32_t *asset_size)
{
    FILE *input_file = fopen(input_file_name, "rb");

    if(!input_file)
    {
        error_printf("Failed to open input file.");
        return EXIT_FAILURE;
    }

    // Check png autenticity.
    png_byte sig[8];
    if(file_read(sig, sizeof(png_byte), sizeof(sig), input_file)) return_failure_int;
    if(!png_check_sig(sig, 8))
    {
        error_printf("Png file is not valid.");
        if(fclose(input_file))
            error_printf("Failed to close input file : %d", errno);
        return EXIT_FAILURE;
    }

    // PNG structs.
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png_ptr)
    {
        error_printf("Failed to allocate png ptr.");
        if(fclose(input_file))
            error_printf("Failed to close input file : %d", errno);
        return EXIT_FAILURE;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr)
    {
        error_printf("Failed to allocate png info.");
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        if(fclose(input_file))
            error_printf("Failed to close input file : %d", errno);
        return EXIT_FAILURE;
    }

    // Fill structs and load data.
    png_init_io(png_ptr, input_file);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);

    uint16_t width = png_get_image_width(png_ptr, info_ptr);
    uint16_t height = png_get_image_height(png_ptr, info_ptr);
    size_t row_length = png_get_rowbytes(png_ptr, info_ptr);

    // Write image size.
    if(file_write(&width, sizeof(width), 1, output_file))
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        if(fclose(input_file))
            error_printf("Failed to close input file : %d", errno);
        return_failure_int;
    }

    *asset_size += sizeof(width);

    if(file_write(&height, sizeof(height), 1, output_file))
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        if(fclose(input_file))
            error_printf("Failed to close input file : %d", errno);
        return_failure_int;
    }

    *asset_size += sizeof(height);

    // Prepare storage.
    png_byte src_pixels[height * row_length];
    png_bytep src_row_ptrs[height];
    for(png_uint_32 i = 0; i < height; i++)
        src_row_ptrs[i] = &src_pixels[i * row_length];

    // Read pixels.
    png_read_image(png_ptr, src_row_ptrs);

    uint16_t dst_pixels[width * height];

    for(uint16_t i = 0; i < height; i++)
    {
        for(uint16_t j = 0; j < width; j += 1)
        {
            if(src_pixels[i * row_length + j * 4 + 3])
            {
                uint8_t r = src_pixels[i * row_length + j * 4]     >> 3;
                uint8_t g = src_pixels[i * row_length + j * 4 + 1] >> 2;
                uint8_t b = src_pixels[i * row_length + j * 4 + 2] >> 3;

                dst_pixels[i * width + j] = ((r) << 11) | ((g) << 5) | b;
            }
            else
            {
                dst_pixels[i * width + j] = 0x0001;
            }
        }
    }

    if(file_write(&dst_pixels, sizeof(*dst_pixels), width * height, output_file))
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        if(fclose(input_file))
            error_printf("Failed to close input file : %d", errno);
        return_failure_int;
    }

    *asset_size += sizeof(dst_pixels);

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    if(fclose(input_file))
    {
        error_printf("Failed to close input file : %d", errno);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
