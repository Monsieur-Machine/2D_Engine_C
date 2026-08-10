#include "lever_component.h"

#include <stdarg.h>
#include "game.h"
#include "animation_system.h"
#include "interact_component.h"
#include "event_bus.h"
#include "memory_alloc.h"
#include "errors.h"

#include "hitbox_component.h"

// Args = entity_t *entity, size_t state
static inline int lever_component_callback(va_list args, void *subscription_data)
{
    const entity_t *entity = subscription_data;

    component_t *lever_component = entity_get_component(entity, LEVER_COMPONENT);
    if(!lever_component) return EXIT_FAILURE;

    lever_component_data_t *component_data = lever_component->data;

    component_data->state = (bool)va_arg(args, size_t);

    return EXIT_SUCCESS;
}

inline int lever_component_init(component_t *component)
{
    lever_component_data_t *component_data = component->data;

    component_t *animation_system = entity_get_component(component->entity, ANIMATION_SYSTEM);
    if(!animation_system)
    {
        error_printf("Lever component require an animation system.");
        return EXIT_FAILURE;
    }
    component_data->animation_system_data = animation_system->data;

    animation_system_data_t *animation_system_data = component_data->animation_system_data;

    animation_system_data->nb_frames = 3;
    animation_system_data->frame_delay_ms = LEVER_DEFAULT_ANIMATION_SPEED;
    if(animation_system_create_frames_clips(animation_system_data)) return EXIT_FAILURE;

    component_data->topic_id = 0;

    event_bus_subscribe(&game.event_bus, component_data->topic_id, lever_component_callback, component->entity);

    component_data->state = false;
    component_data->last_state = false;

    return EXIT_SUCCESS;
}

inline int lever_component_update(component_t *component)
{
    lever_component_data_t *component_data = component->data;
    animation_system_data_t *animation_system_data = component_data->animation_system_data;

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
