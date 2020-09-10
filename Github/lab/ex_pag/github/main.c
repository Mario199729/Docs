#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <getopt.h>
#define PNG_DEBUG 3
#include <png.h>
#include "fileIO.h"
#include "structs.h"
#include "colorPixel.h"
#include "triangle.h"
#include "rectangle.h"
#include "collage.h"
/* Input check */
int isColor(Color *color, char *string)
{
    color->alpha = 255;
    if (!strcmp(string, "white"))
    {
        color->red = color->green = color->blue = 255;
        return 1;
    }
    if (!strcmp(string, "gray"))
    {
        color->red = color->green = color->blue = 150;
        return 1;
    }
    if (!strcmp(string, "black"))
    {
        color->red = color->green = color->blue = 0;
        return 1;
    }
    if (!strcmp(string, "red"))
    {
        color->red = 255;
        color->green = color->blue = 0;
        return 1;
    }

    if (!strcmp(string, "orange"))
    {
        color->red = 255;
        color->green = 125;
        color->blue = 0;
        return 1;
    }
    if (!strcmp(string, "yellow"))
    {
        color->red = color->green = 255;
        color->blue = 0;
        return 1;
    }
    if (!strcmp(string, "green"))
    {
        color->green = 255;
        color->red = color->blue = 0;
        return 1;
    }
    if (!strcmp(string, "dark-green"))
    {
        color->red = 0;
        color->green = 175;
        color->blue = 50;
        return 1;
    }
    if (!strcmp(string, "light-blue"))
    {
        color->red = 0;
        color->green = 175;
        color->blue = 255;
        return 1;
    }
    if (!strcmp(string, "blue"))
    {
        color->blue = 255;
        color->red = color->green = 0;
        return 1;
    }
    if (!strcmp(string, "purple"))
    {
        color->red = 100;
        color->green = 50;
        color->blue = 135;
        return 1;
    }
    if (!strcmp(string, "pink"))
    {
        color->red = 255;
        color->green = 0;
        color->blue = 150;
        return 1;
    }
    if (sscanf(string, "%d:%d:%d", &color->red, &color->green, &color->blue) < 3)
        return 0;
    if (color->red > 255 || color->red < 0 || color->green > 255 || color->green < 0 || color->blue > 255 || color->blue < 0)
        return 0;
    return 1;
}
/* Help text */
void printHelp()
{
    printf("\n");
    printf("\t-t --triangle [\"(<x1> <y1>) (<x2> <y2>) (<x3> <y3>)\"]\n\t\tНарисовать треугольник с координатами (x1, y1), (x2, y2) и\n\t\t(x3, y3). По умолчанию, цвет линий чёрный, толщина линии - 1.\n");
    printf("\t--line-color [<red>:<green>:<blue>] / [<name_color>]\n\t\tУстановить цвет линии, соответствующий коду RGB или названию.\n");
    printf("\t--fill-color [<red>:<green>:<blue>] / [<name_color>]\n\t\tУстановить цвет заливки, соответствующий коду RGB или названию.\n");
    printf("\t--line-depth <value>\n\t\tУстановить толщину линии.\n");
    printf("\t-r --rectangle \"(<red> <green> <blue>) (<red> <green> <blue>)\"\n\t\tПерекрасить самый большой прямоугольник, заданного цвета в\n\t\tдругой. Цвета соответствуют введённым кодам RGB.\n");
    printf("\t-c --collage <n>x<m>\n\t\tСоздать коллаж размера n на m из одной фотографии.\n");
    printf("\t-h -? --help\n\t\tПоказать эту справку и выйти.\n");
    printf("\n");
    printf("\t\tДопустимые названия цветов:\n\t\t\t\"white\" - 255:255:255;\n\t\t\t\"black\" - 0:0:0;\n\t\t\t\"gray\" - 150:150:150;\n\t\t\t\"red\" - 255:0:0;\n\t\t\t\"orange\" - 255:125:0;\n\t\t\t\"yellow\" - 255:255:0;\n\t\t\t\"green\" - 0:255:0;\n\t\t\t\"dark-green\" - 0:175:50;\n\t\t\t\"light-blue\" - 0:175:255;\n\t\t\t\"blue\" - 0:0:255;\n\t\t\t\"purple\" - 100:50:135;\n\t\t\t\"pink\" - 255:0:150.\n");
    printf("\n");
}
int main(int argc, char **argv)
{
    Pixel pix1, pix2, pix3;
    Color line = {0, 0, 0, 255};
    Color fill = {255, 255, 255, 255};
    int depth = 1;
    Color oldColor, newColor;
    int n, m;
    int flagTriangle = 0;
    int flagRectangle = 0;
    int flagCollage = 0;
    int flagFillColor = 0;
    const char *shortOptions = "t:r:c:h?";
    struct option longOptions[] = {
        {"triangle", required_argument, NULL, 't'},
        {"line-color", required_argument, NULL, 0},
        {"fill-color", required_argument, NULL, 0},
        {"line-depth", required_argument, NULL, 0},
        {"rectangle", required_argument, NULL, 'r'},
        {"collage", required_argument, NULL, 'c'},
        {"help", no_argument, NULL, 'h'},
        {NULL, 0, NULL, 0}};
    int longIndex;
    int option = getopt_long(argc, argv, shortOptions, longOptions, &longIndex);
    while (option != -1)
    {
        switch (option)
        {
        case 't':
            if (sscanf(optarg, "(%d %d) (%d %d) (%d %d)", &pix1.x, &pix1.y, &pix2.x,
                       &pix2.y, &pix3.x, &pix3.y) < 6)
            {
                fprintf(stderr, "Error: wrong input value\n");
                printHelp();
                return 0;
            }
            if (pix1.x < 0 || pix1.y < 0 || pix2.x < 0 || pix2.y < 0 || pix3.x < 0 || pix3.y < 0)
            {
                fprintf(stderr, "Error: wrong input value\n");
                return 0;
            }
            flagTriangle = 1;
            break;
        case 'r':
            if (sscanf(optarg, "(%d %d %d) (%d %d %d)", &oldColor.red,
                       &oldColor.green, &oldColor.blue, &newColor.red, &newColor.green, &newColor.blue) < 6)
            {
                fprintf(stderr, "Error: wrong input value\n");
                printHelp();
                return 0;
            }
            oldColor.alpha = newColor.alpha = 255;
            if (oldColor.red > 255 || oldColor.red < 0 || oldColor.green > 255 ||
                oldColor.green < 0 || oldColor.blue > 255 || oldColor.blue < 0 || newColor.red > 255 || newColor.red < 0 || newColor.green > 255 || newColor.green < 0 ||
                newColor.blue > 255 || newColor.blue < 0)
            {
                fprintf(stderr, "Error: wrong input value\n");
                return 0;
            }
            flagRectangle = 1;
            break;
        case 'c':
            if (sscanf(optarg, "%dx%d", &n, &m) < 2)
            {
                fprintf(stderr, "Error: wrong input value\n");
                printHelp();
                return 0;
            }
            flagCollage = 1;
            break;
        case 'h':
        case '?':
            printHelp();
            return 0;
        case 0:
            if (!strcmp(longOptions[longIndex].name, "line-color"))
            {
                if (!isColor(&line, optarg))
                {
                    fprintf(stderr, "Error: wrong input value\n");
                    printHelp();
                    return 0;
                }
                break;
            }
            if (!strcmp(longOptions[longIndex].name, "fill-color"))
            {
                flagFillColor = 1;
                if (!isColor(&fill, optarg))
                {
                    fprintf(stderr, "Error: wrong input value\n");
                    printHelp();
                    return 0;
                }
                break;
            }
            if (!strcmp(longOptions[longIndex].name, "line-depth"))
            {
                if (sscanf(optarg, "%d", &depth) < 1)
                {
                    fprintf(stderr, "Error: wrong input value\n");
                    printHelp();
                    return 0;
                }
                depth = 2 * depth - 1;
                break;
            }
            return 0;
        default:
            return 0;
        }
        option = getopt_long(argc, argv, shortOptions, longOptions, &longIndex);
    }
    argc -= optind;
    argv += optind;
    if (argc < 2)
    {
        fprintf(stderr, "Usage: ./cw <file_in> <file_out>\n");
        return 0;
    }
    PNG image;
    int colorType;
    int check;
    check = readFile(argv[0], &image);
    if (check)
        return 0;

    if (png_get_color_type(image.png_ptr, image.info_ptr) == PNG_COLOR_TYPE_RGB)
    {
        colorType = 3;
    }
    else if (png_get_color_type(image.png_ptr, image.info_ptr) ==
             PNG_COLOR_TYPE_RGBA)
    {
        colorType = 4;
    }
    else
    {
        fprintf(stderr, "Error: color_type of input file must be PNG_COLOR_TYPE_RGBA or PNG_COLOR_TYPE_RGB\n");
        check = 1;
    }
    if (!check && flagTriangle)
    {
        if (pix1.x >= image.width || pix1.y >= image.height || pix2.x >= image.width || pix2.y >= image.height || pix3.x >= image.width || pix3.y >= image.height)
        {
            fprintf(stderr, "Error: wrong input value\n");
            check = 1;
        }
        if (!check)
            drawTriangle(&image, pix1, pix2, pix3, line, fill, colorType, depth,
                         flagFillColor);
    }
    if (!check && flagRectangle)
        recolorRectangle(&image, oldColor, newColor, colorType);
    if (!check && flagCollage)
        makeCollage(&image, n, m, colorType);
    if (!check)
        check = writeFile(argv[1], &image);
    for (int i = 0; i < image.height; i++)
        free(image.row_pointers[i]);
    free(image.row_pointers);
    return 0;
}