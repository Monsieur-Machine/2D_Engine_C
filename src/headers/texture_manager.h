#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <image.h>
#include "vector2d.h"
#include "linked_list.h"

/* texture_t: Texture definition
   This struct is the base for textures

   @name    Name of the texture
   @image   Texture of type bopti_image_t */
typedef struct texture_t {
    const char *name;
    image_t *image;
} texture_t;

/* create_texture(): Create a texture with the given image and name

   @name    Name of the texture
   @image   Texture of type bopti_image_t
   Return the created texture, NULL on error. */
texture_t *create_texture(image_t *, const char *);

/* destroy_texture(): Destroy the given texture

   @texture     Texture to destroy */
void destroy_texture(texture_t *);

/* draw_texture(): Draw the given texture from given src rect to dst rect

   @texture     Texture to draw
   @src         Source rectangle of the texture to draw
   @dst         Destination of the drawing */
void draw_texture(const texture_t *, const rect_t *, const rect_t *, bool);

/* texture_manager_t: Texture manager

   @textures    Textures loaded in memory  */
typedef struct texture_manager_t {
    linked_list_t textures;
} texture_manager_t;

/* texture_manager_init(): Initialise the given texture_manager

   @texture_manager     Pointer to a texture manager */
void texture_manager_init(texture_manager_t *);

/* texture_manager_load_builtin_textures(): Load textures declared in "textures.h"

   @texture_manager     Pointer to a texture manager */
void texture_manager_load_builtin_textures(texture_manager_t *);

/* texture_manager_load_builtin_texture(): Load texture declared in "textures.h" wich have "name"

   @texture_manager     Pointer to a texture manager
   @name                Name of the texture to load */
texture_t *texture_manager_load_builtin_texture(texture_manager_t *, const char*);

/* texture_manager_add_texture(): Add a texture in the texture manager

   @texture_manager     Pointer to a texture manager
   @texture             Texture to add in the texture manager */
void texture_manager_add_texture(texture_manager_t *, texture_t *);

/* texture_manager_get_texture(): Get a texture in the texture manager

   @texture_manager     Pointer to a texture manager
   @name                Name of the texture to get
   Return the texture, NULL on error or if the texture is not found. */
texture_t *texture_manager_get_texture(texture_manager_t *, const char *);

/* texture_manager_remove_texture(): Remove a texture in the texture manager

   @texture_manager     Pointer to a texture manager
   @name                Name of the texture to remove
   Do nothing if the texture is not found. */
void texture_manager_remove_texture(texture_manager_t *, const char *);

/* texture_manager_clear(): Clear the texture manager

   @texture_manager     Pointer to a texture manager */
void texture_manager_clear(texture_manager_t *);

/* texture_manager_draw_texture(): Draw a texture in the texture manager

   @texture_manager     Pointer to a texture manager
   @name                Name of the texture to draw
   Do nothing if the texture is not found. */
void texture_manager_draw_texture(texture_manager_t *, const char *, const rect_t *, const rect_t *, bool);

#endif // TEXTURE_MANAGER_H