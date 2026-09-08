# Asset builder description

This program builds assets into an output file.
The supported file formats are :
 - image (.png)
 - maps (.map which is in reality json)

The different files format are deduced from the extensions only.

--------

To build objects files, they are analised and written in an output file wich content depend on asset type.

Their structure will be :

|    Description    |         Length          |
| :---------------- | :-----------------------|
| Asset size        | unsigned int (32 bits)  |
| Asset data        | depends on asset type   |

#### Image (.png)

Convert png's RGBA8888 to custom RGB565 with #0001 in RGB565 as the transparent color.
If png's alpha is null then the color of the pixel will be replaced with this.

##### Image asset data structure

|    Description    |                   Length                    |
| :---------------- | :------------------------------------------ |
| Image width       | unsigned short (16 bits)                    |
| Image height      | unsigned short (16 bits)                    |
| Image data        | depend on image size, pixel short (16 bits) |

#### Maps (.map)

Convert from json to custom format.

##### Maps asset data structure

|         Description         |                        Length                         |
| :-------------------------- | :---------------------------------------------------- |
| Map width                   | unsigned short (16 bits)                              |
| Map height                  | unsigned short (16 bits)                              |
| Map background layers nb    | unsigned char (8 bits)                                |
| Map background layers data  | depends map size and layer count, tile short (16 bits)|
| Map foreground layer data   | depend on map size, tile short (16 bits)              |
| Tileset asset name          | const char [] (variable length, ends with '\00')      |
| Map hitboxes data           | depend on map size, tile unsigned char (8 bits)       |
| Entities number             | unsigned short (16 bits)                              |
| ...                         |                                                       |
| Entity ID                   | unsigned int (32 bits)                                |
| Entity components number    | unsigned short (16 bits)                              |
|   ...                       |                                                       |
|   Component type            | unsigned short (16 bits)                              |
|     ...                     |                                                       |
|     Component attributes    | depend on attribute type                              |
|     int                     | unsigned int (32 bits)                                |
|     float                   | float (32 bits)                                       |
|     bool                    | _Bool (8 bits)                                        |
|     str                     | const char [] (variable length, ends with '\00')      |
|     ...                     |                                                       |
|   ...                       |                                                       |

--------

To build asset file :

First calculate header wich consists of :

|    Description    |                      Length                       |
| :---------------- | :------------------------------------------------ |
| Assets number     | unsigned int (32 bits)                            |
| ...               |                                                   |
| Asset name        | const char [] (variable length, ends with '\00')  |
| Asset start index | unsigned int (32 bits)                            |
| ...               |                                                   |

And then write content of each input files.
