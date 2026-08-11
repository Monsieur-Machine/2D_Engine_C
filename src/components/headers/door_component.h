#ifndef DOOR_COMPONENT_H
#define DOOR_COMPONENT_H

#include "ecs.h"
#include "animation_system.h"
#include "hitbox_component.h"

#define BARRED_DOOR_DEFAULT_ANIMATION_SPEED 30
#define WOODEN_DOOR_DEFAULT_ANIMATION_SPEED 100

typedef struct door_component_data_t {
    animation_system_data_t *animation_system_data;
    hitbox_component_data_t *hitbox_component_data;

    bool state;
    bool last_state;

    size_t topic_id;
} door_component_data_t;

int door_component_init(component_t *component);

int door_component_update(component_t *component);

int door_component_destroy(component_t *component);

#endif // DOOR_COMPONENT_H