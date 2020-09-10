#pragma once
#define PNG_DEBUG 3
#include <png.h>
#include "structs.h"
void colorPixel(PNG *image, int x, int y, Color color, int colorType);