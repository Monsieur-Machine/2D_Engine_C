#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "ecs.h"
#include "vector2d.h"

typedef struct physics_system_data_t {
    component_t *transform_component;
    component_t *hitbox_component;

    fvector2d_t target_velocity;
    fvector2d_t velocity;
    float speed;
    float smoothing_factor;
} physics_system_data_t;

int physics_system_init(component_t *component);
int physics_system_update(component_t *component);
int physics_system_destroy(component_t *component);

#endif // PHYSICS_SYSTEM_H