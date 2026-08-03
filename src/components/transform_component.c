#include "transform_component.h"

#include "memory_alloc.h"
#include "game.h"

// Args : frect_t bounds, double speed
int transform_component_init(component_t *component)
{
    transform_component_data_t *component_data = component->component_data;

    component_data->bounds = (frect_t) {
        .x = 0.0f,
        .y = 0.0f,
        .w = 32.0f,
        .h = 32.0f
    };

    component_data->velocity = (fvector2d_t) {
        .x = 0.0f,
        .y = 0.0f
    };

    component_data->speed = 0.0f;

    return EXIT_SUCCESS;
}

inline int transform_component_update(component_t *component)
{
    transform_component_data_t *component_data = component->component_data;

    component_data->bounds.x += component_data->velocity.x * component_data->speed * (float)game.delta_time_ms / 1000.0f;
    component_data->bounds.y += component_data->velocity.y * component_data->speed * (float)game.delta_time_ms / 1000.0f;

    return EXIT_SUCCESS;
}

#if DEBUG >= 2
#include "display.h"
#include "errors.h"

inline int transform_component_draw(component_t *component)
{
    transform_component_data_t *component_data = component->component_data;

    if(display_draw_frect(&component_data->bounds, C_BLUE, 128)) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
#endif // DEBUG >= 2

inline int transform_component_destroy(component_t *component)
{
    transform_component_data_t *component_data = component->component_data;

    free(component_data);

    return EXIT_SUCCESS;
}
