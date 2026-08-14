#include "door_component.h"

#include <stdarg.h>
#include "game.h"
#include "ecs.h"
#include "animation_system.h"
#include "hitbox_component.h"
#include "event_bus.h"
#include "memory_alloc.h"
#include "errors.h"

inline int door_component_init(component_t *component)
{
    door_component_data_t *component_data = component->data;

    component_data->animation_system = entity_get_component(component->entity, ANIMATION_SYSTEM);
    if(!component_data->animation_system)
    {
        error_printf("Door component require an animation system.");
        return EXIT_FAILURE;
    }

    component_data->hitbox_component = entity_get_component(component->entity, HITBOX_COMPONENT);
    if(!component_data->hitbox_component)
    {
        error_printf("Door component require an hitbox component.");
        return EXIT_FAILURE;
    }

    component_data->last_state = false;

    return EXIT_SUCCESS;
}

inline int door_component_update(component_t *component)
{
    door_component_data_t *component_data = component->data;
    animation_system_data_t *animation_system_data = component_data->animation_system->data;
    hitbox_component_data_t *hitbox_component_data = component_data->hitbox_component->data;

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

// Event arg : size_t state, Subscription data : entity_t *entity
static inline int door_component_callback(void *event_arg, void *subscription_data)
{
    const component_t *component = subscription_data;
    door_component_data_t *component_data = component->data;

    component_data->state = *(size_t *)event_arg;

    return EXIT_SUCCESS;
}

inline int door_component_subscribe(component_t *component)
{
    door_component_data_t *component_data = component->data;

    if(event_bus_subscribe(component_data->topic_id, door_component_callback, component)) return_failure_int;

    return EXIT_SUCCESS;
}
