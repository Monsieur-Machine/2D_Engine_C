#include "ecs.h"

#include <stdarg.h>
#include "game.h"
#include "components.h"
#include "memory_alloc.h"
#include "errors.h"

const char *component_type_to_name(component_type_t component_type)
{
    switch(component_type)
    {
#define COMPONENT(NAME, ...) \
      case NAME: \
        return #NAME;
#include "components.def"

      default:
        return "INVALID";
    }
}

// Arg : component_type_t component_type
int create_component(component_t *component, va_list args)
{
    component->type = va_arg(args, component_type_t);

    switch(component->type)
    {
#define COMPONENT(NAME, PREFIX, DRAW) \
      case NAME: \
        component->init = PREFIX##_init; \
        component->update = PREFIX##_update; \
        component->draw = DRAW(PREFIX); \
        component->data = malloc(sizeof(PREFIX##_data_t)); \
        if(!component->data) \
        { \
            error_printf("Failed to allocate memory : %d", errno); \
            return EXIT_FAILURE; \
        } \
        component->deleter = PREFIX##_destroy; \
        break;
#include "components.def"

      default:
        error_printf("Component type doesn't exists : %d", component->type);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

inline int destroy_component(component_t *component)
{
    if(component->deleter(component)) return_failure_int;

    return EXIT_SUCCESS;
}

int create_entity(entity_t *entity, va_list args)
{
    entity->id = va_arg(args, size_t);
    entity->draw_priority = 0;

    linked_list_init(&entity->components, sizeof(component_t), (allocator_t)create_component, (deleter_t)destroy_component);

    return EXIT_SUCCESS;
}

inline int destroy_entity(entity_t *entity)
{
    if(linked_list_exit(&entity->components)) return_failure_int;

    return EXIT_SUCCESS;
}

component_t *entity_new_component(entity_t *entity, component_type_t component_type)
{
    elem_t *component_elem = create_elem(&entity->components, component_type);
    if(!component_elem) return_failure_ptr;

    component_t *component = component_elem->data;

    component->entity = entity;

    if(component->init(component)) return_failure_ptr;

    linked_list_push_front(&entity->components, component_elem);

    return component;
}

// Only for this file
// Arg : component_type_t component_type
static inline bool condition_is_component_type(elem_t *elem, void *arg)
{
    return ((component_t *)elem->data)->type == *(component_type_t *)arg;
}

inline component_t *entity_get_component(const entity_t *entity, component_type_t component_type)
{
    elem_t *component_elem = linked_list_get_if(&entity->components, condition_is_component_type, &component_type);
    if(!component_elem) return NULL;

    component_t *component = component_elem->data;

    return component;
}

// Only for this file
// No args
static inline int action_update_component(elem_t *elem, void *arg __attribute__((unused)))
{
    component_t *component = elem->data;

    if(component->update(component)) return_failure_int;

    return EXIT_SUCCESS;
}

inline int update_entity(entity_t *entity)
{
    if(linked_list_for_each(&entity->components, action_update_component, NULL)) return_failure_int;

    return EXIT_SUCCESS;
}

// No args
static inline int action_draw_component(elem_t *elem, void *arg __attribute__((unused)))
{
    component_t *component = elem->data;

    if(component->draw)
        if(component->draw(component)) return_failure_int;

    return EXIT_SUCCESS;
}

inline int draw_entity(entity_t *entity)
{
    if(linked_list_for_each(&entity->components, action_draw_component, NULL)) return_failure_int;

    return EXIT_SUCCESS;
}

inline void entity_manager_init(void)
{
    linked_list_init(&game.entity_manager.entities, sizeof(entity_t), (allocator_t)create_entity, (deleter_t)destroy_entity);

    debug_printf("Initialized entity manager.");
}

inline entity_t *entity_manager_new_entity(size_t id)
{
    elem_t *entity_elem = create_elem(&game.entity_manager.entities, id);
    if(!entity_elem) return_failure_ptr;

    linked_list_push_back(&game.entity_manager.entities, entity_elem);

    debug_printf("Added entity %lu to entity manager.", id);

    return entity_elem->data;
}

// No args
static inline int action_update_entity(elem_t *elem, void *arg __attribute__((unused)))
{
    return update_entity((entity_t *)elem->data);
}

inline int entity_manager_update(void)
{
    if(linked_list_for_each(&game.entity_manager.entities, action_update_entity, NULL)) return_failure_int;

    return EXIT_SUCCESS;
}

// No args
static inline bool condition_is_draw_priority_higher(elem_t *elem, void *arg __attribute__((unused)))
{
    return ((entity_t *)elem->data)->draw_priority > ((entity_t *)elem->next->data)->draw_priority;
}

// No args
static inline int action_draw_entity(elem_t *elem, void *arg __attribute__((unused)))
{
    return draw_entity((entity_t *)elem->data);
}

inline int entity_manager_draw(void)
{
    linked_list_insertion_sort(&game.entity_manager.entities, condition_is_draw_priority_higher, NULL);

    if(linked_list_for_each(&game.entity_manager.entities, action_draw_entity, NULL)) return_failure_int;

    return EXIT_SUCCESS;
}

// Only for this file
// Arg : size_t id
static inline bool condition_is_entity_name(elem_t *elem, void *arg)
{
    return ((entity_t *)elem->data)->id == *(size_t *)arg;
}

inline int entity_manager_remove_entity(size_t id)
{
    if(linked_list_remove_if(&game.entity_manager.entities, condition_is_entity_name, &id)) return_failure_int;

    debug_printf("Removed entity %lu from entity manager.", id);

    return EXIT_SUCCESS;
}

inline int entity_manager_exit(void)
{
    if(linked_list_exit(&game.entity_manager.entities)) return_failure_int;

    debug_printf("Exited entity manager.");

    return EXIT_SUCCESS;
}
