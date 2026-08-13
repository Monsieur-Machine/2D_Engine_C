#include "sprite_component.h"

#include "game.h"
#include "display.h"
#include "memory_alloc.h"

inline int sprite_component_init(component_t *component)
{
    sprite_component_data_t *component_data = component->data;

    component_t * transform_component = entity_get_component(component->entity, TRANSFORM_COMPONENT);
    if(!transform_component)
    {
        error_printf("Sprite component require a transform component.");
        return EXIT_FAILURE;
    }
    component_data->transform_component_data = transform_component->data;

    transform_component_data_t *transform_component_data = component_data->transform_component_data;

    frect_to_rect(&transform_component_data->bounds, &component_data->src_rect);
    component_data->src_rect.x = 0;
    component_data->src_rect.y = 0;

    component_data->flip = false;

    return EXIT_SUCCESS;
}

inline int sprite_component_update(component_t *component)
{
    sprite_component_data_t *component_data = component->data;
    transform_component_data_t *transform_component_data = component_data->transform_component_data;

    component_data->dst_rect = transform_component_data->bounds;
    component->entity->draw_priority = transform_component_data->bounds.y + transform_component_data->bounds.h;

    return EXIT_SUCCESS;
}

inline int sprite_component_draw(const component_t *component)
{
    const sprite_component_data_t *component_data = component->data;

    if(display_draw_texture(component_data->texture, &component_data->src_rect, &component_data->dst_rect, component_data->flip)) return_failure_int;

    return EXIT_SUCCESS;
}

inline int sprite_component_destroy(component_t *component)
{
    sprite_component_data_t *component_data = component->data;

    free(component_data);

    return EXIT_SUCCESS;
}

inline int sprite_component_set_texture(sprite_component_data_t *component_data, const char *texture_name)
{
    asset_t *texture_asset = asset_manager_get(&game.asset_manager, texture_name);
    if(!texture_asset) return_failure_int;

    component_data->texture = texture_asset->data;

    return EXIT_SUCCESS;
}
