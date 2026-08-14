#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include "ecs.h"
#include "asset_manager.h"
#include "texture.h"
#include "vector2d.h"

typedef struct sprite_component_data_t {
    component_t *transform_component;

    texture_t *texture;
    rect_t src_rect;
    frect_t dst_rect;
    bool flip;
} sprite_component_data_t;

int sprite_component_init(component_t *component);
int sprite_component_update(component_t *component);
int sprite_component_draw(const component_t *component);
int sprite_component_destroy(component_t *component);

#endif // SPRITE_COMPONENT_H