#ifndef TEXTURES_H
#define TEXTURES_H

#include <gint/display.h>
#include "texture_manager.h"

typedef struct builtin_texture_t {
    const char *name;
    bopti_image_t *image;
} builtin_texture_t;

extern bopti_image_t img_player_idle_sheet;
extern bopti_image_t img_player_run_sheet;

static struct builtin_texture_t builtin_textures[] = {
    {"player_idle_sheet", &img_player_idle_sheet},
    {"player_run_sheet",  &img_player_run_sheet}
};

#define BUILTIN_TEXTURE_COUNT (sizeof(builtin_textures) / sizeof(builtin_texture_t))

#endif // TEXTURES_H