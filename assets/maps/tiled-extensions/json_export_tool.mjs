function stringifyFloat(floatNumber)
{
    if(floatNumber == Math.trunc(floatNumber))
        return String(floatNumber) + ".0"

    return String(floatNumber);
}

function writeTileLayer(tileLayer, file)
{
    file.writeLine("  \"" + tileLayer.name + "\": [");

    for(let cell_y = 0; cell_y < tileLayer.height; ++cell_y)
    {
        file.write("   ");

        for(let cell_x = 0; cell_x < tileLayer.width; ++cell_x)
        {
            file.write(" " + tileLayer.cellAt(cell_x, cell_y).tileId)
            if(cell_x < tileLayer.width - 1 || cell_y < tileLayer.height - 1)
                file.write(",");
        }

        file.writeLine("");
    }

    file.writeLine("  ],");
}

function writeEntities(objectLayer, file)
{
    file.writeLine("  \"Entities\": [");

    let entities = [];

    for(let i = 0; i < objectLayer.objectCount; i++)
    {
        let entity = objectLayer.objectAt(i);

        if(entity.name)
        {
            entities.push(entity);
        }
    }

    entities.sort((a, b) => {
        if(Number(a.name) > Number(b.name))
            return 1;

        return -1;
    });

    for(let i = 0; i < entities.length; i++)
    {
        let entity = entities[i];

        file.writeLine("    {");
        file.writeLine("      \"ID\":" + entity.name + ",");
        file.writeLine("      \"Components\": [");

        let components = entity.properties()["components"];
        let nb_components = Object.keys(components).length;

        for(let j = 0; j < nb_components; j++)
        {
            let component = components[j];
            let component_type = component[0];

            file.writeLine("        {");

            file.write("          \"Type\": \"" + component_type + "\"");

            if(component.length > 1)
            {
                file.writeLine(",");
                file.writeLine("          \"Attributes\": [");
            }

            // Write atributes, speciale cases need speciale treatment.
            if(component_type == "TRANSFORM_COMPONENT")
            {
                file.writeLine(",");
                file.writeLine("          \"Attributes\": [");
                file.writeLine("            " + stringifyFloat(entity.x) + ",");
                file.writeLine("            " + stringifyFloat(entity.y) + ",");
                file.writeLine("            " + stringifyFloat(entity.width) + ",");
                file.writeLine("            " + stringifyFloat(entity.height));
                file.writeLine("          ]");
            }
            else if(component_type == "HITBOX_COMPONENT")
            {
                let hitbox;

                for(let k = 0; k < objectLayer.objectCount; k++)
                {
                    hitbox = objectLayer.objectAt(k);
                    if(hitbox.id === component[1].id)
                        break;
                }

                let x = (hitbox.x - entity.x) / entity.width;
                let y = (hitbox.y - entity.y) / entity.height;
                let width = hitbox.width / entity.width;
                let height = hitbox.height / entity.height;

                file.writeLine("            " + stringifyFloat(x) + ",");
                file.writeLine("            " + stringifyFloat(y) + ",");
                file.writeLine("            " + stringifyFloat(width) + ",");
                file.writeLine("            " + stringifyFloat(height));
            }
            else if(component_type == "INTERACT_COMPONENT")
            {
                let interact_box;

                for(let k = 0; k < objectLayer.objectCount; k++)
                {
                    interact_box = objectLayer.objectAt(k);
                    if(interact_box.id === component[1].id)
                        break;
                }

                let x = (interact_box.x - entity.x) / entity.width;
                let y = (interact_box.y - entity.y) / entity.height;
                let width = interact_box.width / entity.width;
                let height = interact_box.height / entity.height;

                file.writeLine("            " + stringifyFloat(x) + ",");
                file.writeLine("            " + stringifyFloat(y) + ",");
                file.writeLine("            " + stringifyFloat(width) + ",");
                file.writeLine("            " + stringifyFloat(height) + ",");

                file.writeLine("            " + component[2] + ",");
                file.writeLine("            " + component[3] + ",");
                file.writeLine("            " + component[4]);
            }
            else if(component_type == "SPRITE_COMPONENT")
            {
                file.writeLine("            \"" + component[1] + "\"");
            }
            else if(component_type == "ANIMATION_SYSTEM")
            {
                file.writeLine("            " + component[1] + ",");
                file.writeLine("            " + component[2] + ",");
                file.writeLine("            " + stringifyFloat(component[3]) + ",");
                file.writeLine("            " + component[4] + ",");
                file.writeLine("            " + component[5] + ",");
                file.writeLine("            " + component[6]);
            }
            else if(component_type == "LEVER_COMPONENT")
            {
                file.writeLine("            " + component[1] + ",");
                file.writeLine("            " + component[2]);
            }
            else if(component_type == "DOOR_COMPONENT")
            {
                file.writeLine("            " + component[1] + ",");
                file.writeLine("            " + component[2]);
            }
            else
            {
                throw new Error("Component type not supported for exporting : " + component_type);
            }

            if(component.length > 1)
                file.writeLine("          ]");

            if(j == nb_components - 1)
                file.writeLine("        }");
            else
                file.writeLine("        },");
        }

        file.writeLine("      ]");

        if(i == entities.length - 1)
            file.writeLine("    }");
        else
            file.writeLine("    },");
    }

    file.writeLine("  ]");
}

var customMapFormat = {
    name: "Custom MAP format",
    extension: "map",

    write: function(map, fileName)
    {
        let file = new TextFile(fileName, TextFile.WriteOnly);

        file.writeLine("{");
        file.writeLine("  \"MapWidth\": " + map.width + ",");
        file.writeLine("  \"MapHeight\": " + map.height + ",");
        file.writeLine("");

        for(let i = 0; i < map.layerCount; i++)
        {
            let layer = map.layerAt(i);

            if(layer.isTileLayer)
            {
                writeTileLayer(layer, file);
            }
            else if(layer.isObjectLayer)
            {
                writeEntities(layer, file);
            }
        }

        file.write("}");
        file.commit();
    },
}

tiled.registerMapFormat("custom", customMapFormat)
