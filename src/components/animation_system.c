#include "animation_system.h"

#include "game.h"
#include "memory_alloc.h"
#include "errors.h"

int animation_system_init(component_t *component)
{
    animation_system_data_t *system_data = component->data;

    component_t *sprite_component = entity_get_component(component->entity, SPRITE_COMPONENT);
    if(!sprite_component)
    {
        error_printf("Animation system require a sprite component.");
        return EXIT_FAILURE;
    }
    system_data->sprite_component_data = sprite_component->data;

    system_data->nb_frames = 1;
    system_data->current_frame_nb = 0;
    linked_list_init(&system_data->frames, sizeof(rect_t), NULL, NULL);
    animation_system_create_frames_clips(system_data);

    system_data->frame_delay_ms = 0.0f;
    system_data->frame_timer_ms = 0.0f;

    system_data->play = false;
    system_data->loop = false;
    system_data->reverse = false;

    return EXIT_SUCCESS;
}

int animation_system_update(component_t *component)
{
    animation_system_data_t *system_data = component->data;

    if(system_data->play)
    {
        system_data->frame_timer_ms += game.delta_time_ms;

        if(system_data->frame_timer_ms >= system_data->frame_delay_ms)
        {
            system_data->frame_timer_ms = 0.0f;

            if(system_data->reverse)
            {
                if(system_data->current_frame->prev)
                {
                    system_data->current_frame = system_data->current_frame->prev;
                    system_data->current_frame_nb--;
                }
                else if(system_data->loop)
                {
                    system_data->current_frame = system_data->frames.last;
                    system_data->current_frame_nb = system_data->frames.size - 1;
                }
                else
                {
                    system_data->play = false;
                }
            }
            else
            {
                if(system_data->current_frame->next)
                {
                    system_data->current_frame = system_data->current_frame->next;
                    system_data->current_frame_nb++;
                }
                else if(system_data->loop)
                {
                    system_data->current_frame = system_data->frames.first;
                    system_data->current_frame_nb = 0;
                }
                else
                {
                    system_data->play = false;
                }
            }
        }

        system_data->sprite_component_data->src_rect = *(rect_t *)system_data->current_frame->data;
    }

    return EXIT_SUCCESS;
}

inline int animation_system_destroy(component_t *component)
{
    animation_system_data_t *system_data = component->data;

    if(linked_list_exit(&system_data->frames)) return EXIT_FAILURE;

    free(system_data);

    return EXIT_SUCCESS;
}

int animation_system_create_frames_clips(animation_system_data_t *system_data)
{
    sprite_component_data_t *sprite_component_data = system_data->sprite_component_data;

    if(linked_list_clear(&system_data->frames)) return EXIT_FAILURE;

    if(linked_list_reserve(&system_data->frames, system_data->nb_frames)) return EXIT_FAILURE;

    for(size_t current_frame_nb = 0; current_frame_nb < system_data->nb_frames; current_frame_nb++)
    {
        elem_t *frame = create_elem(&system_data->frames);
        if(!frame) return EXIT_FAILURE;

        ((rect_t *)frame->data)->x = sprite_component_data->src_rect.w * (float)current_frame_nb;
        ((rect_t *)frame->data)->y = 0;
        ((rect_t *)frame->data)->w = sprite_component_data->src_rect.w;
        ((rect_t *)frame->data)->h = sprite_component_data->src_rect.h;

        if(linked_list_push_back(&system_data->frames, frame)) return EXIT_FAILURE;
    }

    system_data->current_frame = linked_list_get(&system_data->frames, system_data->current_frame_nb);
    if(!system_data->current_frame) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

inline int animation_system_apply_changes(animation_system_data_t *system_data)
{
    sprite_component_data_t *sprite_component_data = system_data->sprite_component_data;

    if(animation_system_create_frames_clips(system_data)) return EXIT_FAILURE;

    sprite_component_data->src_rect = *(rect_t *)system_data->current_frame->data;

    return EXIT_SUCCESS;
}
