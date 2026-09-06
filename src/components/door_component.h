#ifndef DOOR_COMPONENT_H
#define DOOR_COMPONENT_H

#include "ecs.h"

#define BARRED_DOOR_DEFAULT_ANIMATION_SPEED 30
#define WOODEN_DOOR_DEFAULT_ANIMATION_SPEED 100

typedef struct door_component_data_t {
    component_t *animation_system;
    component_t *hitbox_component;

    bool state;
    bool last_state;

    size_t topic_id;
} door_component_data_t;

int door_component_init(component_t *component);
int door_component_update(component_t *component);
int door_component_destroy(component_t *component);

int door_component_subscribe(component_t *component);

#endif // DOOR_COMPONENT_H