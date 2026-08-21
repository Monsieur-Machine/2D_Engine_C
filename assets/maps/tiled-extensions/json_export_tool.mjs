function stringifyFloat(floatNumber)
{
    if(floatNumber == Math.trunc(floatNumber))
        return String(floatNumber) + ".0"

    return String(floatNumber);
}
function writeLayerContent(output_file, layer)
{
    for(let cell_y = 0; cell_y < layer.height; ++cell_y)
    {
        output_file.write(" ");

        for(let cell_x = 0; cell_x < layer.width; ++cell_x)
        {
            output_file.file.write(" " + layer.cellAt(cell_x, cell_y).tileId)
            if(cell_x < layer.width - 1 || cell_y < layer.height - 1)
                output_file.file.write(",");
        }

        output_file.file.writeLine("");
    }
}

function writeEntities(output_file, entitieslayer)
{
    let entities = [];

    for(let i = 0; i < entitieslayer.objectCount; i++)
    {
        let entity = entitieslayer.objectAt(i);

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

        output_file.writeLine("{");

        output_file.indent_add(2);
        output_file.writeLine("\"ID\":" + entity.name + ",");
        output_file.writeLine("\"Components\": [");

        let components = entity.properties()["components"];
        let nb_components = Object.keys(components).length;

        output_file.indent_add(2);

        for(let j = 0; j < nb_components; j++)
        {
            let component = components[j];
            let component_type = component[0];

            output_file.writeLine("{");

            output_file.indent_add(2);
            output_file.write("\"Type\": \"" + component_type + "\"");

            if(component.length > 1)
            {
                output_file.file.writeLine(",");
                output_file.writeLine("\"Attributes\": [");
            }

            // Write atributes, speciale cases need speciale treatment.
            if(component_type == "TRANSFORM_COMPONENT")
            {
                output_file.file.writeLine(",");
                output_file.writeLine("\"Attributes\": [");
                output_file.writeLine("  " + stringifyFloat(entity.x) + ",");
                output_file.writeLine("  " + stringifyFloat(entity.y) + ",");
                output_file.writeLine("  " + stringifyFloat(entity.width) + ",");
                output_file.writeLine("  " + stringifyFloat(entity.height));
                output_file.writeLine("]");
            }
            else if(component_type == "HITBOX_COMPONENT")
            {
                let hitbox;

                for(let k = 0; k < entitieslayer.objectCount; k++)
                {
                    hitbox = entitieslayer.objectAt(k);
                    if(hitbox.id === component[1].id)
                        break;
                }

                let x = (hitbox.x - entity.x) / entity.width;
                let y = (hitbox.y - entity.y) / entity.height;
                let width = hitbox.width / entity.width;
                let height = hitbox.height / entity.height;

                output_file.writeLine("  " + stringifyFloat(x) + ",");
                output_file.writeLine("  " + stringifyFloat(y) + ",");
                output_file.writeLine("  " + stringifyFloat(width) + ",");
                output_file.writeLine("  " + stringifyFloat(height));
            }
            else if(component_type == "INTERACT_COMPONENT")
            {
                let interact_box;

                for(let k = 0; k < entitieslayer.objectCount; k++)
                {
                    interact_box = entitieslayer.objectAt(k);
                    if(interact_box.id === component[1].id)
                        break;
                }

                let x = (interact_box.x - entity.x) / entity.width;
                let y = (interact_box.y - entity.y) / entity.height;
                let width = interact_box.width / entity.width;
                let height = interact_box.height / entity.height;

                output_file.writeLine("  " + stringifyFloat(x) + ",");
                output_file.writeLine("  " + stringifyFloat(y) + ",");
                output_file.writeLine("  " + stringifyFloat(width) + ",");
                output_file.writeLine("  " + stringifyFloat(height) + ",");

                output_file.writeLine("  " + component[2] + ",");
                output_file.writeLine("  " + component[3] + ",");
                output_file.writeLine("  " + component[4]);
            }
            else if(component_type == "SPRITE_COMPONENT")
            {
                output_file.writeLine("  \"" + component[1] + "\"");
            }
            else if(component_type == "ANIMATION_SYSTEM")
            {
                output_file.writeLine("  " + component[1] + ",");
                output_file.writeLine("  " + component[2] + ",");
                output_file.writeLine("  " + stringifyFloat(component[3]) + ",");
                output_file.writeLine("  " + component[4] + ",");
                output_file.writeLine("  " + component[5] + ",");
                output_file.writeLine("  " + component[6]);
            }
            else if(component_type == "LEVER_COMPONENT")
            {
                output_file.writeLine("  " + component[1] + ",");
                output_file.writeLine("  " + component[2]);
            }
            else if(component_type == "DOOR_COMPONENT")
            {
                output_file.writeLine("  " + component[1] + ",");
                output_file.writeLine("  " + component[2]);
            }
            else
            {
                throw new Error("Component type not supported for exporting : " + component_type);
            }

            if(component.length > 1)
                output_file.writeLine("]");

            output_file.indent_remove(2);

            if(j == nb_components - 1)
                output_file.writeLine("}");
            else
                output_file.writeLine("},");
        }

        output_file.indent_remove(2);

        output_file.writeLine("]");

        output_file.indent_remove(2);

        if(i == entities.length - 1)
            output_file.writeLine("}");
        else
            output_file.writeLine("},");
    }
}

var customMapFormat = {
    name: "Custom MAP format",
    extension: "map",

    write: function(map, fileName)
    {
        let output_file = {
            file: new TextFile(fileName, TextFile.WriteOnly),
            indent: "",
            indent_add: function(nb)
            {
                this.indent += " ".repeat(nb);
            },
            indent_remove: function(nb)
            {
                this.indent = this.indent.substring(0, this.indent.length - nb);
            },

            write: function(content)
            {
                this.file.write(this.indent);
                this.file.write(content);
            },
            writeLine: function(content)
            {
                this.file.write(this.indent);
                this.file.writeLine(content);
            },
            commit: function()
            {
                this.file.commit();
            }
        };

        output_file.file.writeLine("{");
        output_file.indent_add(2);
        output_file.writeLine("\"MapWidth\": " + map.width + ",");
        output_file.writeLine("\"MapHeight\": " + map.height + ",");
        output_file.file.writeLine("");

        let backgroundlayers = [];
        let foregroundlayer = null;
        let hitboxeslayer = null;
        let entitieslayer = null;

        for(let i = 0; i < map.layerCount; i++)
        {
            let layer = map.layerAt(i);

            if(layer.isTileLayer)
            {
                if(layer.name.startsWith("Background"))
                {
                    backgroundlayers.push(layer);
                }
                else if(layer.name.startsWith("Foreground"))
                {
                    foregroundlayer = layer;
                }
                else if(layer.name == "Hitboxes")
                {
                    if(hitboxeslayer)
                        throw new Error("There cannot be multiple hitboxes layers.");
                    hitboxeslayer = layer;
                }
                else
                {
                    throw new Error("Invalid layer name for a tile layer : " + layer.name);
                }
            }
            else if(layer.isObjectLayer)
            {
                if(layer.name == "Entities")
                {
                    if(entitieslayer)
                        throw new Error("There cannot be multiple entity layers.");

                    entitieslayer = layer;
                }
                else
                {
                    throw new Error("Invalid layer name for an object layer : " + layer.name);
                }
            }
        }

        if(backgroundlayers.length == 0)
        {
            throw new Error("At least one background layer is needed.");
        }

        if(foregroundlayer == null)
        {
            throw new Error("A foreground layer is needed.");
        }

        if(hitboxeslayer == null)
        {
            throw new Error("An hitboxes layer is needed.");
        }

        backgroundlayers.sort((a, b) => {
            if(Number(a.name[a.name.length - 1]) > Number(b.name[b.name.length - 1]))
                return 1;

            return -1;
        });

        output_file.writeLine("\"BackgroundLayers\": [");
        output_file.indent_add(2);
        for(let i = 0; i < backgroundlayers.length; i++)
        {
            const layer = backgroundlayers[i];
 
            output_file.writeLine("[");

            writeLayerContent(output_file, layer);

            if(i == backgroundlayers.length - 1)
                output_file.writeLine("]");
            else
                output_file.writeLine("],");
        };
        output_file.indent_remove(2);
        output_file.writeLine("],");

        output_file.writeLine("\"ForegroundLayer\": [");
        writeLayerContent(output_file, foregroundlayer);
        output_file.writeLine("],");

        output_file.writeLine("\"Hitboxes\": [");
        writeLayerContent(output_file, hitboxeslayer);
        output_file.writeLine("],");

        output_file.writeLine("\"Entities\": [");
        output_file.indent_add(2);
        if(entitieslayer)
            writeEntities(output_file, entitieslayer);
        output_file.indent_remove(2);
        output_file.writeLine("]");

        output_file.indent_remove(2);
        output_file.write("}");
        output_file.commit();
    }
}

tiled.registerMapFormat("custom", customMapFormat)
