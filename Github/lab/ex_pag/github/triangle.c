#include "triangle.h"
/* Line drawing */
void drawLine(PNG *image, int x1, int y1, int x2, int y2, Color line, int colorType, int depth)
{
    int x = x1;
    int y = y1;
    int signx = x2 - x1;
    int signy = y2 - y1;
    int dx = abs(signx);
    int dy = abs(signy);
    signx = x1 < x2 ? 1 : -1;
    signy = y1 < y2 ? 1 : -1;
    int err = dx - dy;
    colorPixel(image, x2, y2, line, colorType);

    while (x != x2 || y != y2)
    {
        colorPixel(image, x, y, line, colorType);
        for (int i = y - depth / 2; i < y + depth / 2; i++)
            for (int j = x - depth / 2; j < x + depth / 2; j++)
                if (i > 0 && j > 0 && i < image->height && j < image->width)
                    colorPixel(image, j, i, line, colorType);
        
        if (err * 2 > -dy)
        {
            err -= dy;
            x += signx;
        }
        else if (err * 2 < dx)
        {
            err += dx;
            y += signy;
        }
    }
}
/* Area filling */
void fillArea(PNG *image, int x, int y, Color fill, Color line, int colorType)
{
    png_byte *row = image->row_pointers[y];
    png_byte *ptr = &(row[x * colorType]);
    if ((colorType == 4 && ptr[0] == line.red && ptr[1] == line.green 
                        && ptr[2] == line.blue && ptr[3] == line.alpha) 
                        || (colorType == 3 && ptr[0] == line.red &&
                        ptr[1] == line.green && ptr[2] == line.blue))
    {
        return;
    }
    if ((colorType == 4 && ptr[0] == fill.red && ptr[1] == fill.green 
                        && ptr[2] == fill.blue && ptr[3] == fill.alpha) 
                        || (colorType == 3 && ptr[0] == fill.red &&
                        ptr[1] == fill.green && ptr[2] == fill.blue))
    {
        return;
    }
    colorPixel(image, x, y, fill, colorType);
    fillArea(image, x + 1, y, fill, line, colorType);
    fillArea(image, x - 1, y, fill, line, colorType);
    fillArea(image, x, y + 1, fill, line, colorType);
    fillArea(image, x, y - 1, fill, line, colorType);
    fillArea(image, x + 1, y + 1, fill, line, colorType);
    fillArea(image, x + 1, y - 1, fill, line, colorType);
    fillArea(image, x - 1, y + 1, fill, line, colorType);
    fillArea(image, x - 1, y - 1, fill, line, colorType);
}
/* Triangle drawing */
void drawTriangle(PNG *image, Pixel pix1, Pixel pix2, Pixel pix3, Color line,
                  Color fill, int colorType, int depth, int flag)
{
    drawLine(image, pix1.x, pix1.y, pix2.x, pix2.y, line, colorType, depth);
    drawLine(image, pix2.x, pix2.y, pix3.x, pix3.y, line, colorType, depth);
    drawLine(image, pix3.x, pix3.y, pix1.x, pix1.y, line, colorType, depth);
    int x = (((pix1.x + pix2.x) / 2) + pix3.x) / 2;
    int y = (((pix1.y + pix2.y) / 2) + pix3.y) / 2;
    if (flag)
        fillArea(image, x, y, fill, line, colorType);
}