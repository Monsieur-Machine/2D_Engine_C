#ifndef GAME_H
#define GAME_H

#include <keycodes.h>
#include "texture_manager.h"
#include "ecs.h"

/**
 * @brief Game data.
 * There should be only one instance of this struct.
 *
 * @param event             Events data
 * @param texture_manager   Texture manager instance
 * @param entity_manager    Entity manager instance
 * @param is_running        Game running state
 * @param delta_time_ms     Frame time
*/
typedef struct game_t {
    bool keys[MAX_KEYCODES];
    texture_manager_t texture_manager;
    entity_manager_t entity_manager;

    bool is_running;
    double delta_time_ms;
} game_t;

/**
 * @brief Global game instance
*/
extern game_t game;

/**
 * @brief Initialise game
*/
void game_init(void);

/**
 * @brief Handle event like inputs
*/
void game_handle_event(void);

/**
 * @brief Update game
*/
void game_update(void);

/**
 * @brief Render game
*/
void game_render(void);

/**
 * @brief Free game instance
*/
void game_exit(void);

#endif // GAME_H