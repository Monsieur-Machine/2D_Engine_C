#include "physics_system.h"

#include "memory_alloc.h"
#include "game.h"

// Args : frect_t bounds, double speed
int physics_system_init(component_t *component)
{
    physics_system_data_t *system_data = component->data;

    system_data->transform_component_data = entity_get_component(component->entity, TRANSFORM_COMPONENT)->data;
    if(!system_data->transform_component_data) return EXIT_FAILURE;

    system_data->target_velocity = (fvector2d_t) {
        .x = 0.0f,
        .y = 0.0f
    };

    system_data->velocity = (fvector2d_t) {
        .x = 0.0f,
        .y = 0.0f
    };

    system_data->speed = 0.0f;

    system_data->smoothing_factor = 1.0f;

    return EXIT_SUCCESS;
}

static inline void physics_system_update_pos(physics_system_data_t *system_data)
{
    transform_component_data_t *transform_component_data = system_data->transform_component_data;

    system_data->velocity.x = lerp(system_data->velocity.x, system_data->target_velocity.x, system_data->smoothing_factor);
    system_data->velocity.y = lerp(system_data->velocity.y, system_data->target_velocity.y, system_data->smoothing_factor);

    transform_component_data->bounds.x += system_data->velocity.x * system_data->speed * (float)game.delta_time_ms / 1000.0f;
    transform_component_data->bounds.y += system_data->velocity.y * system_data->speed * (float)game.delta_time_ms / 1000.0f;
}

int physics_system_update(component_t *component)
{
    physics_system_data_t *system_data = component->data;

    physics_system_update_pos(system_data);

//    linked_list_for_each(&game.entity_manager.entities, );

    return EXIT_SUCCESS;
}

inline int physics_system_destroy(component_t *component)
{
    physics_system_data_t *system_data = component->data;

    free(system_data);

    return EXIT_SUCCESS;
}
