#include "memory_alloc.h"
#include "animation_system.h"
#include "game.h"

// Args : size_t nb_frames, size_t actual_frame, float frame_delay_ms, bool play, bool loop, bool reverse
void animation_system_init(component_t *component, va_list args)
{
    animation_system_data_t *system_data = component->component_data.animation_system_data;

    system_data->sprite_component_data = get_component(component->entity, SPRITE_COMPONENT)->component_data.sprite_component_data;

    system_data->nb_frames = va_arg(args, size_t);
    system_data->actual_frame_nb= va_arg(args, size_t);
    system_data->frame_delay_ms = (float)va_arg(args, double);
    system_data->play = (bool)va_arg(args, int);
    system_data->loop = (bool)va_arg(args, int);
    system_data->reverse = (bool)va_arg(args, int);

    system_data->frame_timer_ms = 0;

    linked_list_init(&system_data->frames, sizeof(rect_t), NULL);

    animation_system_create_frames_clips(system_data);
}

void animation_system_update(component_t *component)
{
    animation_system_data_t *system_data = component->component_data.animation_system_data;

    if(system_data->play)
    {
        system_data->frame_timer_ms += (float)game.delta_time_ms;

        if(system_data->frame_timer_ms >= system_data->frame_delay_ms)
        {
            system_data->frame_timer_ms = 0;

            if(system_data->reverse)
            {
                if(system_data->actual_frame->prev)
                {
                    system_data->actual_frame = system_data->actual_frame->prev;
                    system_data->actual_frame_nb--;
                }
                else if(!system_data->loop)
                {
                    system_data->play = false;
                }
                else
                {
                    system_data->actual_frame = system_data->frames.last;
                    system_data->actual_frame_nb = system_data->frames.size - 1;
                }
            }
            else
            {
                if(system_data->actual_frame->next)
                {
                    system_data->actual_frame = system_data->actual_frame->next;
                    system_data->actual_frame_nb++;
                }
                else if(!system_data->loop)
                {
                    system_data->play = false;
                }
                else
                {
                    system_data->actual_frame = system_data->frames.first;
                    system_data->actual_frame_nb = 0;
                }
            }
        }

        system_data->sprite_component_data->src_rect = *(rect_t *)system_data->actual_frame->data;
    }
}

inline void animation_system_destroy(component_t *component)
{
    linked_list_clear(&component->component_data.animation_system_data->frames);
    free(component->component_data.animation_system_data);
}

void animation_system_create_frames_clips(animation_system_data_t *system_data)
{
    linked_list_clear(&system_data->frames);

    rect_t *sprite_srcR = &system_data->sprite_component_data->src_rect;

    for(size_t actual_frame_nb = 0; actual_frame_nb < system_data->nb_frames; actual_frame_nb++)
    {
        elem_t *frame = elem_create(&system_data->frames);

        ((rect_t *)frame->data)->x = sprite_srcR->w * (float)actual_frame_nb;
        ((rect_t *)frame->data)->y = 0;
        ((rect_t *)frame->data)->w = sprite_srcR->w;
        ((rect_t *)frame->data)->h = sprite_srcR->h;

        linked_list_push_back_elem(&system_data->frames, frame);
    }

    system_data->actual_frame = linked_list_get_elem(&system_data->frames, system_data->actual_frame_nb);
}

inline void animation_system_change_animation(animation_system_data_t *system_data, const char *name, size_t nb_frames)
{
    system_data->nb_frames = nb_frames;
    sprite_component_set_texture(system_data->sprite_component_data, name);
    animation_system_create_frames_clips(system_data);
    system_data->sprite_component_data->src_rect = *(rect_t *)system_data->actual_frame->data;
}
