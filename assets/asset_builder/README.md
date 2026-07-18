# Asset file structure

|    Description    |               Length                |
| :---------------- | :---------------------------------- |
| Assets number     | int (32 bits)                       |
| ...               |                                     |
| Asset name        | const char * (variable length)      |
| Asset start index | int (32 bits)                       |
| Asset end index   | int (32 bits)                       |
| ...               |                                     |
| Asset data        | depend on assets content and number |

## PNG

Convert RGBA to RGB with #FF00FF as the transparent color.
If alpha null then the color of the pixel will be replaced with this.