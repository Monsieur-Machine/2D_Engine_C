#include <stdarg.h>
#include "memory_alloc.h"
#include "ecs.h"
#include "components.h"

component_t *create_component(component_type_t component_type)
{
    component_t *component = malloc(sizeof(component_t));
    component->component_type = component_type;

    switch(component_type)
    {
      case TRANSFORM_COMPONENT:
        component->component_init = transform_component_init;
        component->component_update = transform_component_update;
        component->component_draw = NULL;

        component->component_data.transform_component_data = malloc(sizeof(transform_component_data_t));
        if(!component->component_data.transform_component_data) return NULL;

        component->component_deleter = transform_component_destroy;
        break;

      case SPRITE_COMPONENT:
        component->component_init = sprite_component_init;
        component->component_update = sprite_component_update;
        component->component_draw = sprite_component_draw;

        component->component_data.sprite_component_data = malloc(sizeof(sprite_component_data_t));
        if(!component->component_data.sprite_component_data) return NULL;

        component->component_deleter = sprite_component_destroy;
        break;

      case ANIMATION_SYSTEM:
        component->component_init = animation_system_init;
        component->component_update = animation_system_update;
        component->component_draw = NULL;

        component->component_data.animation_system_data = malloc(sizeof(animation_system_data_t));
        if(!component->component_data.animation_system_data) return NULL;

        component->component_deleter = animation_system_destroy;
        break;

      case PLAYER_SYSTEM:
        component->component_init = player_system_init;
        component->component_update = player_system_update;
        component->component_draw = NULL;

        component->component_data.player_system_data = malloc(sizeof(player_system_data_t));
        if(!component->component_data.player_system_data) return NULL;

        component->component_deleter = player_system_destroy;
        break;

      // Add new components attributions here

      default:
        return NULL;
    }

    return component;
}

inline void destroy_component(component_t *component)
{
    component->component_deleter(component);
    free(component);
}

entity_t *create_entity(const unsigned int id)
{
    entity_t *entity = malloc(sizeof(entity_t));
    if(!entity) return NULL;

    entity->id = id;
    entity->draw_priority = 0;

    linked_list_init(&entity->components, sizeof(component_t), (deleter_t)destroy_component);

    return entity;
}

void add_component(entity_t *entity, component_t *component, ...)
{
    if(!component) return;

    component->entity = entity;

    va_list args, args_copy;

    va_start(args, component);
    va_copy(args_copy, args);

    component->component_init(component, args_copy);

    va_end(args_copy);
    va_end(args);

    linked_list_push_back(&entity->components, component);
}

// Only for this file
// Arg : component_type_t component_type
static inline bool condition_is_component_type(elem_t *elem, va_list args)
{
    return ((component_t *)elem->data)->component_type == va_arg(args, component_type_t);
}

inline component_t *get_component(entity_t *entity, component_type_t component_type)
{
    return linked_list_get_if(&entity->components, condition_is_component_type, component_type);
}

// Only for this file
// No args
static inline void action_update_component(elem_t *elem, va_list args __attribute__((unused)))
{
    component_t *component = elem->data;
    component->component_update(component);
}

inline void update_entity(entity_t *entity)
{
    linked_list_for_each(&entity->components, action_update_component);
}

// Only for this file
// No args
static inline void action_draw_component(elem_t *elem, va_list args __attribute__((unused)))
{
    component_t *component = elem->data;
    if(component->component_draw) component->component_draw(component);
}

inline void draw_entity(entity_t *entity)
{
    linked_list_for_each(&entity->components, action_draw_component);
}

inline void destroy_entity(entity_t *entity)
{
    linked_list_clear(&entity->components);
    free(entity);
}

inline void entity_manager_init(entity_manager_t *entity_manager)
{
    linked_list_init(&entity_manager->entities, sizeof(entity_t), (deleter_t)destroy_entity);
}

inline void entity_manager_add_entity(entity_manager_t *entity_manager, entity_t *entity)
{
    linked_list_push_back(&entity_manager->entities, entity);
}

// Only for this file
// No args
static inline void action_update_entity(elem_t *elem, va_list args __attribute__((unused)))
{
    update_entity(elem->data);
}

inline void entity_manager_update(entity_manager_t *entity_manager)
{
    linked_list_for_each(&entity_manager->entities, action_update_entity);
}

// Only for this file
// No args
static inline void action_draw_entity(elem_t *elem, va_list args __attribute__((unused)))
{
    draw_entity(elem->data);
}

inline void entity_manager_draw(entity_manager_t *entity_manager)
{
    linked_list_for_each(&entity_manager->entities, action_draw_entity);
}

// Only for this file
// Arg : unsigned int id
static inline bool condition_is_entity_name(elem_t *elem, va_list args)
{
    return ((entity_t *)elem->data)->id == va_arg(args, unsigned int);
}

inline void entity_manager_remove_entity(entity_manager_t *entity_manager, unsigned int id)
{
    linked_list_remove_if(&entity_manager->entities, condition_is_entity_name, id);
}

inline void entity_manager_clear(entity_manager_t *entity_manager)
{
    linked_list_clear(&entity_manager->entities);
}
