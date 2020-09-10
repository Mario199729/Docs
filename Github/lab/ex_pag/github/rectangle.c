#include "rectangle.h"
/* Rectangle check */
int isRectangle(PNG *image, int x1, int y1, Color color, int colorType)
{
    png_byte *row, *ptr;
    if (y1 > 0)
    {
        row = image->row_pointers[y1 - 1];
        ptr = &(row[x1 * colorType]);
        if ((colorType == 4 && ptr[0] == color.red && ptr[1] == color.green && ptr[2] == color.blue && ptr[3] == color.alpha) || (colorType == 3 && ptr[0] == color.red && ptr[1] == color.green && ptr[2] == color.blue))
            return 0;
    }
    if (x1 > 0)
    {
        row = image->row_pointers[y1];
        ptr = &(row[(x1 - 1) * colorType]);
        if ((colorType == 4 && ptr[0] == color.red && ptr[1] == color.green && ptr[2] == color.blue && ptr[3] == color.alpha) || (colorType == 3 && ptr[0] == color.red && ptr[1] == color.green && ptr[2] == color.blue))
            return 0;
    }
    int x2 = x1;
    int y2 = y1;
    row = image->row_pointers[y1];

    int x = x1;
    while (x < image->width)
    {
        ptr = &(row[x * colorType]);
        if ((colorType == 4 && (ptr[0] != color.red || ptr[1] != color.green ||
                                ptr[2] != color.blue || ptr[3] != color.alpha)) ||
            (colorType == 3 && (ptr[0] !=
                                    color.red ||
                                ptr[1] != color.green || ptr[2] != color.blue)))
            break;
        x++;
    }
    x2 = x - 1;
    int y = y1;
    while (y < image->height)
    {
        row = image->row_pointers[y];
        ptr = &(row[x1 * colorType]);
        if ((colorType == 4 && (ptr[0] != color.red || ptr[1] != color.green ||
                                ptr[2] != color.blue || ptr[3] != color.alpha)) ||
            (colorType == 3 && (ptr[0] !=
                                    color.red ||
                                ptr[1] != color.green || ptr[2] != color.blue)))
            break;
        y++;
    }
    y2 = y - 1;
    for (y = y1; y <= y2; y++)
    {
        row = image->row_pointers[y];
        for (x = x1; x <= x2; x++)
        {
            ptr = &(row[x * colorType]);
            if ((colorType == 4 && (ptr[0] != color.red || ptr[1] != color.green ||
                                    ptr[2] != color.blue || ptr[3] != color.alpha)) ||
                (colorType == 3 && (ptr[0] !=
                                        color.red ||
                                    ptr[1] != color.green || ptr[2] != color.blue)))
                return 0;
        }
    }
    if (y1 > 0)
    {
        y = y1 - 1;
        row = image->row_pointers[y];
        x = x1;
        while (x <= x2)
        {
            ptr = &(row[x * colorType]);
            if ((colorType == 4 && ptr[0] == color.red && ptr[1] == color.green && ptr[2] == color.blue && ptr[3] == color.alpha) || (colorType == 3 && ptr[0] == color.red && ptr[1] == color.green && ptr[2] == color.blue))
            {
                return 0;
            }
            x++;
        }
    }
    if (x1 > 0)
    {
        x = x1 - 1;
        y = y1;
        while (y <= y2)
        {
            row = image->row_pointers[y];
            ptr = &(row[x * colorType]);
            if ((colorType == 4 && ptr[0] == color.red && ptr[1] == color.green &&
                 ptr[2] == color.blue && ptr[3] == color.alpha) ||
                (colorType == 3 && ptr[0] == color.red && ptr[1] == color.green && ptr[2] == color.blue))
            {
                return 0;
            }
            y++;
        }
    }
    if (y2 < image->height - 1)
    {
        y = y2 + 1;
        row = image->row_pointers[y];
        x = x1;
        while (x <= x2)
        {
            ptr = &(row[x * colorType]);
            if ((colorType == 4 && ptr[0] == color.red 
                                && ptr[1] == color.green &&
                 ptr[2] == color.blue && ptr[3] == color.alpha) ||
                (colorType == 3 && ptr[0] == color.red && ptr[1] == color.green && ptr[2] == color.blue))
            {
                return 0;
            }
            x++;
        }
    }
    if (x2 < image->width - 1)
    {
        x = x2 + 1;
        y = y1;
        while (y <= y2)
        {
            row = image->row_pointers[y];
            ptr = &(row[x * colorType]);
            if ((colorType == 4 && ptr[0] == color.red && ptr[1] == color.green &&
                 ptr[2] == color.blue && ptr[3] == color.alpha) ||
                (colorType == 3 && ptr[0] == color.red && ptr[1] == color.green 
                                && ptr[2] == color.blue))
            {
                return 0;
            }
            y++;
        }
    }
    return 1;
}
/* Getting the area of the rectangle */
int getRectangleArea(PNG *image, int x1, int y1, int *x2, int *y2, Color rect,
                     int colorType)
{
    png_byte *row = image->row_pointers[y1];
    png_byte *ptr;
    int x;
    int width = 0;
    for (x = x1; x < image->width; x++)
    {
        width++;
        ptr = &(row[x * colorType]);
        if ((colorType == 4 && (ptr[0] != rect.red || ptr[1] != rect.green 
                        || ptr[2] != rect.blue || ptr[3] != rect.alpha)) 
                        || (colorType == 3 && (ptr[0] != rect.red 
                        || ptr[1] != rect.green || ptr[2] != rect.blue)))
            break;
    }
    *x2 = x - 1;
    int y;
    int height = 0;
    for (y = y1; y < image->height; y++)
    {
        height++;
        row = image->row_pointers[y];
        ptr = &(row[x1 * colorType]);
        if ((colorType == 4 && (ptr[0] != rect.red || ptr[1] != rect.green 
                    || ptr[2] != rect.blue || ptr[3] != rect.alpha)) 
                    || (colorType == 3 && (ptr[0] != rect.red 
                    || ptr[1] != rect.green || ptr[2] != rect.blue)))
            break;
    }
    *y2 = y - 1;
    return (width * height);
}
/* Color change of the rectangle with the largest area */
void recolorRectangle(PNG *image, Color oldColor, Color newColor, int colorType)
{
    int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    int maxArea = 0;
    for (int y = 0; y < image->height; y++)
    {
        png_byte *row = image->row_pointers[y];
        for (int x = 0; x < image->width; x++)
        {
            png_byte *ptr = &(row[x * colorType]);
            if ((colorType == 4 && ptr[0] == oldColor.red 
                                && ptr[1] == oldColor.green 
                                && ptr[2] == oldColor.blue 
                                && ptr[3] == oldColor.alpha) || (colorType == 3 
                                && ptr[0] == oldColor.red 
                                && ptr[1] == oldColor.green 
                                && ptr[2] == oldColor.blue))
            {
                if (isRectangle(image, x, y, oldColor, colorType))
                {
                    int X, Y;
                    int area = getRectangleArea(image, x, y, &X, &Y, oldColor, colorType);
                    if (area > maxArea)
                    {
                        maxArea = area;
                        x1 = x;
                        y1 = y;
                        x2 = X;
                        y2 = Y;
                    }
                }
            }
        }
    }
    if (maxArea)
        for (int y = y1; y <= y2; y++)
            for (int x = x1; x <= x2; x++)
                colorPixel(image, x, y, newColor, colorType);
    else
        printf("No rectangle of the given color\n");
}