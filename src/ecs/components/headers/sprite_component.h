#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include "ecs.h"
#include "transform_component.h"
#include "texture_manager.h"

typedef struct sprite_component_data_t {
    transform_component_data_t *transform_component_data;

    texture_t *texture;

    rect_t src_rect;
    rect_t dst_rect;

    bool flip;
} sprite_component_data_t;

void sprite_component_init(component_t *, va_list);

void sprite_component_update(component_t *);

void sprite_component_draw(component_t *);

void sprite_component_destroy(component_t *);

void sprite_component_set_texture(sprite_component_data_t *, const char *);

#endif // SPRITE_COMPONENT_H