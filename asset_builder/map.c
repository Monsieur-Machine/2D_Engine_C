#include "map.h"

#include <json-c/json.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "errors.h"

static int json_get_object_of_type(const json_object *obj, const char *key, json_type type, json_object **result)
{
    if(!json_object_object_get_ex(obj, key, result))
    {
        error_printf("Object doesn't contain key \"%s\".", key);
        return EXIT_FAILURE;
    }

    if(!json_object_is_type(*result, type))
    {
        error_printf("Key \"%s\" is not of type %s.", key, json_type_to_name(type));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

static inline int json_get_idx_of_type(const json_object *obj, size_t idx, json_type type, json_object **result)
{
    *result = json_object_array_get_idx(obj, idx);

    if(!*result)
    {
        error_printf("Array index out of range : %lu.", idx);
        return EXIT_FAILURE;
    }

    if(!json_object_is_type(*result, type))
    {
        error_printf("Result is not of type %s.", json_type_to_name(type));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

typedef enum component_type_t {
#define COMPONENT(NAME, ...) NAME,
#include "components.def"
} component_type_t;

int build_map(const char *input_file_name, FILE *output_file, uint32_t *asset_size)
{
    json_object *root = json_object_from_file(input_file_name);

    if(!root)
    {
        error_printf("Failed to load input file.");
        return EXIT_FAILURE;
    }

    json_object *tmp_obj;

    if(json_get_object_of_type(root, "MapWidth", json_type_int, &tmp_obj)) goto_failure;
    uint16_t map_width = json_object_get_int(tmp_obj);

    if(json_get_object_of_type(root, "MapHeight", json_type_int, &tmp_obj)) goto_failure;
    uint16_t map_height = json_object_get_int(tmp_obj);

    if(file_write(&map_width, sizeof(map_width), 1, output_file)) goto_failure;
    *asset_size += sizeof(map_width);

    if(file_write(&map_height, sizeof(map_height), 1, output_file)) goto_failure;
    *asset_size += sizeof(map_height);

    // BackgroundLayers
    json_object *backgroundlayers;
    if(json_get_object_of_type(root, "BackgroundLayers", json_type_array, &backgroundlayers)) goto_failure;
    uint8_t layer_count = json_object_array_length(backgroundlayers);

    if(file_write(&layer_count, sizeof(layer_count), 1, output_file)) goto_failure;
    *asset_size += sizeof(layer_count);

    {
        uint16_t layer_data[(size_t)map_width * (size_t)map_height];

        for(uint8_t i = 0; i < layer_count; i++)
        {
            json_object *backgroundlayer;
            if(json_get_idx_of_type(backgroundlayers, i, json_type_array, &backgroundlayer)) goto_failure;

            for(size_t j = 0; j < (size_t)map_width * (size_t)map_height; j++)
            {
                if(json_get_idx_of_type(backgroundlayer, j, json_type_int, &tmp_obj)) goto_failure;
                layer_data[j] = json_object_get_int(tmp_obj);
            }

            if(file_write(&layer_data, sizeof(*layer_data), (size_t)map_width * (size_t)map_height, output_file)) goto_failure;
            *asset_size += sizeof(layer_data);
        }

        // Foreground
        json_object *foregroundlayer;
        if(json_get_object_of_type(root, "ForegroundLayer", json_type_array, &foregroundlayer)) goto_failure;

        for(size_t i = 0; i < (size_t)map_width * (size_t)map_height; i++)
        {
            if(json_get_idx_of_type(foregroundlayer, i, json_type_int, &tmp_obj)) goto_failure;
            layer_data[i] = json_object_get_int(tmp_obj);
        }

        if(file_write(&layer_data, sizeof(*layer_data), (size_t)map_width * (size_t)map_height, output_file)) goto_failure;
        *asset_size += sizeof(layer_data);
    }
    // Hitboxes
    json_object *hitboxes;
    if(json_get_object_of_type(root, "Hitboxes", json_type_array, &hitboxes)) goto_failure;

    {
        uint8_t hitboxes_layer_data[(size_t)map_width * (size_t)map_height];

        for(size_t i = 0; i < (size_t)map_width * (size_t)map_height; i++)
        {
            if(json_get_idx_of_type(hitboxes, i, json_type_int, &tmp_obj)) goto_failure;
            hitboxes_layer_data[i] = json_object_get_int(tmp_obj);
        }

        if(file_write(&hitboxes_layer_data, sizeof(*hitboxes_layer_data), (size_t)map_width * (size_t)map_height, output_file)) goto_failure;
        *asset_size += sizeof(hitboxes_layer_data);
    }

    // Entities
    json_object *entities;

    if(json_get_object_of_type(root, "Entities", json_type_array, &entities)) goto_failure;
    uint16_t entities_nb = json_object_array_length(entities);

    if(file_write(&entities_nb, sizeof(entities_nb), 1, output_file)) goto_failure;
    *asset_size += sizeof(entities_nb);

    for(uint32_t i = 0; i < entities_nb; i++)
    {
        json_object *entity = json_object_array_get_idx(entities, i);

        if(json_get_object_of_type(entity, "ID", json_type_int, &tmp_obj)) goto_failure;

        uint32_t entity_id = json_object_get_int(tmp_obj);

        if(file_write(&entity_id, sizeof(entity_id), 1, output_file)) goto_failure;
        *asset_size += sizeof(entity_id);

        json_object *components;

        if(json_get_object_of_type(entity, "Components", json_type_array, &components)) goto_failure;

        uint16_t components_nb = json_object_array_length(components);

        if(file_write(&components_nb, sizeof(components_nb), 1, output_file)) goto_failure;
        *asset_size += sizeof(components_nb);

        for(uint32_t j = 0; j < components_nb; j++)
        {
            json_object *component;
            if(json_get_idx_of_type(components, j, json_type_object, &component)) goto_failure;

            if(json_get_object_of_type(component, "Type", json_type_string, &tmp_obj)) goto_failure;

            uint16_t component_type;

#define COMPONENT(NAME, ...) \
            if(!strcmp(#NAME, json_object_get_string(tmp_obj))) \
                component_type = NAME;
#include "components.def"

            if(file_write(&component_type, sizeof(component_type), 1, output_file)) goto_failure;

            *asset_size += sizeof(component_type);

            json_object *attributes;
            if(json_get_object_of_type(component, "Attributes", json_type_array, &attributes)) goto_failure;

            uint32_t attributes_nb = json_object_array_length(attributes);

            for(uint32_t k = 0; k < attributes_nb; k++)
            {
                json_object *attribute = json_object_array_get_idx(attributes, k);

                switch(json_object_get_type(attribute))
                {
                  case json_type_int:
                  {
                    uint32_t value = json_object_get_int(attribute);
                    if(file_write(&value, sizeof(value), 1, output_file)) goto_failure;
                    *asset_size += sizeof(value);
                    break;
                  }

                  case json_type_double:
                  {
                    float value = json_object_get_double(attribute);
                    if(file_write(&value, sizeof(value), 1, output_file)) goto_failure;
                    *asset_size += sizeof(value);
                    break;
                  }

                  case json_type_boolean:
                  {
                    bool value = json_object_get_boolean(attribute);
                    if(file_write(&value, sizeof(value), 1, output_file)) goto_failure;
                    *asset_size += sizeof(value);
                    break;
                  }

                  case json_type_string:
                  {
                    const char *str = json_object_get_string(attribute);
                    if(file_write(str, sizeof(*str), strlen(str) + 1, output_file)) goto_failure;
                    *asset_size += (strlen(str) + 1) * sizeof(*str);
                    break;
                  }

                  default:
                    error_printf("Component attribute of type %s is not handled.", json_type_to_name(json_object_get_type(attribute)));
                    goto_failure;
                }
            }
        }
    }

    json_object_put(root);

    return EXIT_SUCCESS;

failure:
    json_object_put(root);

    return EXIT_FAILURE;
}
