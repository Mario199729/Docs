#include<stdio.h>
#include<stdlib.h>
#include<png.h>


png_structp png_ptr;
png_infop info_ptr;
int width, height, y, x, number_of_passes;
png_byte color_type, bit_depth;
png_bytep * row_pointers;

void read_file_name(char * filaname);

int main(int argc, char **argv){

    if(argc != 3 ){
        fprintf(stderr, "Usage: Program name <file in> <file out>\n");
        return 0;
    }

    read_file_name(argv[1]);

    return 0;
}
void read_file_name(char* filename){

    int number = 8;
    char header[number];

    FILE * fp = fopen(filename, "rb");

    fread(header, 1, number, fp);

    if(png_sig_cmp(header, 0, number) != 0){
        fprintf(stderr, "THE FILE ISN'T A PNG\n");
        fclose(fp);
        exit(1);
    }

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    info_ptr = png_create_info_struct(png_ptr);

    if(png_ptr == NULL){
        fprintf(stderr, "ERROR TRIENING TO INITIALIZE STRUCT PNG_PTR\n");
        fclose(fp);
        exit(1);
    }

    if(info_ptr == NULL){
        fprintf(stderr, "ERROR TRIENING TO ALLOCATE INFO ABOUT STRUCT\n");
        fclose(fp);
        exit(1);
    }

    if(setjmp(png_jmpbuf(png_ptr))){
        fprintf(stderr, "ERROR TRIENING TO INICIATIZE/ALLOCATE STRUCT\n");
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        exit(1);
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, number);
    png_read_info(png_ptr,info_ptr);
    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);
    number_of_passes = png_set_interlace_handling(png_ptr);
    png_read_update_info(png_ptr, info_ptr);

    if(setjmp(png_jmpbuf(png_ptr))){
        fprintf(stderr, "ERROR AFTER PNG_READ_UPDATE_INFO \n");
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        exit(1);
    }

    row_pointers = (png_bytep*) malloc(sizeof(png_bytep)* height);

    for(y = 0; y < height; y++)
        row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr, info_ptr));

    png_read_image(png_ptr, row_pointers);
    fclose(fp);        
    printf("WIDTH: %i\n", width);
    printf("HEIGHT: %i\n", height);
    printf("COLOR TYPE: %i\n", color_type);
    printf("BIT DEPTH: %i\n", bit_depth);
    printf("NUMBER OF PASSES: %i\n", number_of_passes);
}