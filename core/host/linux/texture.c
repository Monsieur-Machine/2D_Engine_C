#include "texture.h"

#include "asset_manager.h"
#include "game.h"
#include "display.h"
#include "errors.h"

texture_t *load_texture(void)
{
    uint16_t width, height;

    if(fread(&width, sizeof(width), 1, game.asset_manager.asset_file) != 1)
    {
        error_printf("Failed to read in asset file : %d", ferror(game.asset_manager.asset_file));
        return NULL;
    }

    if(fread(&height, sizeof(height), 1, game.asset_manager.asset_file) != 1)
    {
        error_printf("Failed to read in asset file : %d", ferror(game.asset_manager.asset_file));
        return NULL;
    }

    SDL_Surface *tmp_surface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGB565);
    if(!tmp_surface)
    {
        error_printf("Failed to create temporary surface : %s", SDL_GetError());
        return NULL;
    }

    if(!SDL_LockSurface(tmp_surface))
    {
        error_printf("Failed to lock temporary surface : %s", SDL_GetError());
        goto failure;
    }

    if(fread(tmp_surface->pixels, sizeof(uint16_t), width * height, game.asset_manager.asset_file) != (size_t)width * (size_t)height)
    {
        error_printf("Failed to read asset file : %d", ferror(game.asset_manager.asset_file));
        goto failure;
    }

    if(!SDL_SetSurfaceColorKey(tmp_surface, true, TEXTURE_TRANSPARENT_COLOR))
    {
        error_printf("Failed to set color key of temporary surface : %s", SDL_GetError());
        goto failure;
    }

    texture_t *texture = SDL_CreateTextureFromSurface(game.display.renderer, tmp_surface);
    if(!texture)
    {
        error_printf("Failed to create texture from temporary surface : %s", SDL_GetError());
        goto failure;
    }

    SDL_DestroySurface(tmp_surface);
    return texture;

failure:
    SDL_DestroySurface(tmp_surface);
    return NULL;
}

void destroy_texture(texture_t *texture)
{
    SDL_DestroyTexture(texture);
}
