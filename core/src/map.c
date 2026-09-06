#include "map.h"

#include "asset_manager.h"
#include "game.h"
#include "ecs.h"
#include "components.h"
#include "vector2d.h"
#include "display.h"
#include "memory_alloc.h"
#include "errors.h"

map_t *load_map(void)
{
    map_t *map = malloc(sizeof(map_t));

    if(fread(&map->width, sizeof(map->width), 1, game.asset_manager.asset_file) != 1)
    {
        error_printf("Failed to read in asset file : %d", errno);
        return NULL;
    }

    if(fread(&map->height, sizeof(map->height), 1, game.asset_manager.asset_file) != 1)
    {
        error_printf("Failed to read in asset file : %d", errno);
        return NULL;
    }

    if(fread(&map->backgroundlayernb, sizeof(map->backgroundlayernb), 1, game.asset_manager.asset_file) != 1)
    {
        error_printf("Failed to read in asset file : %d", errno);
        return NULL;
    }

    linked_list_init(&map->backgroundlayers, map->width * map->height * sizeof(uint16_t), NULL, NULL);
    if(linked_list_reserve(&map->backgroundlayers, map->backgroundlayernb)) return_failure_ptr;

    for(uint8_t i = 0; i < map->backgroundlayernb; i++)
    {
        elem_t *backgroundlayer_elem = create_elem(&map->backgroundlayers);
        if(!backgroundlayer_elem) return_failure_ptr;

        uint16_t *backgroundlayer = backgroundlayer_elem->data;
        if(fread(backgroundlayer, sizeof(*backgroundlayer), map->width * map->height, game.asset_manager.asset_file) != (size_t)map->width * (size_t)map->height)
        {
            error_printf("Failed to read in asset file : %d", errno);
            return NULL;
        }

        linked_list_push_back(&map->backgroundlayers, backgroundlayer_elem);
    }

    map->foregroundlayer = malloc(map->width * map->height * sizeof(*map->foregroundlayer));
    if(fread(map->foregroundlayer, sizeof(*map->foregroundlayer), map->width * map->height, game.asset_manager.asset_file) != (size_t)map->width * (size_t)map->height)
    {
        error_printf("Failed to read in asset file : %d", errno);
        return NULL;
    }

    map->hitboxes = malloc(map->width * map->height * sizeof(*map->hitboxes));
    if(fread(map->hitboxes, sizeof(*map->hitboxes), map->width * map->height, game.asset_manager.asset_file) != (size_t)map->width * (size_t)map->height)
    {
        error_printf("Failed to read in asset file : %d", errno);
        return NULL;
    }

    if(fread(&map->entities_nb, sizeof(map->entities_nb), 1, game.asset_manager.asset_file) != 1)
    {
        error_printf("Failed to read in asset file : %d", errno);
        return NULL;
    }

    map->entities_defs = calloc(map->entities_nb, sizeof(map_entity_def_t));

    for(uint32_t i = 0; i < map->entities_nb; i++)
    {
        map_entity_def_t *entity_def = &map->entities_defs[i];

        if(fread(&entity_def->id, sizeof(entity_def->id), 1, game.asset_manager.asset_file) != 1)
        {
            error_printf("Failed to read in asset file : %d", errno);
            return NULL;
        }

        if(fread(&entity_def->components_nb, sizeof(entity_def->components_nb), 1, game.asset_manager.asset_file) != 1)
        {
            error_printf("Failed to read in asset file : %d", errno);
            return NULL;
        }

        entity_def->components_defs = calloc(entity_def->components_nb, sizeof(map_entity_component_def_t));

        for(uint32_t j = 0; j < entity_def->components_nb; j++)
        {
            map_entity_component_def_t *component_def = &entity_def->components_defs[j];

            if(fread(&component_def->type, sizeof(component_def->type), 1, game.asset_manager.asset_file) != 1)
            {
                error_printf("Failed to read in asset file : %d", errno);
                return NULL;
            }

            switch(component_def->type)
            {
                size_t data_size;

              case TRANSFORM_COMPONENT:
              case HITBOX_COMPONENT:
                data_size = sizeof(fvector2d_t) * 2;
                component_def->attributes = malloc(data_size);
                if(fread(component_def->attributes, data_size, 1, game.asset_manager.asset_file) != 1)
                {
                    error_printf("Failed to read in asset file : %d", errno);
                    return NULL;
                }
                break;

              case SPRITE_COMPONENT:
              {
                size_t cap = 0;
                if(getdelim((char **)&component_def->attributes, &cap, '\0', game.asset_manager.asset_file) == -1)
                {
                    error_printf("Failed to read in asset file : %d", errno);
                    return NULL;
                }
                break;
              }

              case ANIMATION_SYSTEM:
                data_size = sizeof(int) * 2 + sizeof(float) + sizeof(bool) * 3;
                component_def->attributes = malloc(data_size);
                if(fread(component_def->attributes, data_size, 1, game.asset_manager.asset_file) != 1)
                {
                    error_printf("Failed to read in asset file : %d", errno);
                    return NULL;
                }
                break;

              case INTERACT_COMPONENT:
                data_size = sizeof(fvector2d_t) * 2 + sizeof(int) * 3;
                component_def->attributes = malloc(data_size);
                if(fread(component_def->attributes, data_size, 1, game.asset_manager.asset_file) != 1)
                {
                    error_printf("Failed to read in asset file : %d", errno);
                    return NULL;
                }
                break;

              case DOOR_COMPONENT:
                data_size = sizeof(int) + sizeof(bool);
                component_def->attributes = malloc(data_size);
                if(fread(component_def->attributes, data_size, 1, game.asset_manager.asset_file) != 1)
                {
                    error_printf("Failed to read in asset file : %d", errno);
                    return NULL;
                }
                break;

              case LEVER_COMPONENT:
                data_size = sizeof(int) + sizeof(bool);
                component_def->attributes = malloc(data_size);
                if(fread(component_def->attributes, data_size, 1, game.asset_manager.asset_file) != 1)
                {
                    error_printf("Failed to read in asset file : %d", errno);
                    return NULL;
                }
                break;

              default:
                error_printf("Component type not supported for loading : %s (%d)", component_type_to_name(component_def->type), component_def->type);
                return NULL;
            }
        }
    }

    return map;
}

int destroy_map(map_t *map)
{
    if(map->entities_defs)
    {
        for(uint32_t i = 0; i < map->entities_nb; i++)
        {
            map_entity_def_t *entity_def = &map->entities_defs[i];

            if(entity_def->components_defs)
            {
                for(uint32_t j = 0; j < entity_def->components_nb; j++)
                {
                    map_entity_component_def_t *component_def = &entity_def->components_defs[j];
                    if(component_def->attributes) free(component_def->attributes);
                }

                free(entity_def->components_defs);
            }
        }

        free(map->entities_defs);
    }

    if(map->hitboxes) free(map->hitboxes);

    if(map->foregroundlayer) free(map->foregroundlayer);

    if(linked_list_exit(&map->backgroundlayers)) return_failure_int;

    free(map);

    return EXIT_SUCCESS;
}

int map_load_entities(map_t *map)
{
    for(uint32_t i = 0; i < map->entities_nb; i++)
    {
        map_entity_def_t *entity_def = &map->entities_defs[i];

        entity_t *entity = entity_manager_new_entity(entity_def->id);
        if(!entity) return_failure_int;

        for(uint32_t j = 0; j < entity_def->components_nb; j++)
        {
            map_entity_component_def_t *component_def = &entity_def->components_defs[j];

            component_t *component = entity_new_component(entity, component_def->type);
            if(!entity) return_failure_int;

            switch(component_def->type)
            {
              case TRANSFORM_COMPONENT:
              {
                struct {
                    frect_t bounds;
                } *data = component_def->attributes;

                transform_component_data_t *component_data = component->data;

                component_data->bounds = data->bounds;

                break;
              }

              case SPRITE_COMPONENT:
              {
                sprite_component_data_t *component_data = component->data;

                asset_t *texture_asset = asset_manager_load_asset((char *)component_def->attributes, ASSET_TEXTURE);
                if(!texture_asset) return_failure_int;

                component_data->texture = texture_asset->data;

                break;
              }

              case ANIMATION_SYSTEM:
              {
                struct {
                    int nb_frames;
                    int current_frame_nb;
                    float frame_delay_ms;
                    bool play;
                    bool loop;
                    bool reverse;
                } *data = component_def->attributes;

                animation_system_data_t *system_data = component->data;

                system_data->nb_frames = data->nb_frames;
                system_data->current_frame_nb = data->current_frame_nb;
                system_data->frame_delay_ms = data->frame_delay_ms;
                system_data->play = data->play;
                system_data->loop = data->loop;
                system_data->reverse = data->reverse;

                if(animation_system_apply_changes(component)) return_failure_int;

                break;
              }

              case HITBOX_COMPONENT:
              {
                struct {
                    fvector2d_t position;
                    fvector2d_t scale;
                } *data = component_def->attributes;

                hitbox_component_data_t *component_data = component->data;

                component_data->position = data->position;
                component_data->scale = data->scale;

                break;
              }

              case DOOR_COMPONENT:
              {
                struct {
                    int topic_id;
                    bool state;
                } *data = component_def->attributes;

                door_component_data_t *component_data = component->data;

                component_data->topic_id = data->topic_id;
                component_data->state = data->state;

                if(door_component_subscribe(component)) return_failure_int;

                break;
              }

              case INTERACT_COMPONENT:
              {
                struct {
                    fvector2d_t position;
                    fvector2d_t scale;
                    int topic_id;
                    int type;
                    int state;
                } *data = component_def->attributes;

                interact_component_data_t *component_data = component->data;

                component_data->position = data->position;
                component_data->scale = data->scale;
                component_data->type = data->type;
                component_data->state = data->state;

                if(event_bus_new_topic(data->topic_id)) return_failure_int;

                break;
              }

              case LEVER_COMPONENT:
              {
                struct {
                    int topic_id;
                    bool state;
                } *data = component_def->attributes;

                lever_component_data_t *component_data = component->data;

                component_data->topic_id = data->topic_id;
                component_data->state = data->state;

                if(lever_component_subscribe(component)) return_failure_int;

                break;
              }

              default:
                error_printf("Component type not supported for loading : %s (%d)", component_type_to_name(component_def->type), component_def->type);
                return EXIT_FAILURE;
            }
        }
    }

    return EXIT_SUCCESS;
}

int map_remove_entities(map_t *map)
{
    for(uint32_t i = 0; i < map->entities_nb; i++)
    {
        map_entity_def_t *entity_def = &map->entities_defs[i];

        if(entity_manager_remove_entity(entity_def->id)) return_failure_int;

        for(uint32_t j = 0; j < entity_def->components_nb; j++)
        {
            map_entity_component_def_t *component_def = &entity_def->components_defs[j];

            switch(component_def->type)
            {
              case SPRITE_COMPONENT:
                if(asset_manager_let_go_asset((char *)component_def->attributes)) return_failure_int;
                break;

              case INTERACT_COMPONENT:
              {
                struct {
                    fvector2d_t position;
                    fvector2d_t scale;
                    int topic_id;
                    int type;
                    int state;
                } *data = component_def->attributes;

                if(event_bus_remove_topic(data->topic_id)) return_failure_int;

                break;
              }

              default:
                break;
            }
        }
    }

    return EXIT_SUCCESS;
}


static inline int draw_tile(uint16_t tile_x, uint16_t tile_y, uint16_t tile_id)
{
    if(!tile_id) return EXIT_SUCCESS;

    rect_t src_frect = {
        .x = tile_id * TILE_SIZE % game.map->tileset->w,
        .y = tile_id * TILE_SIZE / game.map->tileset->w * TILE_SIZE,
        .w = TILE_SIZE,
        .h = TILE_SIZE
    };

    rect_t dst_frect = {
        .x = tile_x * TILE_SIZE,
        .y = tile_y * TILE_SIZE,
        .w = TILE_SIZE,
        .h = TILE_SIZE
    };

    if(display_draw_texture(game.map->tileset, &src_frect, &dst_frect, false)) return_failure_int;

    return EXIT_SUCCESS;
}

int map_display_background(void)
{
    uint16_t left_tile = 0;
    if(game.camera.view_rect.x / TILE_SIZE > 0)
        left_tile = game.camera.view_rect.x / TILE_SIZE;

    uint16_t right_tile = game.map->width;
    if((game.camera.view_rect.x + game.camera.view_rect.w) / TILE_SIZE + 1 < game.map->width)
        right_tile = (game.camera.view_rect.x + game.camera.view_rect.w) / TILE_SIZE + 1;

    uint16_t top_tile = 0;
    if(game.camera.view_rect.y / TILE_SIZE > 0)
        top_tile = game.camera.view_rect.y / TILE_SIZE;

    uint16_t bottom_tile = game.map->height;
    if((game.camera.view_rect.y + game.camera.view_rect.h) / TILE_SIZE + 1 < game.map->height)
        bottom_tile = (game.camera.view_rect.y + game.camera.view_rect.h) / TILE_SIZE + 1;

    for(uint16_t tile_y = top_tile; tile_y < bottom_tile; tile_y++)
    for(uint16_t tile_x = left_tile; tile_x < right_tile; tile_x++)
    {
        elem_t *backgroundlayer_elem = game.map->backgroundlayers.first;

        while(backgroundlayer_elem)
        {
            if(draw_tile(tile_x, tile_y, ((uint16_t *)backgroundlayer_elem->data)[tile_y * game.map->width + tile_x])) return_failure_int;
            backgroundlayer_elem = backgroundlayer_elem->next;
        }
    }

    return EXIT_SUCCESS;
}

int map_display_foreground(void)
{
    uint32_t left_tile = 0;
    if(game.camera.view_rect.x / TILE_SIZE > 0)
        left_tile = game.camera.view_rect.x / TILE_SIZE;

    uint32_t right_tile = game.map->width;
    if((game.camera.view_rect.x + game.camera.view_rect.w) / TILE_SIZE + 1 < (int)game.map->width)
        right_tile = (game.camera.view_rect.x + game.camera.view_rect.w) / TILE_SIZE + 1;

    uint32_t top_tile = 0;
    if(game.camera.view_rect.y / TILE_SIZE > 0)
        top_tile = game.camera.view_rect.y / TILE_SIZE;

    uint32_t bottom_tile = game.map->height;
    if((game.camera.view_rect.y + game.camera.view_rect.h) / TILE_SIZE + 1 < (int)game.map->height)
        bottom_tile = (game.camera.view_rect.y + game.camera.view_rect.h) / TILE_SIZE + 1;

    for(uint32_t tile_y = top_tile; tile_y < bottom_tile; tile_y++)
    for(uint32_t tile_x = left_tile; tile_x < right_tile; tile_x++)
    {
        if(draw_tile(tile_x, tile_y, game.map->foregroundlayer[tile_y * game.map->width + tile_x])) return_failure_int;
    }

    return EXIT_SUCCESS;
}
