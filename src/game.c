#include "game.h"

#include <stdlib.h>
#include "display.h"
#include "camera.h"
#include "asset_manager.h"
#include "event_bus.h"
#include "ecs.h"
#include "components.h"
#include "map.h"
#include "inputs.h"
#include "errors.h"

int game_init(void)
{
    game.is_running = false;

    if(display_init()) return_failure_int;
    camera_init();
    event_bus_init();
    entity_manager_init();
    if(asset_manager_init()) return_failure_int;

    asset_t *map_asset = asset_manager_load_asset("dungeon_room_1", ASSET_MAP);
    if(!map_asset) return_failure_int;

    game.map = map_asset->data;

    asset_t *tileset_asset = asset_manager_load_asset("tileset", ASSET_TEXTURE);
    if(!tileset_asset) return_failure_int;

    game.map->tileset = tileset_asset->data;

    if(map_load_entities(game.map)) return_failure_int;

    /* ======== Player ======== */
    {
        entity_t *player_entity = entity_manager_new_entity(0);
        if(!player_entity) return_failure_int;

        {
            component_t *transform_component = entity_new_component(player_entity, TRANSFORM_COMPONENT);
            if(!transform_component) return_failure_int;

            transform_component_data_t *transform_component_data = transform_component->data;

            transform_component_data->bounds = (frect_t) {
                .x = 96.0f,
                .y = 176.0f,
                .w = 32.0f,
                .h = 32.0f
            };

            game.camera.tracking_pos = (fvector2d_t *)&transform_component_data->bounds;
            game.camera.tracking_center_offset = (fvector2d_t) {
                .x = transform_component_data->bounds.w / 2,
                .y = transform_component_data->bounds.h / 2
            };
        }

        {
            component_t *sprite_component = entity_new_component(player_entity, SPRITE_COMPONENT);
            if(!sprite_component) return_failure_int;

            sprite_component_data_t *sprite_component_data = sprite_component->data;

            asset_t *asset = asset_manager_load_asset("player_idle_sheet", ASSET_TEXTURE);
            if(!asset) return_failure_int;

            sprite_component_data->texture = asset->data;

            if(!asset_manager_load_asset("player_walk_sheet", ASSET_TEXTURE)) return_failure_int;
            if(!asset_manager_load_asset("player_run_sheet",  ASSET_TEXTURE)) return_failure_int;
        }

        {
            component_t *animation_system = entity_new_component(player_entity, ANIMATION_SYSTEM);
            if(!animation_system) return_failure_int;

            animation_system_data_t *animation_system_data = animation_system->data;

            if(linked_list_reserve(&animation_system_data->frames, 8)) return_failure_int;

            animation_system_data->current_frame_nb = 0;
            animation_system_data->nb_frames = 4;
            if(animation_system_apply_changes(animation_system)) return_failure_int;

            animation_system_data->play = true;
            animation_system_data->loop = true;
            animation_system_data->reverse = false;
        }

        {
            component_t *hitbox_component = entity_new_component(player_entity, HITBOX_COMPONENT);
            if(!hitbox_component) return_failure_int;

            hitbox_component_data_t * hitbox_component_data = hitbox_component->data;

            hitbox_component_data->position = (fvector2d_t) {
                .x = 0.25f,
                .y = 0.6875f
            };

            hitbox_component_data->scale = (fvector2d_t) {
                .x = 0.5f,
                .y = 0.3125f
            };
        }

        if(!entity_new_component(player_entity, PHYSICS_SYSTEM)) return_failure_int;

        if(!entity_new_component(player_entity, PLAYER_SYSTEM))  return_failure_int;
    }

    game.is_running = true;

    return EXIT_SUCCESS;
}

void game_handle_event(void)
{
    input_event_t event;

    while(poll_event(&event))
    {
        if(event.type == INPUT_EVENT_KEY_DOWN)
            game.inputs.keys[event.key] = true;
        else if(event.type == INPUT_EVENT_KEY_UP)
            game.inputs.keys[event.key] = false;

        if(event.type == INPUT_EVENT_QUIT)
            game.is_running = false;
    }
}

int game_update(void)
{
    if(entity_manager_update())
    {
        game.is_running = false;
        return_failure_int;
    }

    camera_update();

    return EXIT_SUCCESS;
}

int game_render(void)
{
    if(display_clear())
    {
        game.is_running = false;
        return_failure_int;
    }
    if(map_display_background())
    {
        game.is_running = false;
        return_failure_int;
    }

    if(entity_manager_draw())
    {
        game.is_running = false;
        return_failure_int;
    }

    if(map_display_foreground())
    {
        game.is_running = false;
        return_failure_int;
    }

    if(display_update())
    {
        game.is_running = false;
        return_failure_int;
    }

    return EXIT_SUCCESS;
}

int game_exit(void)
{
    if(asset_manager_exit()) return_failure_int;
    if(entity_manager_exit()) return_failure_int;
    if(event_bus_exit()) return_failure_int;
    display_exit();

    return EXIT_SUCCESS;
}
