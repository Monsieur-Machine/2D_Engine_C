#include "physics_system.h"

#include <stdarg.h>
#include "vector2d.h"
#include "memory_alloc.h"
#include "game.h"
#include "errors.h"

// Args : frect_t bounds, double speed
int physics_system_init(component_t *component)
{
    physics_system_data_t *system_data = component->data;

    system_data->hitbox_component_data = entity_get_component(component->entity, HITBOX_COMPONENT)->data;
    if(!system_data->hitbox_component_data) return EXIT_FAILURE;
    system_data->transform_component_data = system_data->hitbox_component_data->transform_component_data;

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

// Only for this file
// Args : physics_system_data_t *system_data, size_t entity_id
static inline int action_detect_solve_entities_collision(elem_t *elem, va_list args)
{
    const entity_t *target_entity = elem->data;
    physics_system_data_t *system_data = va_arg(args, physics_system_data_t *);

    if(target_entity->id == va_arg(args, size_t))
    {
        return EXIT_SUCCESS;
    }

    const component_t *target_hitbox_component = entity_get_component(target_entity, HITBOX_COMPONENT);
    if(!target_hitbox_component) return EXIT_SUCCESS;

    if(!((hitbox_component_data_t *)target_hitbox_component->data)->activated)
    {
        return EXIT_SUCCESS;
    }

    const frect_t *hitbox_bounds = &system_data->hitbox_component_data->bounds;
    const frect_t *target_hitbox_bounds = &((hitbox_component_data_t *)target_hitbox_component->data)->bounds;
    frect_t intersect_frect;

    if(intersect_frect(hitbox_bounds, target_hitbox_bounds, &intersect_frect))
    {
        if(intersect_frect.w < intersect_frect.h)
        {
            const float left_correction = hitbox_bounds->x + hitbox_bounds->w - target_hitbox_bounds->x;
            const float right_correction = target_hitbox_bounds->x + target_hitbox_bounds->w - hitbox_bounds->x;

            if(left_correction < right_correction)
            {
                system_data->transform_component_data->bounds.x -= left_correction;
                if(system_data->velocity.x > 0.0f) system_data->velocity.x = 0.0f;
            }
            else
            {
                system_data->transform_component_data->bounds.x += right_correction;
                if(system_data->velocity.x < 0.0f) system_data->velocity.x = 0.0f;
            }
        }
        else
        {
            float top_correction = hitbox_bounds->y + hitbox_bounds->h - target_hitbox_bounds->y;
            float bottom_correction = target_hitbox_bounds->y + target_hitbox_bounds->h - hitbox_bounds->y;

            if(top_correction < bottom_correction)
            {
                system_data->transform_component_data->bounds.y -= top_correction;
                if(system_data->velocity.y > 0.0f) system_data->velocity.y = 0.0f;
            }
            else
            {
                system_data->transform_component_data->bounds.y += bottom_correction;
                if(system_data->velocity.y < 0.0f) system_data->velocity.y = 0.0f;
            }
        }
    }

    return EXIT_SUCCESS;
}

int physics_system_update(component_t *component)
{
    physics_system_data_t *system_data = component->data;
    transform_component_data_t *transform_component_data = system_data->transform_component_data;

    system_data->velocity.x = lerp(system_data->velocity.x, system_data->target_velocity.x, system_data->smoothing_factor);
    system_data->velocity.y = lerp(system_data->velocity.y, system_data->target_velocity.y, system_data->smoothing_factor);

    linked_list_for_each(&game.entity_manager.entities, action_detect_solve_entities_collision, system_data, component->entity->id);

    transform_component_data->bounds.x += system_data->velocity.x * system_data->speed * (float)game.delta_time_ms / 1000.0f;
    transform_component_data->bounds.y += system_data->velocity.y * system_data->speed * (float)game.delta_time_ms / 1000.0f;

    return EXIT_SUCCESS;
}

inline int physics_system_destroy(component_t *component)
{
    physics_system_data_t *system_data = component->data;

    free(system_data);

    return EXIT_SUCCESS;
}
