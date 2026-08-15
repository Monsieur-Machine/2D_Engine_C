#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asset_file.h"
#include "errors.h"

int main(int argc, char *argv[])
{
    int exit_status = EXIT_FAILURE;

    FILE *output_asset_file = NULL;

    // Verify arguments minimum number.
    if(argc < 3)
    {
        puts("\033[33musage\033[m: asset_builder output_file input_file [input_file_2 ...]");
        goto end;
    }

    // Header size and content.
    asset_file_header_t asset_file_header;
    asset_file_header.assets_number = argc - 2;
    asset_file_header.header_size = sizeof(asset_file_header.assets_number) + sizeof(uint32_t) * asset_file_header.assets_number;
    asset_file_header.assets_header_def = malloc(asset_file_header.assets_number * sizeof(asset_header_def_t));
    if(!asset_file_header.assets_header_def)
    {
        error_printf("Failed to allocate memory.");
        goto end;

    }

    // Set assets name.
    for(uint32_t i = 0; i < asset_file_header.assets_number; i++)
    {
        const char *file_name = argv[i + 2];

        const char *file_name_last_slash = strrchr(file_name, '/');
        const char *file_base_name = file_name_last_slash ? file_name_last_slash + 1 : file_name;

        const char *file_name_dot = strrchr(file_name, '.');
        const char *file_name_ext = file_name_dot ? file_name_dot + 1 : file_name;

        size_t asset_name_length = strlen(file_base_name) - strlen(file_name_ext);

        asset_file_header.assets_header_def[i].name = malloc(asset_name_length);
        asset_file_header.assets_header_def[i].name[asset_name_length - 1] = '\0';
        strncpy(asset_file_header.assets_header_def[i].name, file_base_name, asset_name_length - 1);

        // Update header size.
        asset_file_header.header_size += asset_name_length;
    }

    // Parse input files to create header structure and fill output file.
    output_asset_file = fopen(argv[1], "wb");
    if(!output_asset_file)
    {
        error_printf("could not create output file.");
        goto end;
    }

    for(uint32_t i = 0; i < asset_file_header.assets_number; i++)
    {
        if(parse_file_for_header(argv[i + 2], &asset_file_header.assets_header_def[i])) return_failure_int;

        // Calculate absolute position of assets in the file.
        if(i) asset_file_header.assets_header_def[i].start = asset_file_header.assets_header_def[i - 1].end;
        else  asset_file_header.assets_header_def[0].start = asset_file_header.header_size;
        asset_file_header.assets_header_def[i].end += asset_file_header.assets_header_def[i].start;
    }

    if(write_header_to_file(&asset_file_header, output_asset_file))
    {
        error_printf("Failed to write to output file : %d", errno);
        goto end;
    }

    for(uint32_t i = 0; i < asset_file_header.assets_number; i++)
    {
        if(parse_file_for_content_and_write(argv[i + 2], output_asset_file)) return_failure_int;
    }

    exit_status = EXIT_SUCCESS;

end:
    if(asset_file_header.assets_header_def)
    {
        for(uint32_t i = 0; i < asset_file_header.assets_number; i++)
        {
            if(asset_file_header.assets_header_def[i].name)
            {
                free(asset_file_header.assets_header_def[i].name);
            }
        }

        free(asset_file_header.assets_header_def);
    }

    if(output_asset_file)
        fclose(output_asset_file);

    return exit_status;
}
