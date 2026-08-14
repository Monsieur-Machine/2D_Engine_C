#include "lever_component.h"

#include <stdarg.h>
#include "game.h"
#include "ecs.h"
#include "animation_system.h"
#include "interact_component.h"
#include "event_bus.h"
#include "memory_alloc.h"
#include "errors.h"

inline int lever_component_init(component_t *component)
{
    lever_component_data_t *component_data = component->data;

    component_data->animation_system = entity_get_component(component->entity, ANIMATION_SYSTEM);
    if(!component_data->animation_system)
    {
        error_printf("Lever component require an animation system.");
        return EXIT_FAILURE;
    }

    component_data->last_state = false;

    return EXIT_SUCCESS;
}

inline int lever_component_update(component_t *component)
{
    lever_component_data_t *component_data = component->data;
    animation_system_data_t *animation_system_data = component_data->animation_system->data;

    if(component_data->state != component_data->last_state)
    {
        animation_system_data->play = true;
        animation_system_data->reverse = !component_data->state;
    }

    component_data->last_state = component_data->state;

    return EXIT_SUCCESS;
}

inline int lever_component_destroy(component_t *component)
{
    lever_component_data_t *component_data = component->data;

    free(component_data);

    return EXIT_SUCCESS;
}

// Event arg : size_t state, Subscription data : component_t *component
static inline int lever_component_callback(void *event_arg, void *subscription_data)
{
    const component_t *component = subscription_data;
    lever_component_data_t *component_data = component->data;

    component_data->state = *(size_t *)event_arg;

    return EXIT_SUCCESS;
}

inline int lever_component_subscribe(component_t *component)
{
    lever_component_data_t *component_data = component->data;

    if(event_bus_subscribe(component_data->topic_id, lever_component_callback, component)) return_failure_int;

    return EXIT_SUCCESS;
}
