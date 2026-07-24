#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include "ecs.h"
#include "sprite_component.h"

typedef struct animation_system_data_t {
    sprite_component_data_t *sprite_component_data;

    size_t nb_frames;
    linked_list_t frames;

    elem_t *actual_frame;
    size_t actual_frame_nb;

    float frame_delay_ms;
    float frame_timer_ms;

    bool play       :1;
    bool loop       :1;
    bool reverse    :1;
} animation_system_data_t;

void animation_system_init(component_t *component, va_list args);

void animation_system_update(component_t *component);

void animation_system_destroy(component_t *component);

void animation_system_create_frames_clips(animation_system_data_t *system_data);

void animation_system_change_animation(animation_system_data_t *system_data, const char *name, size_t nb_frames);

#endif // SPRITE_COMPONENT_H