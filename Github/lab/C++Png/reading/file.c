#include <stdio.h>
#include<stdlib.h>
#include<png.h>

png_structp  png_ptr;
png_infop  info_png;
int width, height, number_of_passes, x, y;
png_byte color_type;
png_byte bit_depth;
png_bytep *row_pointers;

void read_file_png( char * file_name );

int main(int argc, char** argv){

    if(argc != 3){
        fprintf(stderr, "Usage: Program  name <file in> <file out>\n");
        return 0;
    }

    read_file_png(argv[1]);

    return 0;
}

void read_file_png( char * file_name ){

    int bytes_check = 8;
    char header[bytes_check];

    FILE *fp = fopen(file_name, "rb");

    if(!fp){
        fprintf(stderr, "Thefile didn't open\n");
        exit(1);
    }

    fread(header, 1, bytes_check, fp);
    if(png_sig_cmp(header, 0, bytes_check) != 0){
        fprintf(stderr, "The file isn't a png file\n");
        exit(1);
    }

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(png_ptr == NULL){
        fprintf(stderr, "There is any problem to initialize a struct png\n");
        fclose(fp);
        exit(1);
    }

    info_png = png_create_info_struct(png_ptr);
    if(info_png == NULL){
        fprintf(stderr, "THERE IS ANY PROBLEM TO ALLOCATE A STRUT INFO PNG\n");
        fclose(fp);
        exit(1);
    }

    if(setjmp(png_jmpbuf(png_ptr))){
        fprintf(stderr, "THERE IS ANY ERROR WITH ALLOCATION/INITIALIZATION\n");
        png_destroy_read_struct(&png_ptr, &info_png, NULL);
        fclose(fp);
        exit(1);
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, bytes_check);
    png_read_info(png_ptr, info_png);
    width = png_get_image_width(png_ptr, info_png);
    height = png_get_image_height(png_ptr, info_png);
    color_type = png_get_color_type(png_ptr, info_png);
    bit_depth = png_get_bit_depth(png_ptr, info_png);
    number_of_passes = png_set_interlace_handling(png_ptr);
    png_read_update_info(png_ptr, info_png);

    if(setjmp(png_jmpbuf(png_ptr))){
        fprintf(stderr, "THERE IS ANY ERROR WITH ALLOCATION/INITIALIZATION AFTER WIDTHAND HEIGHT\n");
        png_destroy_read_struct(&png_ptr, &info_png, NULL);
        fclose(fp);
        exit(1);
    }

    row_pointers = (png_bytep*) malloc( sizeof(png_bytep) *height );

    for(y = 0; y <= height; y++)
        row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr, info_png));

    png_read_image(png_ptr, row_pointers);
    fclose(fp);

    printf("WIDTH: %i pixels\n", width);
    printf("HEIGHT: %i pixels\n", height);
    printf("Type color: %d\n", color_type);
    printf("BIT OF DEPTH: %d\n", bit_depth);
    printf("NUMBER OF PASSES: %d\n", number_of_passes);

    printf("The file passed is %s \n", file_name);
}