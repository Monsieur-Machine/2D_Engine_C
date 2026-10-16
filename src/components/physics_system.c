#include "physics_system.h"

#include "game.h"
#include "ecs.h"
#include "transform_component.h"
#include "hitbox_component.h"
#include "vector2d.h"
#include "memory_alloc.h"
#include "errors.h"

inline int physics_system_init(component_t *component)
{
    physics_system_data_t *system_data = component->data;

    system_data->transform_component = entity_get_component(component->entity, TRANSFORM_COMPONENT);
    if(!system_data->transform_component)
    {
        error_printf("Physics system require a transform component.");
        return EXIT_FAILURE;
    }

    system_data->hitbox_component = entity_get_component(component->entity, HITBOX_COMPONENT);
    if(!system_data->hitbox_component)
    {
        error_printf("Physics system require an hitbox component.");
        return EXIT_FAILURE;
    }

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

// Args : component_t *component
static int action_detect_solve_entities_collision(elem_t *elem, void *arg);

int physics_system_update(component_t *component)
{
    physics_system_data_t *system_data = component->data;
    transform_component_data_t *transform_component_data = system_data->transform_component->data;
    const hitbox_component_data_t *hitbox_component_data = system_data->hitbox_component->data;

    system_data->velocity.x = lerp_time(system_data->velocity.x, system_data->target_velocity.x * system_data->speed, system_data->smoothing_factor, game.delta_time_ms);
    system_data->velocity.y = lerp_time(system_data->velocity.y, system_data->target_velocity.y * system_data->speed, system_data->smoothing_factor, game.delta_time_ms);

    if(hitbox_component_data->activated)
    {
        // Resolve entities collisions.
        if(linked_list_for_each(&game.entity_manager.entities, action_detect_solve_entities_collision, component)) return EXIT_FAILURE;

        // Resolve map collisions
        if(game.map)
        {
            uint16_t left_tile = 0;
            if(game.camera.view_rect.x / TILE_SIZE > 0)
                left_tile = hitbox_component_data->bounds.x / TILE_SIZE;

            uint16_t right_tile = game.map->width;
            if((hitbox_component_data->bounds.x + hitbox_component_data->bounds.w) / TILE_SIZE + 1 < game.map->width)
                right_tile = (hitbox_component_data->bounds.x + hitbox_component_data->bounds.w) / TILE_SIZE + 1;

            uint16_t top_tile = 0;
            if(hitbox_component_data->bounds.y / TILE_SIZE > 0)
                top_tile = hitbox_component_data->bounds.y / TILE_SIZE;

            uint16_t bottom_tile = game.map->height;
            if((hitbox_component_data->bounds.y + hitbox_component_data->bounds.h) / TILE_SIZE + 1 < game.map->height)
                bottom_tile = (hitbox_component_data->bounds.y + hitbox_component_data->bounds.h) / TILE_SIZE + 1;

            for(uint16_t tile_y = top_tile; tile_y < bottom_tile; tile_y++)
            for(uint16_t tile_x = left_tile; tile_x < right_tile; tile_x++)
            {
                if(game.map->hitboxes[tile_y * game.map->width + tile_x] == 1)
                {
                    const frect_t *hitbox_bounds = &hitbox_component_data->bounds;
                    const frect_t target_hitbox_bounds = {
                        .x = tile_x * TILE_SIZE,
                        .y = tile_y * TILE_SIZE,
                        .w = TILE_SIZE,
                        .h = TILE_SIZE
                    };
                    frect_t intersect;

                    if(intersect_frect(hitbox_bounds, &target_hitbox_bounds, &intersect))
                    {
                        if(intersect.w < intersect.h)
                        {
                            const float left_correction = hitbox_bounds->x + hitbox_bounds->w - target_hitbox_bounds.x;
                            const float right_correction = target_hitbox_bounds.x + target_hitbox_bounds.w - hitbox_bounds->x;

                            if(left_correction < right_correction)
                            {
                                transform_component_data->bounds.x -= left_correction;
                                if(system_data->velocity.x > 0.0f) system_data->velocity.x = 0.0f;
                            }
                            else
                            {
                                transform_component_data->bounds.x += right_correction;
                                if(system_data->velocity.x < 0.0f) system_data->velocity.x = 0.0f;
                            }
                        }
                        else
                        {
                            float top_correction = hitbox_bounds->y + hitbox_bounds->h - target_hitbox_bounds.y;
                            float bottom_correction = target_hitbox_bounds.y + target_hitbox_bounds.h - hitbox_bounds->y;

                            if(top_correction < bottom_correction)
                            {
                                transform_component_data->bounds.y -= top_correction;
                                if(system_data->velocity.y > 0.0f) system_data->velocity.y = 0.0f;
                            }
                            else
                            {
                                transform_component_data->bounds.y += bottom_correction;
                                if(system_data->velocity.y < 0.0f) system_data->velocity.y = 0.0f;
                            }
                        }
                    }
                }
            }
        }
    }

    transform_component_data->bounds.x += system_data->velocity.x * (float)game.delta_time_ms / 1000.0f;
    transform_component_data->bounds.y += system_data->velocity.y * (float)game.delta_time_ms / 1000.0f;

    return EXIT_SUCCESS;
}

inline int physics_system_destroy(component_t *component)
{
    physics_system_data_t *system_data = component->data;

    free(system_data);

    return EXIT_SUCCESS;
}

static inline int action_detect_solve_entities_collision(elem_t *elem, void *arg)
{
    component_t *physics_system = arg;

    physics_system_data_t *system_data = physics_system->data;
    transform_component_data_t * transform_component_data = system_data->transform_component->data;
    const hitbox_component_data_t *hitbox_component_data = system_data->hitbox_component->data;

    const entity_t *target_entity = elem->data;

    if(target_entity->id == physics_system->entity->id)
    {
        return EXIT_SUCCESS;
    }

    const component_t *target_hitbox_component = entity_get_component(target_entity, HITBOX_COMPONENT);
    if(!target_hitbox_component) return EXIT_SUCCESS;

    const hitbox_component_data_t *target_hitbox_component_data = target_hitbox_component->data;

    if(!target_hitbox_component_data->activated) return EXIT_SUCCESS;

    const frect_t *hitbox_bounds = &hitbox_component_data->bounds;
    const frect_t *target_hitbox_bounds = &target_hitbox_component_data->bounds;
    frect_t intersect;

    if(intersect_frect(hitbox_bounds, target_hitbox_bounds, &intersect))
    {
        if(intersect.w < intersect.h)
        {
            const float left_correction = hitbox_bounds->x + hitbox_bounds->w - target_hitbox_bounds->x;
            const float right_correction = target_hitbox_bounds->x + target_hitbox_bounds->w - hitbox_bounds->x;

            if(left_correction < right_correction)
            {
                transform_component_data->bounds.x -= left_correction;
                if(system_data->velocity.x > 0.0f) system_data->velocity.x = 0.0f;
            }
            else
            {
                transform_component_data->bounds.x += right_correction;
                if(system_data->velocity.x < 0.0f) system_data->velocity.x = 0.0f;
            }
        }
        else
        {
            float top_correction = hitbox_bounds->y + hitbox_bounds->h - target_hitbox_bounds->y;
            float bottom_correction = target_hitbox_bounds->y + target_hitbox_bounds->h - hitbox_bounds->y;

            if(top_correction < bottom_correction)
            {
                transform_component_data->bounds.y -= top_correction;
                if(system_data->velocity.y > 0.0f) system_data->velocity.y = 0.0f;
            }
            else
            {
                transform_component_data->bounds.y += bottom_correction;
                if(system_data->velocity.y < 0.0f) system_data->velocity.y = 0.0f;
            }
        }
    }

    return EXIT_SUCCESS;
}
