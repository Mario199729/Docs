#include "colorPixel.h"
void colorPixel(PNG *image, int x, int y, Color color, int colorType)
{
    png_byte *row = image->row_pointers[y];
    png_byte *ptr = &(row[x * colorType]);
    ptr[0] = color.red;
    ptr[1] = color.green;
    ptr[2] = color.blue;
    if (colorType == 4)
        ptr[3] = color.alpha;
}