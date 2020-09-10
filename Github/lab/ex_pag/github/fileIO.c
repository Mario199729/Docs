#include "fileIO.h"
/* Input file reading */
int readFile(char *nameFile, PNG *image)
{
    char header[8];
    FILE *file = fopen(nameFile, "rb");
    if (!file)
    {
        fprintf(stderr, "Error: %s could not be opened\n", nameFile);
        return 1;
    }
    fread(header, 1, 8, file);
    if (png_sig_cmp(header, 0, 8))
    {
        fprintf(stderr, "Error: %s is not a recognised as PNG file\n", nameFile);
        return 1;
    }
    image->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!image->png_ptr)
    {
        fprintf(stderr, "Error: png_create_read_struct failed\n");
        return 1;
    }
    image->info_ptr = png_create_info_struct(image->png_ptr);
    if (!image->info_ptr)
    {
        fprintf(stderr, "Error: png_create_info_struct failed\n");
        return 1;
    }
    if (setjmp(png_jmpbuf(image->png_ptr)))
    {
        fprintf(stderr, "Error during init_io\n");
        return 1;
    }

    png_init_io(image->png_ptr, file);
    png_set_sig_bytes(image->png_ptr, 8);
    png_read_info(image->png_ptr, image->info_ptr);
    image->width = png_get_image_width(image->png_ptr, image->info_ptr);
    image->height = png_get_image_height(image->png_ptr, image->info_ptr);
    image->color_type = png_get_color_type(image->png_ptr, image->info_ptr);
    image->bit_depth = png_get_bit_depth(image->png_ptr, image->info_ptr);
    image->number_of_passes = png_set_interlace_handling(image->png_ptr);
    png_read_update_info(image->png_ptr, image->info_ptr);
    if (setjmp(png_jmpbuf(image->png_ptr)))
    {
        fprintf(stderr, "Error during read_image\n");
        return 1;
    }
    image->row_pointers = (png_bytep *)malloc(image->height * sizeof(png_bytep));
    for (int y = 0; y < image->height; y++)
        image->row_pointers[y] = (png_byte *)malloc(png_get_rowbytes(image -> png_ptr, image->info_ptr));
    png_read_image(image->png_ptr, image->row_pointers);
    fclose(file);
    return 0;
}
/* Output file writing */
int writeFile(char *nameFile, PNG *image)
{
    FILE *file = fopen(nameFile, "wb");
    if (!file)
    {
        fprintf(stderr, "Error: %s could not be opened\n", nameFile);
        return 1;
    }
    image->png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL,
                                             NULL);
    if (!image->png_ptr)
    {
        fprintf(stderr, "Error: png_create_read_struct failed\n");
        return 1;
    }
    image->info_ptr = png_create_info_struct(image->png_ptr);
    if (!image->info_ptr)
    {
        fprintf(stderr, "Error: png_create_info_struct failed\n");
        return 1;
    }
    if (setjmp(png_jmpbuf(image->png_ptr)))
    {
        fprintf(stderr, "Error: error during init_io\n");
        return 1;
    }
    png_init_io(image->png_ptr, file);
    /* write header */
    if (setjmp(png_jmpbuf(image->png_ptr)))
    {
        fprintf(stderr, "Error: error during writing header\n");
        return 1;
    }
    png_set_IHDR(image->png_ptr, image->info_ptr, image->width, image->height,
                 image->bit_depth, image->color_type, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_write_info(image->png_ptr, image->info_ptr);
    /* write bytes */
    if (setjmp(png_jmpbuf(image->png_ptr)))
    {
        fprintf(stderr, "Error: error during writing bytes\n");
        return 1;
    }
    png_write_image(image->png_ptr, image->row_pointers);
    /* end write */
    if (setjmp(png_jmpbuf(image->png_ptr)))
    {
        fprintf(stderr, "Error: error during end of write\n");
        return 1;
    }
    png_write_end(image->png_ptr, NULL);
    fclose(file);
    return 0;
}