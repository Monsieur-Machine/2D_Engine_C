#include "asset_file.h"

#include <string.h>
#include <stdlib.h>
#include "image.h"
#include "map.h"
#include "errors.h"

int parse_file_for_header(const char *input_file_name, asset_header_def_t *asset_header_def)
{
    const char *file_name_dot = strrchr(input_file_name, '.');

    if(!file_name_dot)
    {
        error_printf("File does not have an extension.");
        return EXIT_FAILURE;
    }

    const char *file_name_ext = file_name_dot + 1;

    if(!strcmp(file_name_ext, "png"))
    {
        if(parse_png_for_header(input_file_name, asset_header_def)) return_failure_int;
    }
    else if(!strcmp(file_name_ext, "map"))
    {
        if(parse_map_for_header(input_file_name, asset_header_def)) return_failure_int;
    }
    else
    {
        error_printf("Extension unsupported : %s", file_name_dot);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int write_header_to_file(asset_file_header_t *asset_file_header, FILE *output_file)
{
    if(fwrite(&asset_file_header->assets_number, sizeof(asset_file_header->assets_number), 1, output_file) != 1)
    {
        error_printf("Failed to write to output file : %d", errno);
        return EXIT_FAILURE;
    }

    for(uint32_t i = 0; i < asset_file_header->assets_number; i++)
    {
        size_t name_length = strlen(asset_file_header->assets_header_def[i].name) + 1;
        if(fwrite(asset_file_header->assets_header_def[i].name, sizeof(unsigned char), name_length, output_file) != name_length)
        {
            error_printf("Failed to write to output file : %d", errno);
            return EXIT_FAILURE;
        }

        if(fwrite(&asset_file_header->assets_header_def[i].start, sizeof(asset_file_header->assets_header_def[i].start), 1, output_file) != 1)
        {
            error_printf("Failed to write to output file : %d", errno);
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

int parse_file_for_content_and_write(const char *input_file_name, FILE *output_file)
{
    const char *file_name_dot = strrchr(input_file_name, '.');

    if(!file_name_dot)
    {
        error_printf("File does not have an extension.");
        return EXIT_FAILURE;
    }

    const char *file_name_ext = file_name_dot + 1;

    if(!strcmp(file_name_ext, "png"))
    {
        if(parse_png_for_content_and_write(input_file_name, output_file)) return_failure_int;
    }
    else if(!strcmp(file_name_ext, "map"))
    {
        if(parse_map_for_content_and_write(input_file_name, output_file)) return_failure_int;
    }
    else
    {
        error_printf("Extension unsupported : %s", file_name_dot);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
