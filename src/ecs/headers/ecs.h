#ifndef ECS_H
#define ECS_H

#include <stdarg.h>
#include <stddef.h>
#include "linked_list.h"

typedef enum component_type_t {
#define COMPONENT(NAME, PREFIX, DRAW) NAME,
#include "components.def"
} component_type_t;

// Component forwarding
typedef struct component_t component_t;

// Component init function. It is called when component added to entity. This function is necessary.
typedef void (*component_init_t)(component_t *component, va_list args);
// Component update function. It is called in the main loop to update the component. This function is necessary.
typedef void (*component_update_t)(component_t *component);
// Component draw function. It is called in the main loop to draw the component on screen. This function is not necessary.
typedef void (*component_draw_t)(component_t *component);
// Component deleter function. It is called when the component is removed. It should free the data in the component data but not the component data itself.
typedef void (*component_deleter_t)(component_t *component);

typedef struct component_t {
    component_type_t component_type;

    component_init_t component_init;
    component_update_t component_update;
    component_draw_t component_draw;

    void *component_data;

    component_deleter_t component_deleter;

    struct entity_t *entity;
} component_t;

component_t *create_component(component_type_t component_type);

void destroy_component(component_t *component);

typedef struct entity_t {
    unsigned int id;
    size_t draw_priority;
    linked_list_t components;
} entity_t;

entity_t *create_entity(const unsigned int id);

void add_component(entity_t *entity, component_t *component, ...);

component_t *get_component(entity_t *entity, component_type_t component_type);

void update_entity(entity_t *entity);

void draw_entity(entity_t *entity);

void destroy_entity(entity_t *entity);

typedef struct entity_manager_t {
    linked_list_t entities;
} entity_manager_t;

void entity_manager_init(entity_manager_t *entity_manager);

void entity_manager_add_entity(entity_manager_t *entity_manager, entity_t *entity);

void entity_manager_update(entity_manager_t *entity_manager);

void entity_manager_draw(entity_manager_t *entity_manager);

void entity_manager_remove_entity(entity_manager_t *entity_manager, const unsigned int id);

void entity_manager_clear(entity_manager_t *entity_manager);

#endif // ECS_H