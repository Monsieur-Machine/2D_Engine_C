#include "hitbox_component.h"

#include "game.h"
#include "transform_component.h"
#include "memory_alloc.h"

// Args : frect_t bounds, double speed
int hitbox_component_init(component_t *component)
{
    hitbox_component_data_t *component_data = component->data;

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

    component_data->activated = true;

    return EXIT_SUCCESS;
}

int hitbox_component_update(component_t *component)
{
    hitbox_component_data_t *component_data = component->data;
    const transform_component_data_t *transform_component_data = component_data->transform_component_data;

    component_data->bounds.x = transform_component_data->bounds.x + (transform_component_data->bounds.w - component_data->bounds.w) * component_data->position.x;
    component_data->bounds.y = transform_component_data->bounds.y + (transform_component_data->bounds.h - component_data->bounds.h) * component_data->position.y;
    component_data->bounds.w = transform_component_data->bounds.w * component_data->scale.x;
    component_data->bounds.h = transform_component_data->bounds.h * component_data->scale.y;

    return EXIT_SUCCESS;
}

#if DEBUG >= 2
#include "display.h"
#include "errors.h"

int hitbox_component_draw(const component_t *component)
{
    const hitbox_component_data_t *component_data = component->data;

    if(component_data->activated)
        if(display_draw_frect(&component_data->bounds, C_RED, 128)) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
#endif // DEBUG >= 2

inline int hitbox_component_destroy(component_t *component)
{
    hitbox_component_data_t *component_data = component->data;

    free(component_data);

    return EXIT_SUCCESS;
}
