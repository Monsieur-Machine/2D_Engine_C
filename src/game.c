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

    /* ======== Player ======== */
    entity_t *player = entity_manager_new_entity(&game.entity_manager, 0);
    if(!player) return EXIT_FAILURE;

    component_t *component = entity_new_component(player, TRANSFORM_COMPONENT);
    if(!component) return EXIT_FAILURE;

    ((transform_component_data_t *)component->data)->bounds = (frect_t) {
        .x = 10.0f,
        .y = 10.0f,
        .w = 32.0f,
        .h = 32.0f
    };

    component = entity_new_component(player, HITBOX_COMPONENT);
    if(!component) return EXIT_FAILURE;

    ((hitbox_component_data_t *)component->data)->position = (fvector2d_t) {
        .x = 0.5f,
        .y = 1.0f
    };

    ((hitbox_component_data_t *)component->data)->scale = (fvector2d_t) {
        .x = 0.5f,
        .y = 0.5f
    };

    if(!entity_new_component(player, PHYSICS_SYSTEM))   return EXIT_FAILURE;
    if(!entity_new_component(player, SPRITE_COMPONENT)) return EXIT_FAILURE;
    if(!entity_new_component(player, ANIMATION_SYSTEM)) return EXIT_FAILURE;
    if(!entity_new_component(player, PLAYER_SYSTEM))    return EXIT_FAILURE;

    /* ======== Test entity ======== */
    entity_t *entity = entity_manager_new_entity(&game.entity_manager, 1);
    if(!entity) return EXIT_FAILURE;

    component = entity_new_component(entity, TRANSFORM_COMPONENT);
    if(!component) return EXIT_FAILURE;

    ((transform_component_data_t *)component->data)->bounds = (frect_t) {
        .x = 120.0f,
        .y = 30.0f,
        .w = 32.0f,
        .h = 32.0f
    };

    component = entity_new_component(entity, HITBOX_COMPONENT);
    if(!component) return EXIT_FAILURE;

    ((hitbox_component_data_t *)component->data)->position = (fvector2d_t) {
        .x = 0.5f,
        .y = 0.5f
    };

    ((hitbox_component_data_t *)component->data)->scale = (fvector2d_t) {
        .x = 2.0f / 3.0f,
        .y = 2.0f / 3.0f
    };

    if(!entity_new_component(entity, SPRITE_COMPONENT)) return EXIT_FAILURE;

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
    entity_manager_exit(&game.entity_manager);

    if(asset_manager_exit(&game.asset_manager)) return EXIT_FAILURE;

    display_exit();

    return EXIT_SUCCESS;
}
