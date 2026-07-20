#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asset_file.h"

void print_error(const char *content)
{
    printf("\033[31merror\033[m: %s\n", content);
}

int main(int argc, char *argv[])
{
    int exit_status = EXIT_SUCCESS;

    // Verify arguments minimum number.
    if(argc < 3)
    {
        puts("\033[33musage\033[m: asset_builder output_file input_file [input_file_2 ...]");
        exit_status = EXIT_FAILURE;
        goto end;
    }

    // Header size and content.
    asset_file_header_t asset_file_header;
    asset_file_header.assets_number = argc - 2;
    asset_file_header.header_size = sizeof(size_t) + sizeof(size_t) * asset_file_header.assets_number;
    asset_file_header.assets_header_def = malloc(asset_file_header.assets_number * sizeof(asset_header_def_t));

    // Set assets name.
    for(size_t i = 0; i < asset_file_header.assets_number; i++)
    {
        const char *file_name = argv[i + 2];

        const char *file_name_last_slash = strrchr(file_name, '/');
        const char *file_base_name = file_name_last_slash ? file_name_last_slash + 1 : file_name;

        const char *file_name_dot = strrchr(file_name, '.');
        const char *file_name_ext = file_name_dot ? file_name_dot + 1 : file_name;

        size_t asset_name_length = strlen(file_base_name) - strlen(file_name_ext) - 1;

        asset_file_header.assets_header_def[i].name = malloc(asset_name_length);
        strncpy(asset_file_header.assets_header_def[i].name, file_base_name, asset_name_length);

        // Update header size.
        asset_file_header.header_size += asset_name_length + 1;
    }

    // Parse input files to create header structure and fill output file.
    FILE *output_asset_file = fopen(argv[1], "wb");

    if(!output_asset_file)
    {
        print_error("could not create output file.");
        exit_status = EXIT_FAILURE;
        goto end;
    }

    for(size_t i = 0; i < asset_file_header.assets_number; i++)
    {
        int ret = parse_file_for_header(argv[i + 2], &asset_file_header.assets_header_def[i]);

        if(ret)
        {
            switch(ret)
            {
              case -1:
                print_error("file does not have an extension.");
                return -1;

              case -2:
                print_error("extension not yet supported.");
                return -1;

              case -3:
                print_error("failed to open file.");
                return -1;

              case -4:
                print_error("file not valid.");
                return -1;

              case -5:
                print_error("memory allocation failed.");
                return -1;

              case -6:
                print_error("png bit depth not supported.");
                return -1;

              case -7:
                print_error("png color type not supported.");
                return -1;

              default:
                print_error("unknown error");
                return -1;
            }

            exit_status = EXIT_FAILURE;
            goto end;
        }

        // Calculate absolute position of assets in the file.
        if(i) asset_file_header.assets_header_def[i].start = asset_file_header.assets_header_def[i - 1].end + 1;
        else  asset_file_header.assets_header_def[0].start = asset_file_header.header_size;
        asset_file_header.assets_header_def[i].end += asset_file_header.assets_header_def[i].start;
    }

    if(write_header_to_file(&asset_file_header, output_asset_file))
    {
        print_error("can't write to output file.");
        exit_status = EXIT_FAILURE;
        goto end;
    }

    for(size_t i = 0; i < asset_file_header.assets_number; i++)
    {
        int ret = parse_file_for_content_and_write(argv[i + 2], output_asset_file);

        if(ret)
        {
            switch(ret)
            {
              case -3:
                print_error("failed to open file.");
                return -1;

              case -4:
                print_error("file not valid.");
                return -1;

              case -5:
                print_error("memory allocation failed.");
                return -1;

              case -6:
                print_error("png bit depth not supported.");
                return -1;

              case -7:
                print_error("png color type not supported.");
                return -1;

              case -8:
                print_error("failed writing to output file.");
                return -1;

              default:
                print_error("unknown error");
                return -1;
            }

            exit_status = EXIT_FAILURE;
            goto end;
        }
    }

end:
    if(output_asset_file) fclose(output_asset_file);

    return exit_status;
}
