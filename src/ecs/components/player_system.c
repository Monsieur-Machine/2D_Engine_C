#include "memory_alloc.h"
#include "player_system.h"
#include "animation_system.h"
#include "sprite_component.h"
#include "transform_component.h"
#include "game.h"

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

    system_data->state &= 0xF0; // 11110000

    system_data->state |= game.events.keys[KEY_UP] * PLAYER_RUNNING_UP;
    system_data->state |= game.events.keys[KEY_DOWN] * PLAYER_RUNNING_DOWN;
    system_data->state |= game.events.keys[KEY_LEFT] * PLAYER_RUNNING_LEFT;
    system_data->state |= game.events.keys[KEY_RIGHT] * PLAYER_RUNNING_RIGHT;

    // Special cases
    if(system_data->state & PLAYER_RUNNING_UP && system_data->state & PLAYER_RUNNING_DOWN)    system_data->state &= 0xFC; // 11111100
    if(system_data->state & PLAYER_RUNNING_LEFT && system_data->state & PLAYER_RUNNING_RIGHT) system_data->state &= 0xF3; // 11110011
}

// Only for this file
static inline void player_system_set_velocity(player_system_data_t *system_data)
{
    transform_component_data_t *transform_component_data = system_data->transform_component_data;

    transform_component_data->velocity.x =
        (float)((system_data->state & PLAYER_RUNNING_RIGHT) >> 3) - (float)((system_data->state & PLAYER_RUNNING_LEFT) >> 2);
    transform_component_data->velocity.y =
        (float)((system_data->state & PLAYER_RUNNING_DOWN) >> 1) - (float)(system_data->state & PLAYER_RUNNING_UP);

    if(f_is_not_zero(transform_component_data->velocity.x) && f_is_not_zero(transform_component_data->velocity.y))
    {
        transform_component_data->velocity.x *= 0.707106781186548f;
        transform_component_data->velocity.y *= 0.707106781186548f;
    }
}

// Only for this file
static inline void player_system_set_animation(player_system_data_t *system_data)
{
    sprite_component_data_t *sprite_component_data = system_data->sprite_component_data;
    animation_system_data_t *animation_system_data = system_data->animation_system_data;

    if(system_data->state & PLAYER_RUNNING_LEFT) sprite_component_data->flip = true;
    if(system_data->state & PLAYER_RUNNING_RIGHT) sprite_component_data->flip = false;

    if((system_data->state & PLAYER_RUNNING) && !(system_data->last_state & PLAYER_RUNNING))
    {
        animation_system_data->frame_delay_ms = PLAYER_DEFAULT_RUN_ANIMATION_SPEED;
        animation_system_change_animation(animation_system_data, "player_run_sheet", 6);
    }
    else if(!(system_data->state & PLAYER_RUNNING) && (system_data->last_state & PLAYER_RUNNING))
    {
        animation_system_data->actual_frame_nb = 0;
        animation_system_data->frame_delay_ms = PLAYER_DEFAULT_IDLE_ANIMATION_SPEED;
        animation_system_change_animation(animation_system_data, "player_idle_sheet", 4);
    }
}

void player_system_update(component_t *component)
{
    player_system_data_t *system_data = component->component_data;

    player_system_get_inputs(system_data);
    player_system_set_velocity(system_data);
    player_system_set_animation(system_data);
}

void player_system_destroy(component_t *component)
{
    free(component->component_data);
}
