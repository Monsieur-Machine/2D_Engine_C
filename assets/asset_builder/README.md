# Asset file structure

|    Description    |               Length                |
| :---------------- | :---------------------------------- |
| Assets number     | size_t (64 bits)                    |
| ...               |                                     |
| Asset name        | const char * (variable length)      |
| Asset start index | size_t (64 bits)                    |
| Asset end index   | size_t (64 bits)                    |
| ...               |                                     |
| Asset data        | depend on assets content and number |

## Image (.png)

Convert RGBA to RGB with #FF00FF as the transparent color.
If alpha null then the color of the pixel will be replaced with this.

Image data is in RGB 565.

### Image asset data structure

|    Description    |                   Length                    |
| :---------------- | :------------------------------------------ |
| Image width       | size_t (64 bits)                            |
| Image height      | size_t (64 bits)                            |
| Image data        | depend on image size, pixel short (16 bits) |



## Maps (.map)



### Maps asset data structure

|         Description         |                 Length                  |
| :-------------------------- | :-------------------------------------- |
| Map width                   | int (32 bits)                           |
| Map height                  | int (32 bits)                           |
| Map background layer 1 data | depend on map size, tile short (16 bit) |
| Map background layer 2 data | depend on map size, tile short (16 bit) |
| Map background layer 3 data | depend on map size, tile short (16 bit) |
| Map foreground layer data   | depend on map size, tile short (16 bit) |
| Entities number             | int (32 bits)                           |
| ...                         |                                         |
| Entity ID                   | size_t (64 bits)                        |
| Entity component number     | int (32 bits)                           |
|   ...                       |                                         |
|   Component type            | int (32 bits)                           |
|     ...                     |                                         |
|     Component attributes    | depend on attribute type                |
|     ...                     |                                         |
|   ...                       |                                         |
