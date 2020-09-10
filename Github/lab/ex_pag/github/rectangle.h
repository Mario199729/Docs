#pragma once
#include <stdio.h>
#define PNG_DEBUG 3
#include <png.h>

#include "structs.h"
#include "colorPixel.h"
int isRectangle(PNG *image, int x1, int y1, Color color, int colorType);
int getRectangleArea(PNG *image, int x1, int y1, int *x2, int *y2, Color rect,
int colorType);
void recolorRectangle(PNG *image, Color oldColor, Color newColor, int
colorType);