#ifndef DISPLAY_H
#define DISPLAY_H

#include <gint/display.h>
#include "texture.h"
#include "vector2d.h"

#define DISPLAY_WIDTH DWIDTH
#define DISPLAY_HEIGHT DHEIGHT

typedef struct display_t {
} display_t;

int display_init(void);
void display_exit(void);

int display_clear(void);
int display_update(void);

int display_draw_texture(texture_t *texture, const rect_t *src_rect, const rect_t *dst_rect, bool flip);
int display_draw_frect(const frect_t *frect, unsigned int r, unsigned int g, unsigned int b, unsigned int a);

#endif // DISPLAY_H