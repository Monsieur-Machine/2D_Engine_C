#include "object.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "utils.h"
#include "image.h"
#include "map.h"
#include "errors.h"

int build_object(const char *input_file_path, FILE *output_file)
{
    // Reserve space to write asset_size
    uint32_t asset_size = 0;
    if(file_write(&asset_size, sizeof(asset_size), 1, output_file)) return_failure_int;

    const char *input_file_dot = strrchr(input_file_path, '.');
    if(!input_file_dot)
    {
        error_printf("Input file \"%s\" doesn't have an extension.", input_file_path);
        return EXIT_FAILURE;
    }

    const char *input_file_ext = input_file_dot + 1;

    if(!strcmp(input_file_ext, "png"))
    {
        if(build_image(input_file_path, output_file, &asset_size)) return_failure_int;
    }
    else if(!strcmp(input_file_ext, "map"))
    {
        if(build_map(input_file_path, output_file, &asset_size)) return_failure_int;
    }
    else
    {
        error_printf("Extension \"%s\" not supported.", input_file_ext);
        return EXIT_FAILURE;
    }

    if(fseek(output_file, 0, SEEK_SET))
    {
        error_printf("Failed to seek in output file : %d", errno);
        return EXIT_FAILURE;
    }

    if(file_write(&asset_size, sizeof(asset_size), 1, output_file)) return_failure_int;

    return EXIT_SUCCESS;
}