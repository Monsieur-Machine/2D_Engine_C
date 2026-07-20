#ifndef ASSET_FILE_H
#define ASSET_FILE_H

#include <stdint.h>
#include <stdio.h>

typedef struct asset_header_def_t {
    char *name;
    size_t start, end;
} asset_header_def_t;

typedef struct asset_file_header_t {
    size_t assets_number;
    asset_header_def_t *assets_header_def;

    size_t header_size;
} asset_file_header_t;

int parse_file_for_header(const char *input_file_name, asset_header_def_t *asset_header_def);

int write_header_to_file(asset_file_header_t *asset_file_header, FILE *output_file);

int parse_file_for_content_and_write(const char *input_file_name, FILE *output_file);

#endif // ASSET_FILE_H