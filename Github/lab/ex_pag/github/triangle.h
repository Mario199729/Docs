#pragma once
#include <stdlib.h>
#define PNG_DEBUG 3
#include <png.h>
#include "structs.h"
#include "colorPixel.h"
void drawLine(PNG *image, int x1, int y1, int x2, int y2, Color line, int colorType, int depth);
void fillArea(PNG *image, int x, int y, Color fill, Color line, int colorType);
void drawTriangle(PNG *image, Pixel pix1, Pixel pix2, Pixel pix3, Color line, Color fill, 
                int colorType, int depth, int flag);