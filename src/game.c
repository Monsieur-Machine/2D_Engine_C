#include "game.h"

#include <stdlib.h>
#include "display.h"
#include "asset_manager.h"
#include "ecs.h"
#include "event_bus.h"
#include "inputs.h"
#include "components.h"
#include "errors.h"

int game_init(void)
{
    game.is_running = false;

    if(display_init()) return EXIT_FAILURE;
    if(asset_manager_init(&game.asset_manager)) return EXIT_FAILURE;
    event_bus_init(&game.event_bus);
    entity_manager_init(&game.entity_manager);

    if(asset_manager_load_asset(&game.asset_manager, "dummy_sheet",       ASSET_TEXTURE)) return EXIT_FAILURE;
    if(asset_manager_load_asset(&game.asset_manager, "player_idle_sheet", ASSET_TEXTURE)) return EXIT_FAILURE;
    if(asset_manager_load_asset(&game.asset_manager, "player_walk_sheet", ASSET_TEXTURE)) return EXIT_FAILURE;
    if(asset_manager_load_asset(&game.asset_manager, "player_run_sheet",  ASSET_TEXTURE)) return EXIT_FAILURE;
    if(asset_manager_load_asset(&game.asset_manager, "lever_sheet",       ASSET_TEXTURE)) return EXIT_FAILURE;
    if(asset_manager_load_asset(&game.asset_manager, "barred_door_sheet", ASSET_TEXTURE)) return EXIT_FAILURE;
    if(asset_manager_load_asset(&game.asset_manager, "wooden_door_sheet", ASSET_TEXTURE)) return EXIT_FAILURE;
    if(asset_manager_load_asset(&game.asset_manager, "tileset",           ASSET_TEXTURE)) return EXIT_FAILURE;

    /* ======== Player ======== */
    entity_t *player_entity = entity_manager_new_entity(&game.entity_manager, 0);
    if(!player_entity) return EXIT_FAILURE;

    component_t *component = entity_new_component(player_entity, TRANSFORM_COMPONENT);
    if(!component) return EXIT_FAILURE;

    ((transform_component_data_t *)component->data)->bounds = (frect_t) {
        .x = 10.0f,
        .y = 10.0f,
        .w = 32.0f,
        .h = 32.0f
    };

    if(!entity_new_component(player_entity, SPRITE_COMPONENT)) return EXIT_FAILURE;

    component = entity_new_component(player_entity, ANIMATION_SYSTEM);
    if(!component) return EXIT_FAILURE;

    ((animation_system_data_t *)component->data)->loop = true;
    ((animation_system_data_t *)component->data)->play = true;

    component = entity_new_component(player_entity, HITBOX_COMPONENT);
    if(!component) return EXIT_FAILURE;

    ((hitbox_component_data_t *)component->data)->position = (fvector2d_t) {
        .x = 0.5f,
        .y = 1.0f
    };

    ((hitbox_component_data_t *)component->data)->scale = (fvector2d_t) {
        .x = 0.5f,
        .y = 1.0f / 3.0f
    };

    if(!entity_new_component(player_entity, PHYSICS_SYSTEM))   return EXIT_FAILURE;

    if(!entity_new_component(player_entity, PLAYER_SYSTEM))    return EXIT_FAILURE;

    /* ======== Lever ========*/
    entity_t *lever_entity = entity_manager_new_entity(&game.entity_manager, 1);
    if(!lever_entity) return EXIT_FAILURE;

    component = entity_new_component(lever_entity, TRANSFORM_COMPONENT);
    if(!component) return EXIT_FAILURE;

    ((transform_component_data_t *)component->data)->bounds = (frect_t) {
        .x = 30.0f,
        .y = 120.0f,
        .w = 48.0f,
        .h = 32.0f
    };

    component = entity_new_component(lever_entity, SPRITE_COMPONENT);
    if(!component) return EXIT_FAILURE;

    if(sprite_component_set_texture(component->data, "lever_sheet")) return EXIT_FAILURE;

    component = entity_new_component(lever_entity, ANIMATION_SYSTEM);
    if(!component) return EXIT_FAILURE;

    ((animation_system_data_t *)component->data)->nb_frames = 3;
    ((animation_system_data_t *)component->data)->frame_delay_ms = LEVER_DEFAULT_ANIMATION_SPEED;
    if(animation_system_create_frames_clips(component->data)) return EXIT_FAILURE;

    component = entity_new_component(lever_entity, HITBOX_COMPONENT);
    if(!component) return EXIT_FAILURE;

    ((hitbox_component_data_t *)component->data)->position = (fvector2d_t) {
        .x = 0.5f,
        .y = 0.9f
    };

    ((hitbox_component_data_t *)component->data)->scale = (fvector2d_t) {
        .x = 1.0f / 3.0f,
        .y = 0.3f
    };

    component = entity_new_component(lever_entity, INTERACT_COMPONENT);
    if(!component) return EXIT_FAILURE;

    ((interact_component_data_t *)component->data)->position = (fvector2d_t) {
        .x = 0.5f,
        .y = 1.9f
    };

    ((interact_component_data_t *)component->data)->scale = (fvector2d_t) {
        .x = 2.0f / 3.0f,
        .y = 0.8f
    };

    if(!entity_new_component(lever_entity, LEVER_COMPONENT)) return EXIT_FAILURE;

    /* ======== Door ======== */
    entity_t *door_entity = entity_manager_new_entity(&game.entity_manager, 1);
    if(!lever_entity) return EXIT_FAILURE;

    component = entity_new_component(door_entity, TRANSFORM_COMPONENT);
    if(!component) return EXIT_FAILURE;

    ((transform_component_data_t *)component->data)->bounds = (frect_t) {
        .x = 120.0f,
        .y = 30.0f,
        .w = 32.0f,
        .h = 48.0f
    };

    component = entity_new_component(door_entity, SPRITE_COMPONENT);
    if(!component) return EXIT_FAILURE;

    if(sprite_component_set_texture(component->data, "barred_door_sheet")) return EXIT_FAILURE;

    component = entity_new_component(door_entity, ANIMATION_SYSTEM);
    if(!component) return EXIT_FAILURE;

    ((animation_system_data_t *)component->data)->nb_frames = 8;
    ((animation_system_data_t *)component->data)->frame_delay_ms = BARRED_DOOR_DEFAULT_ANIMATION_SPEED;
    if(animation_system_create_frames_clips(component->data)) return EXIT_FAILURE;

    component = entity_new_component(door_entity, HITBOX_COMPONENT);
    if(!component) return EXIT_FAILURE;

    ((hitbox_component_data_t *)component->data)->position = (fvector2d_t) {
        .x = 0.5f,
        .y = 1.0f
    };

    ((hitbox_component_data_t *)component->data)->scale = (fvector2d_t) {
        .x = 1.0f,
        .y = 1.0f / 7.0f
    };

    if(!entity_new_component(door_entity, DOOR_COMPONENT)) return EXIT_FAILURE;

    /* ======== Door 2 ======== */
    entity_t *door2_entity = entity_manager_new_entity(&game.entity_manager, 1);
    if(!lever_entity) return EXIT_FAILURE;

    component = entity_new_component(door2_entity, TRANSFORM_COMPONENT);
    if(!component) return EXIT_FAILURE;

    ((transform_component_data_t *)component->data)->bounds = (frect_t) {
        .x = 240.0f,
        .y = 30.0f,
        .w = 32.0f,
        .h = 48.0f
    };

    component = entity_new_component(door2_entity, SPRITE_COMPONENT);
    if(!component) return EXIT_FAILURE;

    if(sprite_component_set_texture(component->data, "wooden_door_sheet")) return EXIT_FAILURE;

    component = entity_new_component(door2_entity, ANIMATION_SYSTEM);
    if(!component) return EXIT_FAILURE;

    ((animation_system_data_t *)component->data)->nb_frames = 6;
    ((animation_system_data_t *)component->data)->frame_delay_ms = WOODEN_DOOR_DEFAULT_ANIMATION_SPEED;
    if(animation_system_create_frames_clips(component->data)) return EXIT_FAILURE;

    component = entity_new_component(door2_entity, HITBOX_COMPONENT);
    if(!component) return EXIT_FAILURE;

    ((hitbox_component_data_t *)component->data)->position = (fvector2d_t) {
        .x = 0.5f,
        .y = 1.0f
    };

    ((hitbox_component_data_t *)component->data)->scale = (fvector2d_t) {
        .x = 1.0f,
        .y = 1.0f / 7.0f
    };

    if(!entity_new_component(door2_entity, DOOR_COMPONENT)) return EXIT_FAILURE;

    game.is_running = true;

    return EXIT_SUCCESS;
}

void game_handle_event(void)
{
    input_event_t event;

    while(poll_event(&event))
    {
        if(event.type == INPUT_EVENT_KEY_DOWN)
            game.events.keys[event.key] = true;
        else if(event.type == INPUT_EVENT_KEY_UP)
            game.events.keys[event.key] = false;

        if(event.type == INPUT_EVENT_QUIT)
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
    event_bus_exit(&game.event_bus);
    if(asset_manager_exit(&game.asset_manager)) return EXIT_FAILURE;
    display_exit();

    return EXIT_SUCCESS;
}
