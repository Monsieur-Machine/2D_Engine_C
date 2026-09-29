#include "display.h"

#include <SDL3/SDL.h>
#include <stdlib.h>
#include "asset_manager.h"
#include "game.h"
#include "texture.h"
#include "errors.h"

#define DRIVERS "vulkan,gpu,software"

inline int display_init(void)
{
    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
    {
        error_printf("Failed to initialise SDL3 : %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    if(!SDL_SetAppMetadata("2D_Engine_SDL3_Core", "1.0", "2D_Engine"))
    {
        error_printf("Failed to set app metadata : %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    game.display.window = SDL_CreateWindow("2D_Engine", DISPLAY_WIDTH, DISPLAY_HEIGHT, 0);
    if(!game.display.window)
    {
        error_printf("Failed to create window : %s", SDL_GetError());
        return EXIT_FAILURE;
    }

#ifdef DRIVERS
    warning_printf("This build of 2D_Engine_C only innitialize for the following drivers : %s", DRIVERS);

    debug_printf("Listing available render drivers.");
    const int NB_DRIVERS = SDL_GetNumRenderDrivers();
    for(int i = 0; i < NB_DRIVERS; i++)
        debug_printf("%d : %s", i, SDL_GetRenderDriver(i));

    game.display.renderer = SDL_CreateRenderer(game.display.window, DRIVERS);
#else
    game.display.renderer = SDL_CreateRenderer(game.display.window, NULL);
#endif // DRIVERS

    if(!game.display.renderer)
    {
        error_printf("Failed to create renderer : %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    if(!SDL_SetRenderVSync(game.display.renderer, 1))
    {
        error_printf("Failed to activate adaptative vsync : %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    if(!SDL_SetRenderDrawBlendMode(game.display.renderer, SDL_BLENDMODE_BLEND))
    {
        error_printf("Failed to set renderer draw blend mode : %s", SDL_GetError());
        return EXIT_FAILURE;
    }
/*
    if(!SDL_SetDefaultTextureScaleMode(game.display.renderer, SDL_SCALEMODE_NEAREST))
    {
        error_printf("Failed to set default texture scale mode : %s", SDL_GetError());
        return EXIT_FAILURE;
    }
*/
    debug_printf("Initialized display.");

    return EXIT_SUCCESS;
}

inline void display_exit(void)
{
    if(game.display.renderer) SDL_DestroyRenderer(game.display.renderer);
    if(game.display.window) SDL_DestroyWindow(game.display.window);
    SDL_Quit();

    debug_printf("Exited display.");
}

inline int display_clear(void)
{
    if(!SDL_SetRenderDrawColor(game.display.renderer, C_WHITE, 255))
    {
        error_printf("Failed to change renderer draw color : %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    if(!SDL_RenderClear(game.display.renderer))
    {
        error_printf("Failed to clear renderer : %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

inline int display_update(void)
{
    if(!SDL_RenderPresent(game.display.renderer))
    {
        error_printf("Failed to present renderer : %s", SDL_GetError());
        return EXIT_FAILURE;
    }

#if DEBUG >= 1
    float fps = 1000.0f / game.delta_time_ms;
    static float max_fps_10s = 0.0f;
    static float low_fps_10s = 10000000.0f;

    if(fps > max_fps_10s) max_fps_10s = fps;
    if(fps < low_fps_10s) low_fps_10s = fps;

    static float fps_update_delta_time = 0.0f;
    fps_update_delta_time += game.delta_time_ms;

    if(fps_update_delta_time >= 100.0f)
    {
        fps_update_delta_time = 0.0f;
        char title[100];
        if(sprintf(title, "2D_Engine | cur: %.2ffps | max: %.2ffps | low: %.2ffps", fps, max_fps_10s, low_fps_10s) == -1)
        {
            error_printf("Sprintf failed : %d", errno);
            return EXIT_FAILURE;
        }
        if(!SDL_SetWindowTitle(game.display.window, title))
        {
            error_printf("Failed to set window title : %s", SDL_GetError());
            return EXIT_FAILURE;
        }
    }

    static float fps_max_low_update_delta_time = 0.0f;
    fps_max_low_update_delta_time += game.delta_time_ms;

    if(fps_max_low_update_delta_time > 10000.0f)
    {
        fps_max_low_update_delta_time = 0.0f;
        max_fps_10s = 0.0f;
        low_fps_10s = 10000000.0f;
    }
#endif // DEBUG >= 1

    return EXIT_SUCCESS;
}

int display_draw_texture(texture_t *texture, const rect_t *src_rect, const rect_t *dst_rect, bool flip)
{
    frect_t src_frect;
    if(src_rect) rect_to_frect(src_rect, &src_frect);

    frect_t dst_frect;
    if(dst_rect)
    dst_frect = (frect_t) {
            .x = (dst_rect->x - game.camera.view_rect.x) * (DISPLAY_WIDTH  / game.camera.view_rect.w),
            .y = (dst_rect->y - game.camera.view_rect.y) * (DISPLAY_HEIGHT / game.camera.view_rect.h),
            .w = dst_rect->w * (DISPLAY_WIDTH  / game.camera.view_rect.w),
            .h = dst_rect->h * (DISPLAY_HEIGHT / game.camera.view_rect.h)
    };

    if(!SDL_RenderTextureRotated(game.display.renderer, texture, src_rect ? &src_frect : NULL, dst_rect ? &dst_frect : NULL, 0, NULL, flip))
    {
        error_printf("Failed to copy texture to renderer : %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int display_draw_frect(const frect_t *frect, unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
    if(!SDL_SetRenderDrawColor(game.display.renderer, r, g, b, a))
    {
        error_printf("Failed to change renderer draw color : %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    frect_t display_frect = {
        .x = (frect->x - game.camera.view_rect.x) * (DISPLAY_WIDTH  / game.camera.view_rect.w),
        .y = (frect->y - game.camera.view_rect.y) * (DISPLAY_HEIGHT / game.camera.view_rect.h),
        .w = frect->w * (DISPLAY_WIDTH  / game.camera.view_rect.w),
        .h = frect->h * (DISPLAY_HEIGHT / game.camera.view_rect.h)
    };

    if(!SDL_RenderRect(game.display.renderer, &display_frect))
    {
        error_printf("Failed to draw rectangle to renderer : %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
