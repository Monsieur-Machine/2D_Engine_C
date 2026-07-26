#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "ecs.h"
#include "vector2d.h"

typedef struct transform_component_data_t {
    frect_t bounds;
    fvector2d_t velocity;
    float speed;
} transform_component_data_t;

int transform_component_init(component_t *, va_list);

int transform_component_update(component_t *);

void transform_component_destroy(component_t *);

#endif // TRANSFORM_COMPONENT_H