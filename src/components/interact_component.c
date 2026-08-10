#include "interact_component.h"

#include "game.h"
#include "event_bus.h"
#include "memory_alloc.h"
#include "errors.h"

inline int interact_component_init(component_t *component)
{
    interact_component_data_t *component_data = component->data;

    component_t *transform_component = entity_get_component(component->entity, TRANSFORM_COMPONENT);
    if(!transform_component)
    {
        error_printf("Hitbox component require a transform component.");
        return EXIT_FAILURE;
    }
    component_data->transform_component_data = transform_component->data;

    component_data->position = (fvector2d_t) {
        .x = 0.0f,
        .y = 0.0f
    };
    component_data->scale = (fvector2d_t) {
        .x = 1.0f,
        .y = 1.0f
    };

    component_data->topic_id = 0;
    component_data->type = INTERACT_TOGGLE;

    component_data->event_payload = 0;

    if(event_bus_new_topic(&game.event_bus, component_data->topic_id, 1)) return EXIT_FAILURE;

    component_data->activated = true;

    return EXIT_SUCCESS;
}

inline int interact_component_update(component_t *component __attribute__((unused)))
{
    interact_component_data_t *component_data = component->data;
    const transform_component_data_t *transform_component_data = component_data->transform_component_data;

    component_data->bounds.x = transform_component_data->bounds.x + (transform_component_data->bounds.w - component_data->bounds.w) * component_data->position.x;
    component_data->bounds.y = transform_component_data->bounds.y + (transform_component_data->bounds.h - component_data->bounds.h) * component_data->position.y;
    component_data->bounds.w = transform_component_data->bounds.w * component_data->scale.x;
    component_data->bounds.h = transform_component_data->bounds.h * component_data->scale.y;

    return EXIT_SUCCESS;
}

#if DEBUG >= 2
#include "display.h"

inline int interact_component_draw(const component_t *component)
{
    const interact_component_data_t *component_data = component->data;

    if(display_draw_frect(&component_data->bounds, C_GREEN, 128)) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
#endif // DEBUG >= 2

inline int interact_component_destroy(component_t *component)
{
    interact_component_data_t *component_data = component->data;

    free(component_data);

    return EXIT_SUCCESS;
}

inline int interact_component_set_topic(interact_component_data_t *component_data, size_t id)
{
    if(event_bus_change_topic_id(&game.event_bus, component_data->topic_id, id)) return EXIT_FAILURE;

    component_data->topic_id = id;

    return EXIT_SUCCESS;
}