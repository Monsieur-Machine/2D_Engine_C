#include "game.h"

#include <stdlib.h>
#include "display.h"
#include "asset_manager.h"
#include "event_bus.h"
#include "ecs.h"
#include "components.h"
#include "inputs.h"
#include "errors.h"

int game_init(void)
{
    game.is_running = false;

    if(display_init()) return_failure_int;
    if(asset_manager_init(&game.asset_manager)) return_failure_int;
    event_bus_init(&game.event_bus);
    entity_manager_init(&game.entity_manager);

    if(asset_manager_load_asset(&game.asset_manager, "player_idle_sheet", ASSET_TEXTURE)) return_failure_int;
    if(asset_manager_load_asset(&game.asset_manager, "player_walk_sheet", ASSET_TEXTURE)) return_failure_int;
    if(asset_manager_load_asset(&game.asset_manager, "player_run_sheet",  ASSET_TEXTURE)) return_failure_int;
    if(asset_manager_load_asset(&game.asset_manager, "lever_sheet",       ASSET_TEXTURE)) return_failure_int;
    if(asset_manager_load_asset(&game.asset_manager, "barred_door_sheet", ASSET_TEXTURE)) return_failure_int;
    if(asset_manager_load_asset(&game.asset_manager, "wooden_door_sheet", ASSET_TEXTURE)) return_failure_int;
    if(asset_manager_load_asset(&game.asset_manager, "tileset",           ASSET_TEXTURE)) return_failure_int;

    /* ======== Player ======== */
    {
        entity_t *player_entity = entity_manager_new_entity(&game.entity_manager, 0);
        if(!player_entity) return_failure_int;

        {
            component_t *transform_component = entity_new_component(player_entity, TRANSFORM_COMPONENT);
            if(!transform_component) return_failure_int;

            transform_component_data_t *transform_component_data = transform_component->data;

            transform_component_data->bounds = (frect_t) {
                .x = 10.0f,
                .y = 10.0f,
                .w = 32.0f,
                .h = 32.0f
            };
        }

        {
            component_t *sprite_component = entity_new_component(player_entity, SPRITE_COMPONENT);
            if(!sprite_component) return_failure_int;

            sprite_component_data_t *sprite_component_data = sprite_component->data;

            sprite_component_set_texture(sprite_component_data, "player_idle_sheet");
        }

        {
            component_t *animation_system = entity_new_component(player_entity, ANIMATION_SYSTEM);
            if(!animation_system) return_failure_int;

            animation_system_data_t *animation_system_data = animation_system->data;

            if(linked_list_reserve(&animation_system_data->frames, 8)) return_failure_int;

            animation_system_data->current_frame_nb = 0;
            animation_system_data->nb_frames = 4;
            if(animation_system_apply_changes(animation_system_data)) return_failure_int;

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
    /* ======== Lever ========*/
    {
        entity_t *lever_entity = entity_manager_new_entity(&game.entity_manager, 1);
        if(!lever_entity) return_failure_int;

        {
            component_t *transform_component = entity_new_component(lever_entity, TRANSFORM_COMPONENT);
            if(!transform_component) return_failure_int;

            transform_component_data_t *transform_component_data = transform_component->data;

            transform_component_data->bounds = (frect_t) {
                .x = 30.0f,
                .y = 120.0f,
                .w = 48.0f,
                .h = 32.0f
            };
        }

        {
            component_t *sprite_component = entity_new_component(lever_entity, SPRITE_COMPONENT);
            if(!sprite_component) return_failure_int;

            sprite_component_data_t *sprite_component_data = sprite_component->data;

            if(sprite_component_set_texture(sprite_component_data, "lever_sheet")) return_failure_int;
        }

        {
            component_t *animation_system = entity_new_component(lever_entity, ANIMATION_SYSTEM);
            if(!animation_system) return_failure_int;

            animation_system_data_t *animation_system_data = animation_system->data;

            animation_system_data->nb_frames = 3;
            animation_system_data->current_frame_nb = 0;
            animation_system_data->frame_delay_ms = LEVER_DEFAULT_ANIMATION_SPEED;

            animation_system_data->play = false;
            animation_system_data->loop = false;
            animation_system_data->reverse = false;

            if(animation_system_apply_changes(animation_system_data)) return_failure_int;
        }

        {
            component_t *hitbox_component = entity_new_component(lever_entity, HITBOX_COMPONENT);
            if(!hitbox_component) return_failure_int;

            hitbox_component_data_t *hitbox_component_data = hitbox_component->data;

            hitbox_component_data->position = (fvector2d_t) {
                .x = 0.3333333333333333f,
                .y = 0.59375
            };

            hitbox_component_data->scale = (fvector2d_t) {
                .x = 0.3333333333333333f,
                .y = 0.34375f
            };
        }

        {
            component_t *interact_component = entity_new_component(lever_entity, INTERACT_COMPONENT);
            if(!interact_component) return_failure_int;

            interact_component_data_t *interact_component_data = interact_component->data;

            interact_component_data->position = (fvector2d_t) {
                .x = 0.16666666666666666f,
                .y = 0.34375f
            };

            interact_component_data->scale = (fvector2d_t) {
                .x = 0.6666666666666666f,
                .y = 0.84375f
            };

            interact_component_data->type = INTERACT_TOGGLE;
            interact_component_data->state = 0;

            interact_component_modify_topic_id(interact_component_data, 0);

        }

        {
            component_t *lever_component = entity_new_component(lever_entity, LEVER_COMPONENT);
            if(!lever_component) return_failure_int;

            lever_component_data_t * lever_component_data = lever_component->data;

            lever_component_data->topic_id = 0;
            lever_component_data->state = false;

            lever_component_subscribe(lever_component_data, lever_entity);
        }
    }

    /* ======== Door ======== */
    {
        entity_t *door_entity = entity_manager_new_entity(&game.entity_manager, 1);
        if(!door_entity) return_failure_int;

        {
            component_t *transform_component = entity_new_component(door_entity, TRANSFORM_COMPONENT);
            if(!transform_component) return_failure_int;

            transform_component_data_t *transform_component_data = transform_component->data;

            transform_component_data->bounds = (frect_t) {
                .x = 120.0f,
                .y = 30.0f,
                .w = 32.0f,
                .h = 48.0f
            };
        }

        {
            component_t *sprite_component = entity_new_component(door_entity, SPRITE_COMPONENT);
            if(!sprite_component) return_failure_int;

            sprite_component_data_t * sprite_component_data = sprite_component->data;

            if(sprite_component_set_texture(sprite_component_data, "barred_door_sheet")) return_failure_int;
        }

        {
            component_t *animation_system = entity_new_component(door_entity, ANIMATION_SYSTEM);
            if(!animation_system) return_failure_int;

            animation_system_data_t *animation_system_data = animation_system->data;

            animation_system_data->nb_frames = 8;
            animation_system_data->current_frame_nb = 0;
            animation_system_data->frame_delay_ms = BARRED_DOOR_DEFAULT_ANIMATION_SPEED;

            animation_system_data->play = false;
            animation_system_data->loop = false;
            animation_system_data->reverse = false;

            if(animation_system_apply_changes(animation_system->data)) return_failure_int;
        }

        {
            component_t *hitbox_component = entity_new_component(door_entity, HITBOX_COMPONENT);
            if(!hitbox_component) return_failure_int;

            hitbox_component_data_t * hitbox_component_data = hitbox_component->data;

            hitbox_component_data->position = (fvector2d_t) {
                .x = 0.0f,
                .y = 0.8333333333333334f
            };

            hitbox_component_data->scale = (fvector2d_t) {
                .x = 1.0f,
                .y = 0.16666666666666666f
            };
        }

        {
            component_t *door_component = entity_new_component(door_entity, DOOR_COMPONENT);
            if(!door_component) return_failure_int;

            door_component_data_t * door_component_data = door_component->data;

            door_component_data->topic_id = 0;
            door_component_data->state = false;

            door_component_subscribe(door_component_data, door_entity);
        }
    }

    /* ======== Door 2 ======== */
    {
        entity_t *door2_entity = entity_manager_new_entity(&game.entity_manager, 1);
        if(!door2_entity) return_failure_int;

        {
            component_t *transform_component = entity_new_component(door2_entity, TRANSFORM_COMPONENT);
            if(!transform_component) return_failure_int;

            transform_component_data_t *transform_component_data = transform_component->data;

            transform_component_data->bounds = (frect_t) {
                .x = 240.0f,
                .y = 30.0f,
                .w = 32.0f,
                .h = 48.0f
            };
        }

        {
            component_t *sprite_component = entity_new_component(door2_entity, SPRITE_COMPONENT);
            if(!sprite_component) return_failure_int;

            sprite_component_data_t *sprite_component_data = sprite_component->data;

            if(sprite_component_set_texture(sprite_component_data, "wooden_door_sheet")) return_failure_int;
        }

        {
            component_t *animation_system = entity_new_component(door2_entity, ANIMATION_SYSTEM);
            if(!animation_system) return_failure_int;

            animation_system_data_t *animation_system_data = animation_system->data;

            animation_system_data->nb_frames = 6;
            animation_system_data->current_frame_nb = 0;
            animation_system_data->frame_delay_ms = WOODEN_DOOR_DEFAULT_ANIMATION_SPEED;

            animation_system_data->play = false;
            animation_system_data->loop = false;
            animation_system_data->reverse = false;

            if(animation_system_apply_changes(animation_system->data)) return_failure_int;
        }

        {
            component_t *hitbox_component = entity_new_component(door2_entity, HITBOX_COMPONENT);
            if(!hitbox_component) return_failure_int;

            hitbox_component_data_t *hitbox_component_data = hitbox_component->data;

            hitbox_component_data->position = (fvector2d_t) {
                .x = 0.0f,
                .y = 0.8333333333333334f
            };

            hitbox_component_data->scale = (fvector2d_t) {
                .x = 1.0f,
                .y = 0.16666666666666666f
            };
        }

        {
            component_t *door_component = entity_new_component(door2_entity, DOOR_COMPONENT);
            if(!door_component) return_failure_int;

            door_component_data_t * door_component_data = door_component->data;

            door_component_data->topic_id = 0;
            door_component_data->state = false;

            door_component_subscribe(door_component_data, door2_entity);
        }
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
        return_failure_int;
    }

    return EXIT_SUCCESS;
}

int game_render(void)
{
    if(display_clear())
    {
        game.is_running = false;
        return_failure_int;
    }

    if(entity_manager_draw(&game.entity_manager))
    {
        game.is_running = false;
        return_failure_int;
    }

    display_update();

    return EXIT_SUCCESS;
}

int game_exit(void)
{
    entity_manager_exit(&game.entity_manager);
    event_bus_exit(&game.event_bus);
    if(asset_manager_exit(&game.asset_manager)) return_failure_int;
    display_exit();

    return EXIT_SUCCESS;
}
