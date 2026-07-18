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
    // Verify arguments minimum number.
    if(argc < 3)
    {
        printf("\033[33musage\033[m: asset_builder output_file input_file [input_file_2 ...]");
        return -1;
    }

    for(size_t i = 0; i < (size_t)argc; i++)
        puts(argv[i]);

    // Create and open output file.
    FILE *output_asset = fopen(argv[1], "wb");

    if(!output_asset)
    {
        print_error("could not create output file.");
        return -1;
    }

    // Parse input files to create header structure.
    asset_file_header_t asset_file_header;

    asset_file_header.assets_number = argc - 2;
    asset_file_header.assets_def = malloc(asset_file_header.assets_number * sizeof(asset_def_t));
    asset_file_header.header_size = sizeof(uint32_t) + sizeof(uint32_t) * 2 * asset_file_header.assets_number;

    for(size_t i = 0; i < asset_file_header.assets_number; i++)
    {
        // Fill asset_def.
        int ret = parse_file_for_header(argv[i + 2], &asset_file_header.assets_def[i]);
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
        }
        asset_file_header.header_size += strlen(asset_file_header.assets_def[i].name);
    }

    asset_file_header.assets_def[0].start = asset_file_header.header_size + 1;
    asset_file_header.assets_def[0].end += asset_file_header.assets_def[0].start;

    for(size_t i = 1; i < asset_file_header.assets_number; i++)
    {
        // Adjust start and end.
        asset_file_header.assets_def[i].start = asset_file_header.assets_def[i - 1].end + 1;
        asset_file_header.assets_def[i].end += asset_file_header.assets_def[i].start;
    }

    for(size_t i = 0; i < asset_file_header.assets_number; i++)
    {
        printf("%s : %d - %d\n", asset_file_header.assets_def[i].name, asset_file_header.assets_def[i].start, asset_file_header.assets_def[i].end);
    }

    return 0;
}
