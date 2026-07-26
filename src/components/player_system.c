#include "player_system.h"

#include "game.h"
#include "memory_alloc.h"
#include "animation_system.h"
#include "sprite_component.h"
#include "transform_component.h"

// No args
void player_system_init(component_t *component, va_list args __attribute__((unused)))
{
    player_system_data_t *system_data = component->component_data;

    system_data->animation_system_data = get_component(component->entity, ANIMATION_SYSTEM)->component_data;
    system_data->sprite_component_data = system_data->animation_system_data->sprite_component_data;
    system_data->transform_component_data = system_data->sprite_component_data->transform_component_data;

    system_data->state = PLAYER_IDLE;
}

// Only for this file
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

// Only for this file
static inline void player_system_set_velocity(player_system_data_t *system_data)
{
    transform_component_data_t *transform_component_data = system_data->transform_component_data;

    if((system_data->state & PLAYER_MOVING) != (system_data->last_state & PLAYER_MOVING))
    {
        transform_component_data->velocity.x =
            (float)((system_data->state & PLAYER_MOVING_RIGHT) >> 3) - (float)((system_data->state & PLAYER_MOVING_LEFT) >> 2);
        transform_component_data->velocity.y =
            (float)((system_data->state & PLAYER_MOVING_DOWN) >> 1) - (float)(system_data->state & PLAYER_MOVING_UP);

        if(f_is_not_zero(transform_component_data->velocity.x) && f_is_not_zero(transform_component_data->velocity.y))
        {
            transform_component_data->velocity.x *= 0.707106781186548f;
            transform_component_data->velocity.y *= 0.707106781186548f;
        }
    }
}

// Only for this file
static inline void player_system_set_animation_and_speed(player_system_data_t *system_data)
{
    animation_system_data_t *animation_system_data = system_data->animation_system_data;
    sprite_component_data_t *sprite_component_data = system_data->sprite_component_data;
    transform_component_data_t *transform_component_data = system_data->transform_component_data;

    if(system_data->state & PLAYER_MOVING_LEFT) sprite_component_data->flip = true;
    if(system_data->state & PLAYER_MOVING_RIGHT) sprite_component_data->flip = false;

    if(((system_data->state & PLAYER_MOVING) && !(system_data->last_state & PLAYER_MOVING))
        || ((system_data->state & PLAYER_MOVING) && (system_data->state & PLAYER_RUNNING) != (system_data->last_state & PLAYER_RUNNING)))
    {
        if(system_data->state & PLAYER_RUNNING)
        {
            transform_component_data->speed = PLAYER_DEFAULT_RUN_SPEED;
            animation_system_data->frame_delay_ms = PLAYER_DEFAULT_RUN_ANIMATION_SPEED;
            animation_system_data->actual_frame_nb = 0;
            animation_system_change_animation(animation_system_data, "player_run_sheet", 8);
        }
        else
        {
            transform_component_data->speed = PLAYER_DEFAULT_WALK_SPEED;
            animation_system_data->frame_delay_ms = PLAYER_DEFAULT_WALK_ANIMATION_SPEED;
            animation_system_data->actual_frame_nb = 0;
            animation_system_change_animation(animation_system_data, "player_walk_sheet", 4);
        }
    }
    else if(!(system_data->state & PLAYER_MOVING) && (system_data->last_state & PLAYER_MOVING))
    {
        animation_system_data->actual_frame_nb = 0;
        animation_system_data->frame_delay_ms = PLAYER_DEFAULT_IDLE_ANIMATION_SPEED;
        animation_system_data->actual_frame_nb = 0;
        animation_system_change_animation(animation_system_data, "player_idle_sheet", 4);
    }
}

void player_system_update(component_t *component)
{
    player_system_data_t *system_data = component->component_data;

    player_system_get_inputs(system_data);
    player_system_set_velocity(system_data);
    player_system_set_animation_and_speed(system_data);
}

void player_system_destroy(component_t *component)
{
    free(component->component_data);
}
