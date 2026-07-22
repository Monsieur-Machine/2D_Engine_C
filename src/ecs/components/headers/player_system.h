#ifndef PLAYER_SYSTEM_H
#define PLAYER_SYSTEM_H

#include "ecs.h"
#include "transform_component.h"
#include "sprite_component.h"
#include "animation_system.h"

#define PLAYER_DEFAULT_SPEED 80.0f // px / s
#define PLAYER_DEFAULT_IDLE_ANIMATION_SPEED 250.0f // ms / frame
#define PLAYER_DEFAULT_WALK_ANIMATION_SPEED  160.0f // ms / frame

typedef enum player_state_t {
    PLAYER_IDLE          = 0x00, // 00000000
    PLAYER_RUNNING_UP    = 0x01, // 00000001
    PLAYER_RUNNING_DOWN  = 0x02, // 00000010
    PLAYER_RUNNING_LEFT  = 0x04, // 00000100
    PLAYER_RUNNING_RIGHT = 0x08, // 00001000
    PLAYER_RUNNING       = 0x0F, // 00001111
    PLAYER_DEATH         = 0x80  // 10000000
} player_state_t;

typedef struct player_system_data_t {
    transform_component_data_t *transform_component_data;
    sprite_component_data_t *sprite_component_data;
    animation_system_data_t *animation_system_data;

    player_state_t state;
    player_state_t last_state;
} player_system_data_t;

void player_system_init(component_t *, va_list);

void player_system_update(component_t *);

void player_system_destroy(component_t *);

#endif // PLAYER_SYSTEM_H