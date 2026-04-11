#include <malloc.h>
#include <string.h>
#include "texture_manager.h"
//#include "textures.h"

texture_t *create_texture(image_t *image, const char *name)
{
    if(!image) return NULL;

    texture_t *texture = kmalloc(sizeof(texture_t), NULL);
    if (!texture) return NULL;

    texture->name = name;
    texture->image = image;

    return texture;
}

inline void destroy_texture(texture_t *texture)
{
    kfree(texture);
}

inline void draw_texture(const texture_t *texture, const rect_t *src, const rect_t *dst, bool flip)
{
    dsubimage_rgb16_effect((int)dst->x, (int)dst->y, texture->image, (int)src->x, (int)src->y, (int)src->w, (int)src->h, flip ? IMAGE_HFLIP : DIMAGE_NONE);
}

inline void texture_manager_init(texture_manager_t *texture_manager)
{
    linked_list_init(&texture_manager->textures, sizeof(texture_t), NULL);
}

void texture_manager_load_builtin_textures(texture_manager_t *texture_manager)
{
    for(size_t i = 0; i < BUILTIN_TEXTURE_COUNT; i++)
    {
        texture_t *texture = create_texture((image_t *)builtin_textures[i].image, builtin_textures[i].name);
        texture_manager_add_texture(texture_manager, texture);
    }
}

texture_t *texture_manager_load_builtin_texture(texture_manager_t *texture_manager, const char *name)
{
    for(size_t i = 0; i < BUILTIN_TEXTURE_COUNT; i++)
    {
        if(!strcmp(builtin_textures[i].name, name))
        {
            texture_t *texture = create_texture((image_t *)builtin_textures[i].image, builtin_textures[i].name);
            texture_manager_add_texture(texture_manager, texture);
            return texture;
        }
    }

    return NULL;
}

inline void texture_manager_add_texture(texture_manager_t *texture_manager, texture_t *texture)
{
    linked_list_push_back(&texture_manager->textures, texture);
}

// Only for this file
// Arg : const char *name
static inline bool is_texture_name(elem_t *elem, va_list args)
{
    return !strcmp(((texture_t *)elem->data)->name, va_arg(args, const char*));
}

texture_t *texture_manager_get_texture(texture_manager_t *texture_manager, const char *name)
{
    texture_t *texture = linked_list_get_if(&texture_manager->textures, is_texture_name, name);
    if(texture) return texture;

    texture = texture_manager_load_builtin_texture(texture_manager, name);
    return texture;
}

inline void texture_manager_remove_texture(texture_manager_t *texture_manager, const char *name)
{
    linked_list_remove_if(&texture_manager->textures, is_texture_name, name);
}

inline void texture_manager_clear(texture_manager_t *texture_manager)
{
    linked_list_clear(&texture_manager->textures);
}

inline void texture_manager_draw_texture(texture_manager_t *texture_manager, const char *name, const rect_t *src, const rect_t *dst, bool flip)
{
    draw_texture(texture_manager_get_texture(texture_manager, name), src, dst, flip);
}
