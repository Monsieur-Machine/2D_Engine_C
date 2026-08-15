# Asset file structure

|    Description    |                      Length                       |
| :---------------- | :------------------------------------------------ |
| Assets number     | unsigned int (32 bits)                            |
| ...               |                                                   |
| Asset name        | const char [] (variable length, ends with '\00')  |
| Asset start index | unsigned int (32 bits)                            |
| ...               |                                                   |
| Asset data        | depend on assets content and number               |

## Image (.png)

Convert RGBA to RGB with #FF00FF as the transparent color.
If alpha null then the color of the pixel will be replaced with this.

Image data is in RGB 565.

### Image asset data structure

|    Description    |                   Length                    |
| :---------------- | :------------------------------------------ |
| Image width       | unsigned int (32 bits)                      |
| Image height      | unsigned int (32 bits)                      |
| Image data        | depend on image size, pixel short (16 bits) |



## Maps (.map)



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
