#ifndef GAME_H
#define GAME_H

#include "events.h"
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
    events_t events;
    texture_manager_t texture_manager;
    entity_manager_t entity_manager;

    bool is_running;
    double delta_time_ms;
} game_t;

// Global game instance
extern game_t game;

// Initialise game
void game_init(void);

// Handle event like inputs
void game_handle_event(void);

// Update game
void game_update(void);

// Render game
void game_render(void);

// Free game instance
void game_exit(void);

#endif // GAME_H