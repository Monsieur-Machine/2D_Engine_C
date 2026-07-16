from tiled import *

class Json_Export_Tool(Plugin):
    @classmethod
    def nameFilter(cls):
        return "Json files (*.json)"

    @classmethod
    def shortName(cls):
        return "json"

    @classmethod
    def write(cls, tileMap, fileName):
        with open(fileName, 'w') as fileHandle:
            print("{", file=fileHandle)
            print(f'    "MapWidth": {tileMap.width()},', file=fileHandle)
            print(f'    "MapHeight": {tileMap.height()},\n', file=fileHandle)

            for i in range(tileMap.layerCount()):
                if isTileLayerAt(tileMap, i):
                    tileLayer = tileLayerAt(tileMap, i)
                    print(f'    "{tileLayer.name()}": [', file=fileHandle)
                    for cell_y in range(tileLayer.height()):
                        row = []
                        for cell_x in range(tileLayer.width()):
                            row.append(tileLayer.cellAt(cell_x, cell_y).tile().id())
                        print("        " + ", ".join(str(tile) for tile in row) + ("," if cell_y != (tileLayer.height() - 1) else ""), file=fileHandle)
                    print("    ]," if i != (tileMap.layerCount() - 1) else "    ]", file=fileHandle)
            print("}", file=fileHandle)

        return True
