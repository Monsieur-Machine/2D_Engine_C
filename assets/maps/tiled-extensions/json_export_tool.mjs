var customMapFormat = {
    name: "Custom map format",
    extension: "custom",

    write: function(map, fileName)
    {
        var file = new TextFile(fileName, TextFile.WriteOnly);

        file.writeLine("{");
        file.writeLine("    \"MapWidth\": " + map.width + ",");
        file.writeLine("    \"MapHeight\": " + map.height + ",");
        file.writeLine("");

        for(var i = 0; i < map.layerCount; i++)
        {
            var layer = map.layerAt(i);

            if(layer.isTileLayer)
            {
                file.writeLine("    \"" + layer.name + "\": [");

                for(var cell_y = 0; cell_y < layer.height; ++cell_y)
                {
                    file.write("       ");

                    for (var cell_x = 0; cell_x < layer.width; ++cell_x)
                    {
                        file.write(" " + layer.cellAt(cell_x, cell_y).tileId)
                        if(cell_x < layer.width - 1 || cell_y < layer.height - 1)
                            file.write(",");
                    }

                    file.writeLine("");
                }

                if(i < map.layerCount - 1)
                    file.writeLine("    ],");
                else
                    file.writeLine("    ]");
            }
        }

        file.write("}");
        file.commit();
    },
}

tiled.registerMapFormat("custom", customMapFormat)
