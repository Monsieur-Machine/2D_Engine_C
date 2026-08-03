#include "physics_system.h"

#include "memory_alloc.h"
#include "game.h"

// Args : frect_t bounds, double speed
int physics_system_init(component_t *component)
{
    physics_system_data_t *component_data = component->component_data;

    component_data->transform_component_data = entity_get_component(component->entity, TRANSFORM_COMPONENT)->component_data;
    if(!component_data->transform_component_data) return EXIT_FAILURE;

    component_data->velocity = (fvector2d_t) {
        .x = 0.0f,
        .y = 0.0f
    };

    component_data->speed = 0.0f;

    return EXIT_SUCCESS;
}

int physics_system_update(component_t *component)
{
    physics_system_data_t *component_data = component->component_data;
    transform_component_data_t *transform_component_data = component_data->transform_component_data;

    transform_component_data->bounds.x += component_data->velocity.x * component_data->speed * (float)game.delta_time_ms / 1000.0f;
    transform_component_data->bounds.y += component_data->velocity.y * component_data->speed * (float)game.delta_time_ms / 1000.0f;

    return EXIT_SUCCESS;
}

inline int physics_system_destroy(component_t *component)
{
    physics_system_data_t *component_data = component->component_data;

    free(component_data);

    return EXIT_SUCCESS;
}
