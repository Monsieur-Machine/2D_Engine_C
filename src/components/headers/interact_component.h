#ifndef INTERACT_COMPONENT_H
#define INTERACT_COMPONENT_H

#include "ecs.h"
#include "vector2d.h"
#include "errors.h"

typedef enum interact_type_t {
    INTERACT_TOGGLE
} interact_type_t;

typedef struct interact_component_data_t {
    component_t *transform_component;

    fvector2d_t position;
    fvector2d_t scale;
    frect_t bounds;

    interact_type_t type;
    size_t state;
    size_t topic_id;

    bool activated;
} interact_component_data_t;

int interact_component_init(component_t *component);
int interact_component_update(component_t *component);
#if DEBUG >= 2
int interact_component_draw(const component_t *component);
#endif // DEBUG >= 2
int interact_component_destroy(component_t *component);

int interact_component_interact(component_t *component);
int interact_component_modify_topic_id(component_t *component, size_t id);

#endif // TRANSFORM_COMPONENT_H