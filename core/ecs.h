#ifndef ECS_H
#define ECS_H

#include <stdarg.h>
#include <stddef.h>
#include "linked_list.h"

typedef enum component_type_t {
#define COMPONENT(NAME, ...) NAME,
#include "components.def"
} component_type_t;

const char *component_type_to_name(component_type_t component_type);

// Component forwarding
typedef struct component_t component_t;

// Component init function. It is called when component added to entity. This function is necessary.
typedef int (*component_init_t)(component_t *component);
// Component update function. It is called in the main loop to update the component. This function is necessary.
typedef int (*component_update_t)(component_t *component);
// Component draw function. It is called in the main loop to draw the component on screen. This function is not necessary.
typedef int (*component_draw_t)(const component_t *component);
// Component deleter function. It is called when the component is removed. It should free the data in the component data but not the component itself.
typedef int (*component_deleter_t)(component_t *component);

typedef struct component_t {
    component_init_t init;
    component_update_t update;
    component_draw_t draw;

    void *data;

    component_deleter_t deleter;

    struct entity_t *entity;

    component_type_t type;

} component_t;

int create_component(component_t *component, va_list args);
int destroy_component(component_t *component);

typedef struct entity_t {
    size_t id;
    size_t draw_priority;
    linked_list_t components;
} entity_t;

int create_entity(entity_t *entity, va_list args);
int destroy_entity(entity_t *entity);

component_t *entity_new_component(entity_t *entity, component_type_t component_type);
int update_entity(entity_t *entity);
int draw_entity(entity_t *entity);
component_t *entity_get_component(const entity_t *entity, component_type_t component_type);

typedef struct entity_manager_t {
    linked_list_t entities;
} entity_manager_t;

void entity_manager_init(void);
int entity_manager_exit(void);

entity_t *entity_manager_new_entity(size_t id);
int entity_manager_update(void);
int entity_manager_draw(void);
int entity_manager_remove_entity(size_t id);

#endif // ECS_H