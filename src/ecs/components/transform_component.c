#include "memory_alloc.h"
#include "transform_component.h"
#include "game.h"

// Args : rect_t bounds, double speed
void transform_component_init(component_t *component, va_list args)
{
    transform_component_data_t *const component_data = component->component_data.transform_component_data;

    rect_t bounds  = va_arg(args, rect_t);
    float speed = (float)va_arg(args, double);

    component_data->bounds = bounds;

    component_data->velocity.x = 0;
    component_data->velocity.y = 0;

    component_data->speed = speed;
}

void transform_component_update(component_t *component)
{
    transform_component_data_t *component_data = component->component_data.transform_component_data;

    component_data->bounds.x += component_data->velocity.x * component_data->speed * (float)game.delta_time_ms / 1000.0f;
    component_data->bounds.y += component_data->velocity.y * component_data->speed * (float)game.delta_time_ms / 1000.0f;
}

inline void transform_component_destroy(component_t *component)
{
    free(component->component_data.transform_component_data);
}
