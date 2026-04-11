#include "memory_alloc.h"
#include "sprite_component.h"
#include "game.h"

// Arg : const char *name
void sprite_component_init(component_t *component, va_list args)
{
    sprite_component_data_t *component_data = component->component_data.sprite_component_data;

    component_data->transform_component_data = get_component(component->entity, TRANSFORM_COMPONENT)->component_data.transform_component_data;

    component_data->src_rect = component_data->transform_component_data->bounds;
    component_data->src_rect.x = 0;
    component_data->src_rect.y = 0;

    component_data->flip = false;

    sprite_component_set_texture(component_data, va_arg(args, const char *));
}

inline void sprite_component_update(component_t *component)
{
    sprite_component_data_t *component_data = component->component_data.sprite_component_data;
    component_data->dst_rect = component_data->transform_component_data->bounds;
}

inline void sprite_component_draw(component_t *component)
{
    sprite_component_data_t *component_data = component->component_data.sprite_component_data;
    draw_texture(component_data->texture, &component_data->src_rect, &component_data->dst_rect, component_data->flip);
}

inline void sprite_component_destroy(component_t *component)
{
    free(component->component_data.sprite_component_data);
}

inline void sprite_component_set_texture(sprite_component_data_t *component_data, const char *name)
{
    component_data->texture = texture_manager_get_texture(&game.texture_manager, name);
}
