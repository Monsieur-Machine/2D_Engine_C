#include "game.h"

#include <stdlib.h>
#include "display.h"
#include "asset_manager.h"
#include "ecs.h"
#include "components.h"
#include "errors.h"

int game_init(void)
{
    game.is_running = false;

    if(display_init()) return EXIT_FAILURE;

    if(asset_manager_init(&game.asset_manager)) return EXIT_FAILURE;

    if(asset_manager_load_asset(&game.asset_manager, "dummy_sheet",       ASSET_TEXTURE)) return EXIT_FAILURE;
    if(asset_manager_load_asset(&game.asset_manager, "player_idle_sheet", ASSET_TEXTURE)) return EXIT_FAILURE;
    if(asset_manager_load_asset(&game.asset_manager, "player_walk_sheet", ASSET_TEXTURE)) return EXIT_FAILURE;
    if(asset_manager_load_asset(&game.asset_manager, "player_run_sheet",  ASSET_TEXTURE)) return EXIT_FAILURE;

    entity_manager_init(&game.entity_manager);

    entity_t *player = entity_manager_new_entity(&game.entity_manager, 0);
    if(!player) return EXIT_FAILURE;

    if(!entity_new_component(player, TRANSFORM_COMPONENT)) return EXIT_FAILURE;
    if(!entity_new_component(player, SPRITE_COMPONENT))    return EXIT_FAILURE;
    if(!entity_new_component(player, ANIMATION_SYSTEM))    return EXIT_FAILURE;
    if(!entity_new_component(player, PLAYER_SYSTEM))       return EXIT_FAILURE;

    game.is_running = true;

    return EXIT_SUCCESS;
}

void game_handle_event(void)
{
    event_t event;

    while(pollevent(&event))
    {
        if(event.type == EVENT_KEY_DOWN)
            game.events.keys[event.key] = true;
        else if(event.type == EVENT_KEY_UP)
            game.events.keys[event.key] = false;

        if(event.type == EVENT_QUIT)
            game.is_running = false;
    }
}

int game_update(void)
{
    if(entity_manager_update(&game.entity_manager))
    {
        game.is_running = false;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int game_render(void)
{
    if(display_clear())
    {
        game.is_running = false;
        return EXIT_FAILURE;
    }

    if(entity_manager_draw(&game.entity_manager))
    {
        game.is_running = false;
        return EXIT_FAILURE;
    }

    display_update();

    return EXIT_SUCCESS;
}

int game_exit(void)
{
    if(asset_manager_let_go_asset(&game.asset_manager, "player_idle_sheet")) return EXIT_FAILURE;
    if(asset_manager_let_go_asset(&game.asset_manager, "player_walk_sheet")) return EXIT_FAILURE;
    if(asset_manager_let_go_asset(&game.asset_manager, "player_run_sheet")) return EXIT_FAILURE;

    entity_manager_exit(&game.entity_manager);
    if(asset_manager_exit(&game.asset_manager)) return EXIT_FAILURE;

    display_exit();

    return EXIT_SUCCESS;
}
