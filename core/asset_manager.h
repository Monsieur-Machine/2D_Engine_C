#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <stdio.h>
#include <stdint.h>
#include "linked_list.h"

typedef enum asset_type_t {
    ASSET_NULL,
    ASSET_TEXTURE,
    ASSET_MAP
} asset_type_t;

typedef struct asset_t {
    const char *name;

    void *data;

    uint32_t nb_refs;

    asset_type_t type;
} asset_t;

int create_asset(asset_t *asset, va_list args);
int destroy_asset(asset_t *asset);

typedef struct asset_header_def_t {
    char *name;
    uint32_t start;
} asset_header_def_t;

int create_asset_header_def(asset_header_def_t *asset_header_def, va_list args);
int destroy_asset_header_def(asset_header_def_t *asset_header_def);

typedef struct asset_manager_t {
    linked_list_t assets_header_defs;
    linked_list_t assets;
    FILE *asset_file;
} asset_manager_t;

int asset_manager_init(void);
int asset_manager_exit(void);
int asset_manager_collect_garbage(void);

asset_t *asset_manager_load_asset(char *asset_name, asset_type_t asset_type);
int asset_manager_let_go_asset(char *asset_name);
asset_t *asset_manager_get_asset(char *asset_name);

#endif // ASSET_MANAGER_H