#include "collage.h"
/* Collage making */
void makeCollage(PNG *image, int n, int m, int colorType)
{
    int height = image->height;
    int width = image->width;
    png_bytep *rowPointers = (png_bytep *)malloc(height * sizeof(png_bytep));
    for (int y = 0; y < height; y++)
    {
        rowPointers[y] = (png_byte *)malloc(width * colorType * sizeof(png_byte));
        for (int x = 0; x < width * colorType; x++)
            rowPointers[y][x] = image->row_pointers[y][x];
    }
    image->height *= m;
    image->width *= n;
    image->row_pointers = (png_bytep *)calloc(image->height, sizeof(png_bytep));
    
    for (int y = 0; y < height; y++)
        image->row_pointers[y] = (png_byte *)calloc(image->width * colorType, sizeof(png_byte));
    for (int y = height; y < image->height; y++)
        image->row_pointers[y] = (png_byte *)malloc(image->width * colorType * sizeof(png_byte));
    int x = 0, y = 0;
    for (int j = 0; j < m; j++)
        for (int i = 0; i < n; i++)
            for (int y = 0; y < height; y++)
                for (int x = 0; x < width * colorType; x++)
                    image->row_pointers[y + (j * height)][x + (i * width * colorType)] = rowPointers[y][x];
    for (int y = 0; y < height; y++)
        free(rowPointers[y]);
    free(rowPointers);
}