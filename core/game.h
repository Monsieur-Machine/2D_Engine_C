#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include "display.h"
#include "asset_manager.h"
#include "camera.h"
#include "event_bus.h"
#include "ecs.h"
#include "map.h"
#include "inputs.h"

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
    display_t display;
    camera_t camera;
    asset_manager_t asset_manager;
    event_bus_t event_bus;
    entity_manager_t entity_manager;
    inputs_t inputs;
    map_t *map;

    float delta_time_ms;
    bool is_running;
} game_t;

extern game_t game;

int game_init(void);
int game_exit(void);

void game_handle_event(void);
int game_update(void);
int game_render(void);

#endif // GAME_H