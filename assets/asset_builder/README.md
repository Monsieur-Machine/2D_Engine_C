# Asset builder description

This program builds assets into an output file.
The supported file formats are :
 - image (.png)
 - maps (.map which is in reality json)

The different files format are deduced from the extensions only.

The code has some optimisation problems but here is the main idea :

First calculate header wich consists of :

|    Description    |                      Length                       |
| :---------------- | :------------------------------------------------ |
| Assets number     | unsigned int (32 bits)                            |
| ...               |                                                   |
| Asset name        | const char [] (variable length, ends with '\00')  |
| Asset start index | unsigned int (32 bits)                            |
| ...               |                                                   |

This calculation needs to analse the content of inputs files to calculate the start offsets in the file.

Then write the content of each file in the same order as they were analysed for header calculations.

For the moment each input file is openend and analized 2 times.

## Image (.png)

Convert png's RGBA8888 to custom RGB565 with #FF00FF (or #F81F in RGB565) as the transparent color.
If png's alpha is null then the color of the pixel will be replaced with this.

### Image asset data structure

|    Description    |                   Length                    |
| :---------------- | :------------------------------------------ |
| Image width       | unsigned int (32 bits)                      |
| Image height      | unsigned int (32 bits)                      |
| Image data        | depend on image size, pixel short (16 bits) |

## Maps (.map)

Convert from json to custom format.

### Maps asset data structure

|         Description         |                      Length                       |
| :-------------------------- | :------------------------------------------------ |
| Map width                   | unsigned int (32 bits)                            |
| Map height                  | unsigned int (32 bits)                            |
| Map background layer 1 data | depend on map size, tile short (16 bits)          |
| Map background layer 2 data | depend on map size, tile short (16 bits)          |
| Map background layer 3 data | depend on map size, tile short (16 bits)          |
| Map foreground layer data   | depend on map size, tile short (16 bits)          |
| Map hitboxes data           | depend on map size, tile unsigned char (8 bits)   |
| Entities number             | unsigned int (32 bits)                            |
| ...                         |                                                   |
| Entity ID                   | unsigned int (32 bits)                            |
| Entity components number    | unsigned int (32 bits)                            |
|   ...                       |                                                   |
|   Component type            | int (32 bits)                                     |
|     ...                     |                                                   |
|     Component attributes    | depend on attribute type                          |
|     int                     | int (32 bits)                                     |
|     float                   | float (32 bits)                                   |
|     bool                    | bool (8 bits)                                     |
|     str                     | const char [] (variable length, ends with '\00')  |
|     ...                     |                                                   |
|   ...                       |                                                   |
