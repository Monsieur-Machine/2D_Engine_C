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
                file.writeLine("            " + entity.x + ",");
                file.writeLine("            " + entity.y + ",");
                file.writeLine("            " + entity.width + ",");
                file.writeLine("            " + entity.height);
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

                file.writeLine("            " + x + ",");
                file.writeLine("            " + y + ",");
                file.writeLine("            " + width + ",");
                file.writeLine("            " + height);
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

                file.writeLine("            " + x + ",");
                file.writeLine("            " + y + ",");
                file.writeLine("            " + width + ",");
                file.writeLine("            " + height + ",");

                file.writeLine("            " + component[2] + ",");
                file.writeLine("            " + component[3] + ",");
                file.writeLine("            " + component[4]);
            }
            else
            {
                if(component.length > 1)
                {
                    for(let k = 1; k < component.length; k++)
                    {
                        file.write("            ");
                        switch(typeof(component[k]))
                        {
                            case typeof(Number()):
                            case typeof(Boolean()):
                                file.write(component[k]);
                                break;

                            case typeof(String()):
                                file.write("\"" + component[k] + "\"");
                                break;

                            default:
                                throw new Error("Attribute \"" + component[k] + "\", type not supported : " + typeof(component[k]));
                        }

                        if(k == component.length - 1)
                            file.writeLine("");
                        else
                            file.writeLine(",");
                    }
                }
                else
                    file.writeLine("");
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
