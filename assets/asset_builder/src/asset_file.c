#include "asset_file.h"

#include <stdio.h>
#include <string.h>
#include <png.h>

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
        return -3;
    }

    fclose(input_png_file);

    return 0;

    // Check png autenticity.
    unsigned char sig[8];
    fread(sig, 1, 8, input_png_file);
    if(!png_check_sig(sig, 8))
    {
        fclose(input_png_file);
        return -4;   // bad signature
    }

    // PNG structs
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

    // Fill structs and load data
    png_init_io(png_ptr, input_png_file);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);

    printf("%s bit depth : %d", file_name, png_get_bit_depth(png_ptr, info_ptr));

    fclose(input_png_file);

    return 0;
}
