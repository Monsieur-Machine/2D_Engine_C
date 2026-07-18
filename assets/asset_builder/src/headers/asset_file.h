#ifndef ASSET_FILE_H
#define ASSET_FILE_H

#include <stdint.h>
#include <stddef.h>

typedef struct asset_def_t {
    char *name;
    uint32_t start, end;
} asset_def_t;

typedef struct asset_file_header_t {
    uint32_t assets_number;
    asset_def_t *assets_def;

    size_t header_size;
} asset_file_header_t;

int parse_file_for_header(const char *file_name, asset_def_t *asset_def);

#endif // ASSET_FILE_H