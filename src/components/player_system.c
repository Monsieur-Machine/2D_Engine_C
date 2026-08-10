#include "player_system.h"

#include "game.h"
#include "sprite_component.h"
#include "animation_system.h"
#include "interact_component.h"
#include "physics_system.h"
#include "memory_alloc.h"
#include "errors.h"

inline int player_system_init(component_t *component)
{
    player_system_data_t *system_data = component->data;

    component_t *sprite_component = entity_get_component(component->entity, SPRITE_COMPONENT);
    if(!sprite_component)
    {
        error_printf("Player system require a sprite component.");
        return EXIT_FAILURE;
    }
    system_data->sprite_component_data = sprite_component->data;

    component_t *animation_system = entity_get_component(component->entity, ANIMATION_SYSTEM);
    if(!animation_system)
    {
        error_printf("Player system require an animation system.");
        return EXIT_FAILURE;
    }
    system_data->animation_system_data = animation_system->data;

    component_t *physics_system = entity_get_component(component->entity, PHYSICS_SYSTEM);
    if(!physics_system)
    {
        error_printf("Player system require a physics system.");
        return EXIT_FAILURE;
    }
    system_data->physics_system_data = physics_system->data;

    system_data->state = PLAYER_MOVING;

    animation_system_data_t *animation_system_data = system_data->animation_system_data;

    animation_system_data->loop = true;
    animation_system_data->play = true;

    return EXIT_SUCCESS;
}

static void player_system_get_inputs(player_system_data_t *system_data);
static void player_system_set_velocity(player_system_data_t *system_data);
static int player_system_set_animation_and_speed(player_system_data_t *system_data);
static int player_system_process_interaction(player_system_data_t *system_data);

inline int player_system_update(component_t *component)
{
    player_system_data_t *system_data = component->data;

    player_system_get_inputs(system_data);
    player_system_set_velocity(system_data);
    if(player_system_set_animation_and_speed(system_data)) return EXIT_FAILURE;
    if(player_system_process_interaction(system_data)) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

static inline void player_system_get_inputs(player_system_data_t *system_data)
{
    system_data->last_state = system_data->state;

    system_data->state &= 0xE0; // 11100000

    system_data->state |= game.events.keys[KEY_UP]    ? PLAYER_MOVING_UP    : 0;
    system_data->state |= game.events.keys[KEY_DOWN]  ? PLAYER_MOVING_DOWN  : 0;
    system_data->state |= game.events.keys[KEY_LEFT]  ? PLAYER_MOVING_LEFT  : 0;
    system_data->state |= game.events.keys[KEY_RIGHT] ? PLAYER_MOVING_RIGHT : 0;
    system_data->state |= game.events.keys[KEY_MOD]   ? PLAYER_RUNNING      : 0;

    // Special cases
    if(system_data->state & PLAYER_MOVING_UP && system_data->state & PLAYER_MOVING_DOWN)
    {
        if(system_data->last_state & PLAYER_MOVING_UP)
            system_data->state &= ~PLAYER_MOVING_DOWN;
        else
            system_data->state &= ~PLAYER_MOVING_UP;
    }

    if(system_data->state & PLAYER_MOVING_LEFT && system_data->state & PLAYER_MOVING_RIGHT)
    {
        if(system_data->last_state & PLAYER_MOVING_LEFT)
            system_data->state &= ~PLAYER_MOVING_RIGHT;
        else
            system_data->state &= ~PLAYER_MOVING_LEFT;
    }
}

static inline void player_system_set_velocity(player_system_data_t *system_data)
{
    physics_system_data_t *physics_system_data = system_data->physics_system_data;

    if((system_data->state & PLAYER_MOVING) != (system_data->last_state & PLAYER_MOVING))
    {
        physics_system_data->target_velocity.x =
            (float)((system_data->state & PLAYER_MOVING_RIGHT) >> 3) - (float)((system_data->state & PLAYER_MOVING_LEFT) >> 2);
        physics_system_data->target_velocity.y =
            (float)((system_data->state & PLAYER_MOVING_DOWN) >> 1) - (float)(system_data->state & PLAYER_MOVING_UP);

        if(f_is_not_zero(physics_system_data->target_velocity.x) && f_is_not_zero(physics_system_data->target_velocity.y))
        {
            physics_system_data->target_velocity.x *= 0.707106781186548f;
            physics_system_data->target_velocity.y *= 0.707106781186548f;
        }
    }
}

static inline int player_system_set_animation_and_speed(player_system_data_t *system_data)
{
    animation_system_data_t *animation_system_data = system_data->animation_system_data;
    sprite_component_data_t *sprite_component_data = system_data->sprite_component_data;
    physics_system_data_t *physics_system_data = system_data->physics_system_data;

    if(system_data->state & PLAYER_MOVING_LEFT) sprite_component_data->flip = true;
    if(system_data->state & PLAYER_MOVING_RIGHT) sprite_component_data->flip = false;

    if(((system_data->state & PLAYER_MOVING) && !(system_data->last_state & PLAYER_MOVING))
        || ((system_data->state & PLAYER_MOVING) && (system_data->state & PLAYER_RUNNING) != (system_data->last_state & PLAYER_RUNNING)))
    {
        if(system_data->state & PLAYER_RUNNING)
        {
            physics_system_data->speed = PLAYER_DEFAULT_RUN_SPEED;

            if(sprite_component_set_texture(sprite_component_data, "player_run_sheet")) return EXIT_FAILURE;

            animation_system_data->nb_frames = 8;
            animation_system_data->current_frame_nb = 0;
            animation_system_data->frame_delay_ms = PLAYER_DEFAULT_RUN_ANIMATION_SPEED;
            if(animation_system_apply_changes(animation_system_data)) return EXIT_FAILURE;
        }
        else
        {
            physics_system_data->speed = PLAYER_DEFAULT_WALK_SPEED;

            if(sprite_component_set_texture(sprite_component_data, "player_walk_sheet")) return EXIT_FAILURE;

            animation_system_data->nb_frames = 4;
            animation_system_data->current_frame_nb = 0;
            animation_system_data->frame_delay_ms = PLAYER_DEFAULT_WALK_ANIMATION_SPEED;
            if(animation_system_apply_changes(animation_system_data)) return EXIT_FAILURE;
        }
    }
    else if(!(system_data->state & PLAYER_MOVING) && (system_data->last_state & PLAYER_MOVING))
    {
        if(sprite_component_set_texture(sprite_component_data, "player_idle_sheet")) return EXIT_FAILURE;

        animation_system_data->nb_frames = 4;
        animation_system_data->current_frame_nb = 0;
        animation_system_data->frame_delay_ms = PLAYER_DEFAULT_IDLE_ANIMATION_SPEED;
        if(animation_system_apply_changes(animation_system_data)) return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

// Args : player_system_data_t *system_data
static inline int action_detect_activate_interact(elem_t *elem, va_list args)
{
    const entity_t *target_entity = elem->data;
    const player_system_data_t *system_data = va_arg(args, player_system_data_t *);

    const component_t *target_interact_component = entity_get_component(target_entity, INTERACT_COMPONENT);
    if(!target_interact_component) return EXIT_SUCCESS;

    interact_component_data_t *target_interact_component_data = target_interact_component->data;

    if(!target_interact_component_data->activated)
    {
        return EXIT_SUCCESS;
    }

    const frect_t *hitbox_bounds = &system_data->physics_system_data->hitbox_component_data->bounds;
    const frect_t *target_interact_bounds = &target_interact_component_data->bounds;

    if(fhas_intersection(hitbox_bounds, target_interact_bounds))
    {
        if(interact_component_interact(target_interact_component_data)) return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

static inline int player_system_process_interaction(player_system_data_t *system_data)
{
    static bool last_state = false; // XXX UGLY XXX //

    if(game.events.keys[KEY_INTERACT] && !last_state)
    {
        if(linked_list_for_each(&game.entity_manager.entities, action_detect_activate_interact, system_data)) return EXIT_FAILURE;
    }

    last_state = game.events.keys[KEY_INTERACT];

    return EXIT_SUCCESS;
}

inline int player_system_destroy(component_t *component)
{
    player_system_data_t *component_data = component->data;

    free(component_data);

    return EXIT_SUCCESS;
}
