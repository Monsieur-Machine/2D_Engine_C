#include <stdio.h>

#include "asset_file.h"

void print_error(const char *content)
{
    printf("\033[31merror\033[m: %s\n", content);
}

int main(int argc, char *argv[])
{
    // Verify arguments minimum number.
    printf("%d args\n", argc);

    if(argc < 3)
    {
        print_error("not enough args.");
        return -1;
    }

    for(size_t i = 0; i < (size_t)argc; i++)
        puts(argv[i]);

    // Create and open output file.
    FILE *output_asset = fopen(argv[2], "wb");

    if(!output_asset)
    {
        print_error("could not create output file.");
        return -1;
    }

    // Parse input files to create header structure.
    asset_file_header_t asset_file_header;

    asset_file_header.assets_number = argc - 2;

    for(size_t i = 2; i < (size_t)argc; i++)
    {
        asset_def_t asset_def;
        int ret = parse_file_for_header(argv[i], &asset_def);
        if(ret)
        {
            puts(argv[i]);

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
            }
        }
    }

    return 0;
}
