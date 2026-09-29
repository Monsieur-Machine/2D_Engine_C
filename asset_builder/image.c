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

    png_struct *img = NULL;
    png_info *img_info = NULL;

    // Check png autenticity.
    uint8_t sig[8];
    if(file_read(sig, sizeof(*sig), sizeof(sig), input_file)) goto_failure;
    if(!png_check_sig(sig, 8))
    {
        error_printf("Png file is not valid.");
        goto failure;
    }

    // PNG structs.
    img = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!img)
    {
        error_printf("Failed to allocate png ptr.");
        goto failure;
    }

    img_info = png_create_info_struct(img);
    if(!img_info)
    {
        error_printf("Failed to allocate png info.");
        goto failure;
    }

    // Fill structs and load data.
    png_init_io(img, input_file);
    png_set_sig_bytes(img, 8);
    png_read_info(img, img_info);

    {
        uint16_t width = png_get_image_width(img, img_info);
        uint16_t height = png_get_image_height(img, img_info);
        size_t row_length = png_get_rowbytes(img, img_info);

        // Write image size.
        if(file_write(&width, sizeof(width), 1, output_file)) goto_failure;
        *asset_size += sizeof(width);

        if(file_write(&height, sizeof(height), 1, output_file)) goto_failure;
        *asset_size += sizeof(height);

        // Prepare storage.
        uint8_t *src_pixels = calloc(height * row_length, sizeof(*src_pixels));
        if(!src_pixels)
        {
            error_printf("Failed to alloc memory : %d", errno);
            goto failure;
        }

        uint8_t **src_row_ptrs = calloc(height, sizeof(*src_row_ptrs));
        if(!src_row_ptrs)
        {
            error_printf("Failed to alloc memory : %d", errno);
            free(src_pixels);
            goto failure;
        }

        for(uint16_t i = 0; i < height; i++)
            src_row_ptrs[i] = &src_pixels[i * row_length];

        // Read pixels.
        png_read_image(img, src_row_ptrs);

        free(src_row_ptrs);
        png_destroy_read_struct(&img, &img_info, NULL);

        uint16_t *dst_pixels = calloc(width * height, sizeof(*dst_pixels));
        if(!dst_pixels)
        {
            error_printf("Failed to alloc memory : %d", errno);
            free(src_pixels);
            goto failure;
        }

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

        free(src_pixels);

        if(file_write(dst_pixels, sizeof(*dst_pixels), width * height, output_file))
        {
            free(dst_pixels);
            goto_failure;
        }

        *asset_size += width * height * sizeof(*dst_pixels);

        free(dst_pixels);
    }

    if(fclose(input_file))
    {
        error_printf("Failed to close input file : %d", errno);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;

failure:
    if(fclose(input_file))
        error_printf("Failed to close input file : %d", errno);

    return EXIT_FAILURE;
}
