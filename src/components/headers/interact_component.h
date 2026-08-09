#ifndef INTERACT_COMPONENT_H
#define INTERACT_COMPONENT_H

#include "ecs.h"
#include "transform_component.h"
#include "vector2d.h"
#include "errors.h"

typedef struct interact_component_data_t {
    transform_component_data_t *transform_component_data;
    fvector2d_t position;
    fvector2d_t scale;
    frect_t bounds;
    bool activated;
} interact_component_data_t;

int interact_component_init(component_t *component);

int interact_component_update(component_t *component);

#if DEBUG >= 2
int interact_component_draw(const component_t *component);
#endif // DEBUG >= 2

int interact_component_destroy(component_t *component);

#endif // TRANSFORM_COMPONENT_H