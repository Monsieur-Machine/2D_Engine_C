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

## PNG

Convert RGBA to RGB with #FF00FF as the transparent color.
If alpha null then the color of the pixel will be replaced with this.

### Image asset data structure

|    Description    |               Length                |
| :---------------- | :---------------------------------- |
| Image width       | size_t (64 bits)                    |
| Image height      | size_t (64 bits)                    |
| Image data        | depend on image content             |
