#include "door_component.h"

#include <stdarg.h>
#include "game.h"
#include "animation_system.h"
#include "hitbox_component.h"
#include "event_bus.h"
#include "memory_alloc.h"
#include "errors.h"

// Event arg = size_t state, Subscription arg = entity_t *entity
static inline int door_component_callback(va_list event_args, void *subscription_data)
{
    const entity_t *entity = subscription_data;

    component_t *door_component = entity_get_component(entity, DOOR_COMPONENT);
    if(!door_component)
    {
        error_printf("Failed to get door component.");
        return EXIT_FAILURE;
    }

    door_component_data_t *component_data = door_component->data;

    component_data->state = (bool)va_arg(event_args, size_t);

    return EXIT_SUCCESS;
}

inline int door_component_init(component_t *component)
{
    door_component_data_t *component_data = component->data;

    component_t *animation_system = entity_get_component(component->entity, ANIMATION_SYSTEM);
    if(!animation_system)
    {
        error_printf("Door component require an animation system.");
        return EXIT_FAILURE;
    }
    component_data->animation_system_data = animation_system->data;

    component_t *hitbox_component = entity_get_component(component->entity, HITBOX_COMPONENT);
    if(!hitbox_component)
    {
        error_printf("Door component require an hitbox component.");
        return EXIT_FAILURE;
    }
    component_data->hitbox_component_data = hitbox_component->data;

    component_data->last_state = false;

    return EXIT_SUCCESS;
}

inline int door_component_update(component_t *component)
{
    door_component_data_t *component_data = component->data;
    animation_system_data_t *animation_system_data = component_data->animation_system_data;
    hitbox_component_data_t *hitbox_component_data = component_data->hitbox_component_data;

    if(component_data->state != component_data->last_state)
    {
        animation_system_data->play = true;
        animation_system_data->reverse = !component_data->state;

        hitbox_component_data->activated = !component_data->state;
    }

    component_data->last_state = component_data->state;

    return EXIT_SUCCESS;
}

inline int door_component_destroy(component_t *component)
{
    door_component_data_t *component_data = component->data;

    free(component_data);

    return EXIT_SUCCESS;
}

inline int door_component_subscribe(door_component_data_t *component_data, entity_t *entity)
{
    if(event_bus_subscribe(&game.event_bus, component_data->topic_id, door_component_callback, entity)) return_failure_int;

    return EXIT_SUCCESS;
}
