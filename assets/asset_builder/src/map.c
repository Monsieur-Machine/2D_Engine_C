#include "map.h"

#include <json-c/json.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "asset_file.h"
#include "errors.h"

int parse_map_for_header(const char *input_file_name, asset_header_def_t *asset_header_def)
{
    json_object *root = json_object_from_file(input_file_name);
    if (!root)
    {
        error_printf("Failed to load input file.");
        return EXIT_FAILURE;
    }

    if(json_object_object_length(root) != 8)
    {
        json_object_put(root);
        error_printf("Map file doesn't contain 8 elements at its root.");
        return EXIT_FAILURE;
    }

    json_object *tmp_obj;

    if(!json_object_object_get_ex(root, "MapWidth", &tmp_obj))
    {
        json_object_put(root);
        error_printf("Map file doesn't contain key \"MapWidth\".");
        return EXIT_FAILURE;
    }

    if(!json_object_is_type(tmp_obj, json_type_int))
    {
        error_printf("Map key \"MapWidth\" is not of type int.");
        return EXIT_FAILURE;
    }

    int map_width = json_object_get_int(tmp_obj);

    if(!json_object_object_get_ex(root, "MapHeight", &tmp_obj))
    {
        json_object_put(root);
        error_printf("Map file doesn't contain key \"MapHeight\".");
        return EXIT_FAILURE;
    }

    if(!json_object_is_type(tmp_obj, json_type_int))
    {
        error_printf("Map key \"MapHeight\" is not of type int.");
        return EXIT_FAILURE;
    }

    int map_height = json_object_get_int(tmp_obj);

    json_object *entities;

    if(!json_object_object_get_ex(root, "Entities", &entities))
    {
        json_object_put(root);
        error_printf("Map file doesn't contain key \"Entities\".");
        return EXIT_FAILURE;
    }

    if(!json_object_is_type(entities, json_type_array))
    {
        json_object_put(root);
        error_printf("Map object \"Entities\" is not an array.");
        return EXIT_FAILURE;
    }

    uint32_t entities_nb = json_object_array_length(entities);

    asset_header_def->start = 0;
    asset_header_def->end = sizeof(map_width) + sizeof(map_height)
                            + map_width * map_height * sizeof(uint16_t) * 4
                            + map_width * map_height * sizeof(uint8_t)
                            + sizeof(entities_nb);

    for(uint32_t i = 0; i < entities_nb; i++)
    {
        json_object *entity = json_object_array_get_idx(entities, i);

        if(!json_object_is_type(entity, json_type_object))
        {
            error_printf("Map entity is not of type object.");
            return EXIT_FAILURE;
        }

        asset_header_def->end += sizeof(uint32_t);

        json_object *components;

        if(!json_object_object_get_ex(entity, "Components", &components))
        {
            error_printf("Entity doesn't have key \"Components\".");
            return EXIT_FAILURE;
        }

        if(!json_object_is_type(components, json_type_array))
        {
            error_printf("Component is not of type array.");
            return EXIT_FAILURE;
        }

        uint32_t components_nb = json_object_array_length(components);

        asset_header_def->end += sizeof(components_nb);

        for(uint32_t j = 0; j < components_nb; j++)
        {
            json_object *component = json_object_array_get_idx(components, j);

            if(!json_object_is_type(component, json_type_object))
            {
                error_printf("Entity component is not of type object.");
                return EXIT_FAILURE;
            }

            asset_header_def->end += sizeof(uint32_t);

            json_object *attributes;

            if(!json_object_object_get_ex(component, "Attributes", &attributes))
            {
                error_printf("Component doesn't have key \"Attributes\".");
                return EXIT_FAILURE;
            }

            if(!json_object_is_type(attributes, json_type_array))
            {
                error_printf("Attributes is not of type array.");
                return EXIT_FAILURE;
            }

            uint32_t attributes_nb = json_object_array_length(attributes);

            for(uint32_t k = 0; k < attributes_nb; k++)
            {
                json_object *attribute = json_object_array_get_idx(attributes, k);

                switch(json_object_get_type(attribute))
                {
                  case json_type_int:
                    asset_header_def->end += sizeof(uint32_t);
                    break;

                  case json_type_double:
                    asset_header_def->end += sizeof(float);
                    break;

                  case json_type_boolean:
                    asset_header_def->end += sizeof(bool);
                    break;

                  case json_type_string:
                    asset_header_def->end += json_object_get_string_len(attribute) + 1;
                    break;

                  default:
                    error_printf("Attribute type is not supported : %s", json_type_to_name(json_object_get_type(attribute)));
                    return EXIT_FAILURE;
                }
            }
        }
    }

    json_object_put(root);

    return EXIT_SUCCESS;
}

typedef enum component_type_t {
#define COMPONENT(NAME, ...) NAME,
#include "components.def"
} component_type_t;

int parse_map_for_content_and_write(const char *input_file_name, FILE *output_file)
{
    json_object *root = json_object_from_file(input_file_name);
    if (!root)
    {
        error_printf("Failed to load input file.");
        return EXIT_FAILURE;
    }

    json_object *tmp_obj;

    if(!json_object_object_get_ex(root, "MapWidth", &tmp_obj))
    {
        json_object_put(root);
        error_printf("Map file doesn't contain key \"MapWidth\".");
        return EXIT_FAILURE;
    }

    int map_width = json_object_get_int(tmp_obj);

    if(fwrite(&map_width, sizeof(map_width), 1, output_file) != 1)
    {
        json_object_put(root);
        error_printf("Failed to write to output file : %d", errno);
        return EXIT_FAILURE;
    }

    if(!json_object_object_get_ex(root, "MapHeight", &tmp_obj))
    {
        json_object_put(root);
        error_printf("Map file doesn't contain key \"MapHeight\".");
        return EXIT_FAILURE;
    }

    int map_height = json_object_get_int(tmp_obj);

    if(fwrite(&map_height, sizeof(map_height), 1, output_file) != 1)
    {
        json_object_put(root);
        error_printf("Failed to write to output file : %d", errno);
        return EXIT_FAILURE;
    }

    // Background Layer 1
    if(!json_object_object_get_ex(root, "BackgroundLayer1", &tmp_obj))
    {
        json_object_put(root);
        error_printf("Map file doesn't contain key \"BackgroundLayer1\".");
        return EXIT_FAILURE;
    }

    if(!json_object_is_type(tmp_obj, json_type_array))
    {
        json_object_put(root);
        error_printf("Map background layer 1 is not of type array.");
        return EXIT_FAILURE;
    }

    uint16_t layer_data[map_width * map_height];

    for(size_t i = 0; i < (size_t)map_width * (size_t)map_height; i++)
    {
        json_object *tile = json_object_array_get_idx(tmp_obj, i);
        layer_data[i] = json_object_get_int(tile);
    }

    if(fwrite(&layer_data, sizeof(uint16_t), map_width * map_height, output_file) != (size_t)map_width * (size_t)map_height)
    {
        json_object_put(root);
        error_printf("Failed to write to output file : %d", errno);
        return EXIT_FAILURE;
    }

    // Background Layer 2
    if(!json_object_object_get_ex(root, "BackgroundLayer2", &tmp_obj))
    {
        json_object_put(root);
        error_printf("Map file doesn't contain key \"BackgroundLayer2\".");
        return EXIT_FAILURE;
    }

    if(!json_object_is_type(tmp_obj, json_type_array))
    {
        json_object_put(root);
        error_printf("Map background layer 1 is not of type array.");
        return EXIT_FAILURE;
    }

    for(size_t i = 0; i < (size_t)map_width * (size_t)map_height; i++)
    {
        json_object *tile = json_object_array_get_idx(tmp_obj, i);
        layer_data[i] = json_object_get_int(tile);
    }

    if(fwrite(&layer_data, sizeof(uint16_t), map_width * map_height, output_file) != (size_t)map_width * (size_t)map_height)
    {
        json_object_put(root);
        error_printf("Failed to write to output file : %d", errno);
        return EXIT_FAILURE;
    }

    // Background Layer 3
    if(!json_object_object_get_ex(root, "BackgroundLayer3", &tmp_obj))
    {
        json_object_put(root);
        error_printf("Map file doesn't contain key \"BackgroundLayer3\".");
        return EXIT_FAILURE;
    }

    if(!json_object_is_type(tmp_obj, json_type_array))
    {
        json_object_put(root);
        error_printf("Map background layer 1 is not of type array.");
        return EXIT_FAILURE;
    }

    for(size_t i = 0; i < (size_t)map_width * (size_t)map_height; i++)
    {
        json_object *tile = json_object_array_get_idx(tmp_obj, i);
        layer_data[i] = json_object_get_int(tile);
    }

    if(fwrite(&layer_data, sizeof(uint16_t), map_width * map_height, output_file) != (size_t)map_width * (size_t)map_height)
    {
        json_object_put(root);
        error_printf("Failed to write to output file : %d", errno);
        return EXIT_FAILURE;
    }

    // Foreground
    if(!json_object_object_get_ex(root, "Foreground", &tmp_obj))
    {
        json_object_put(root);
        error_printf("Map file doesn't contain key \"Foreground\".");
        return EXIT_FAILURE;
    }

    if(!json_object_is_type(tmp_obj, json_type_array))
    {
        json_object_put(root);
        error_printf("Map background layer 1 is not of type array.");
        return EXIT_FAILURE;
    }

    for(size_t i = 0; i < (size_t)map_width * (size_t)map_height; i++)
    {
        json_object *tile = json_object_array_get_idx(tmp_obj, i);
        layer_data[i] = json_object_get_int(tile);
    }

    if(fwrite(&layer_data, sizeof(uint16_t), map_width * map_height, output_file) != (size_t)map_width * (size_t)map_height)
    {
        json_object_put(root);
        error_printf("Failed to write to output file : %d", errno);
        return EXIT_FAILURE;
    }

    // Hitboxes
    if(!json_object_object_get_ex(root, "Hitboxes", &tmp_obj))
    {
        json_object_put(root);
        error_printf("Map file doesn't contain key \"Hitboxes\".");
        return EXIT_FAILURE;
    }

    if(!json_object_is_type(tmp_obj, json_type_array))
    {
        json_object_put(root);
        error_printf("Map background layer 1 is not of type array.");
        return EXIT_FAILURE;
    }

    uint8_t hitboxes_layer[map_width * map_height];

    for(size_t i = 0; i < (size_t)map_width * (size_t)map_height; i++)
    {
        json_object *tile = json_object_array_get_idx(tmp_obj, i);
        hitboxes_layer[i] = json_object_get_int(tile);
    }

    if(fwrite(&hitboxes_layer, sizeof(uint8_t), map_width * map_height, output_file) != (size_t)map_width * (size_t)map_height)
    {
        json_object_put(root);
        error_printf("Failed to write to output file : %d", errno);
        return EXIT_FAILURE;
    }

    json_object *entities;

    if(!json_object_object_get_ex(root, "Entities", &entities))
    {
        json_object_put(root);
        error_printf("Map file doesn't contain key \"Entities\".");
        return EXIT_FAILURE;
    }

    uint32_t entities_nb = json_object_array_length(entities);

    if(fwrite(&entities_nb, sizeof(entities_nb), 1, output_file) != 1)
    {
        json_object_put(root);
        error_printf("Failed to write to output file : %d", errno);
        return EXIT_FAILURE;
    }

    for(uint32_t i = 0; i < entities_nb; i++)
    {
        json_object *entity = json_object_array_get_idx(entities, i);

        if(!json_object_object_get_ex(entity, "ID", &tmp_obj))
        {
            json_object_put(root);
            error_printf("Entity doesn't have key \"ID\".");
            return EXIT_FAILURE;
        }

        if(!json_object_is_type(tmp_obj, json_type_int))
        {
            json_object_put(root);
            error_printf("Entity ID is not of type int.");
            return EXIT_FAILURE;
        }

        uint32_t entity_id = json_object_get_int(tmp_obj);

        if(fwrite(&entity_id, sizeof(entity_id), 1, output_file) != 1)
        {
            json_object_put(root);
            error_printf("Failed to write to output file : %d", errno);
            return EXIT_FAILURE;
        }

        json_object *components;

        if(!json_object_object_get_ex(entity, "Components", &components))
        {
            json_object_put(root);
            error_printf("Entity doesn't have key \"Components\".");
            return EXIT_FAILURE;
        }

        uint32_t components_nb = json_object_array_length(components);

        if(fwrite(&components_nb, sizeof(components_nb), 1, output_file) != 1)
        {
            json_object_put(root);
            error_printf("Failed to write to output file : %d", errno);
            return EXIT_FAILURE;
        }

        for(uint32_t j = 0; j < components_nb; j++)
        {
            json_object *component = json_object_array_get_idx(components, j);

            json_object *attributes;

            if(!json_object_object_get_ex(component, "Type", &tmp_obj))
            {
                json_object_put(root);
                error_printf("Component doesn't have key \"Type\".");
                return EXIT_FAILURE;
            }

            if(!json_object_is_type(tmp_obj, json_type_string))
            {
                json_object_put(root);
                error_printf("Component type is not of type string.");
                return EXIT_FAILURE;
            }

            component_type_t component_type;
#define COMPONENT(NAME, ...) \
            if(!strcmp(#NAME, json_object_get_string(tmp_obj))) \
                component_type = NAME;
#include "components.def"

            if(fwrite(&component_type, sizeof(uint32_t), 1, output_file) != 1)
            {
                json_object_put(root);
                error_printf("Failed to write to output file : %d", errno);
                return EXIT_FAILURE;
            }

            if(!json_object_object_get_ex(component, "Attributes", &attributes))
            {
                json_object_put(root);
                error_printf("Component doesn't have key \"Attributes\".");
                return EXIT_FAILURE;
            }

            uint32_t attributes_nb = json_object_array_length(attributes);

            for(uint32_t k = 0; k < attributes_nb; k++)
            {
                json_object *attribute = json_object_array_get_idx(attributes, k);

                switch(json_object_get_type(attribute))
                {
                  case json_type_int:
                  {
                    int value = json_object_get_int(attribute);
                    if(fwrite(&value, sizeof(value), 1, output_file) != 1)
                    {
                        json_object_put(root);
                        error_printf("Failed to write to output file : %d", errno);
                        return EXIT_FAILURE;
                    }
                    break;
                  }

                  case json_type_double:
                  {
                    float value = json_object_get_double(attribute);
                    if(fwrite(&value, sizeof(value), 1, output_file) != 1)
                    {
                        json_object_put(root);
                        error_printf("Failed to write to output file : %d", errno);
                        return EXIT_FAILURE;
                    }
                    break;
                  }

                  case json_type_boolean:
                  {
                    bool value = json_object_get_boolean(attribute);
                    if(fwrite(&value, sizeof(value), 1, output_file) != 1)
                    {
                        json_object_put(root);
                        error_printf("Failed to write to output file : %d", errno);
                        return EXIT_FAILURE;
                    }
                    break;
                  }

                  case json_type_string:
                  {
                    const char *str = json_object_get_string(attribute);
                    if(fwrite(str, sizeof(char), strlen(str) + 1, output_file) != strlen(str) + 1)
                    {
                        json_object_put(root);
                        error_printf("Failed to write to output file : %d", errno);
                        return EXIT_FAILURE;
                    }
                    break;
                  }

                  default:
                    return EXIT_FAILURE;
                }
            }
        }
    }

    json_object_put(root);

    return EXIT_SUCCESS;
}
