#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include "ecs.h"
#include "transform_component.h"
#include "asset_manager.h"

typedef struct sprite_component_data_t {
    transform_component_data_t *transform_component_data;

    texture_t *texture;

    rect_t src_rect;
    frect_t dst_rect;

    bool flip;
} sprite_component_data_t;

int sprite_component_init(component_t *component);

int sprite_component_update(component_t *component);

int sprite_component_draw(const component_t *component);

int sprite_component_destroy(component_t *component);

int sprite_component_set_texture(sprite_component_data_t *component_data, const char *texture_name);

#endif // SPRITE_COMPONENT_H