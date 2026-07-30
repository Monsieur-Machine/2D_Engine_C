#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include "ecs.h"
#include "sprite_component.h"

typedef struct animation_system_data_t {
    sprite_component_data_t *sprite_component_data;

    size_t nb_frames;
    linked_list_t frames;

    size_t current_frame_nb;
    elem_t *current_frame;

    float frame_delay_ms;
    float frame_timer_ms;

    bool play       :1;
    bool loop       :1;
    bool reverse    :1;
} animation_system_data_t;

int animation_system_init(component_t *component);

int animation_system_update(component_t *component);

int animation_system_destroy(component_t *component);

int animation_system_create_frames_clips(animation_system_data_t *system_data);

int animation_system_apply_changes(animation_system_data_t *system_data);

#endif // SPRITE_COMPONENT_H