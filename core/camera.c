#include "camera.h"

#include "game.h"
#include "display.h"

void camera_init(void)
{
    game.camera.view_rect = (rect_t) {
        .x = 0,
        .y = 0,
        .w = DISPLAY_WIDTH / 2,
        .h = DISPLAY_HEIGHT / 2
    };

    game.camera.tracking_pos = NULL;
    game.camera.tracking_center_offset = (vector2d_t) {
        .x = 0,
        .y = 0
    };
}

void camera_update(void)
{
    if(game.camera.tracking_pos)
    {
        game.camera.view_rect.x = (int)game.camera.tracking_pos->x + game.camera.tracking_center_offset.x - game.camera.view_rect.w / 2;
        game.camera.view_rect.y = (int)game.camera.tracking_pos->y + game.camera.tracking_center_offset.y - game.camera.view_rect.h / 2;
    }

    if(game.map)
    {
        if(game.camera.view_rect.w > game.map->width * TILE_SIZE)
        {
            game.camera.view_rect.x = (game.map->width * TILE_SIZE - game.camera.view_rect.w) / 2;
        }
        else
        {
            if(game.camera.view_rect.x < 0)
            {
                game.camera.view_rect.x = 0;
            }
            else if(game.camera.view_rect.x + game.camera.view_rect.w > game.map->width * TILE_SIZE)
            {
                game.camera.view_rect.x = game.map->width * TILE_SIZE - game.camera.view_rect.w;
            }
        }

        if(game.camera.view_rect.h > game.map->height * TILE_SIZE)
        {
            game.camera.view_rect.y = (game.map->height * TILE_SIZE - game.camera.view_rect.h) / 2;
        }
        else
        {
            if(game.camera.view_rect.y < 0)
            {
                game.camera.view_rect.y = 0;
            }
            else if(game.camera.view_rect.y + game.camera.view_rect.h > game.map->height * TILE_SIZE)
            {
                game.camera.view_rect.y = game.map->height * TILE_SIZE - game.camera.view_rect.h;
            }
        }
    }
}
