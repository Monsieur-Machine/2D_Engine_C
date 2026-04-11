#ifndef MAPS_H
#define MAPS_H

#include "vector2d.h"

typedef unsigned int tile_t;

typedef struct builtin_map_t {
    char *entities;
    size_t width;
    size_t height;
    tile_t *tiles;
} builtin_map_t;

#endif // MAPS_H