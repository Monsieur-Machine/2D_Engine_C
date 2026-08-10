#ifndef LEVER_COMPONENT_H
#define LEVER_COMPONENT_H

#include "ecs.h"
#include "animation_system.h"
#include "vector2d.h"

#define LEVER_DEFAULT_ANIMATION_SPEED 100

typedef struct lever_component_data_t {
    animation_system_data_t *animation_system_data;

    bool state;
    bool last_state;

    size_t topic_id;
} lever_component_data_t;

int lever_component_init(component_t *component);

int lever_component_update(component_t *component);

int lever_component_destroy(component_t *component);

#endif // LEVER_COMPONENT_H