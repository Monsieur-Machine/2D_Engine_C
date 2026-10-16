#ifndef MAP_H
#define MAP_H

#include <stdint.h>
#include "linked_list.h"
#include "texture.h"

#define TILE_SIZE 16

typedef struct map_entity_component_def_t {
    uint16_t type;
    void *attributes;
} map_entity_component_def_t;

typedef struct map_entity_def_t {
    uint32_t id;
    uint16_t components_nb;
    map_entity_component_def_t *components_defs;
} map_entity_def_t;

typedef struct map_t {
    uint16_t width, height;
    uint8_t backgroundlayernb;
    linked_list_t backgroundlayers;
    uint16_t *foregroundlayer;
    texture_t *tileset;
    uint8_t *hitboxes;
    uint16_t entities_nb;
    map_entity_def_t *entities_defs;
} map_t;

map_t *load_map(void);

int destroy_map(map_t *map);

int map_load_entities(map_t *map);

int map_remove_entities(map_t *map);

int map_display_background(void);

int map_display_foreground(void);

#if DEBUG > 1
int map_display_hitboxes(void);
#endif // DEBUG > 1

#endif // MAP_H