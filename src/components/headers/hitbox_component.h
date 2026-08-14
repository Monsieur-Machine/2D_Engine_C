#ifndef HITBOX_COMPONENT_H
#define HITBOX_COMPONENT_H

#include <stdbool.h>
#include "ecs.h"
#include "transform_component.h"
#include "vector2d.h"
#include "errors.h"

typedef struct hitbox_component_data_t {
    transform_component_data_t *transform_component_data;

    fvector2d_t position;
    fvector2d_t scale;
    frect_t bounds;

    bool activated;
} hitbox_component_data_t;

int hitbox_component_init(component_t *component);
int hitbox_component_update(component_t *component);
#if DEBUG >= 2
int hitbox_component_draw(const component_t *component);
#endif
int hitbox_component_destroy(component_t *component);

#endif // HITBOX_COMPONENT_H