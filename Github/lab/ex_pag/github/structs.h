#pragma once
#define PNG_DEBUG 3
#include <png.h>
typedef struct
{
    int width;
    int height;
    png_byte color_type;
    png_byte bit_depth;
    png_structp png_ptr;
    png_infop info_ptr;
    int number_of_passes;
    png_bytep *row_pointers;
} PNG;
typedef struct
{
    int x;
    int y;
} Pixel;

typedef struct
{
    int red;
    int green;
    int blue;
    int alpha;
} Color;