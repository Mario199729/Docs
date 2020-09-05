#include<stdio.h>
#include<stdlib.h>
#include<png.h>


struct PNG {
  png_structp png_ptr;
  png_infop info_ptr;  
};

void read_file(char * file_name, struct PNG  *image){

    int number = 8;
    char header[number];

    FILE *fp = fopen(file_name, "rb");

    if(!fp){
        fprintf(stderr, "File was not open\n");
    }

    fread(header, 1, number, fp);
    if(png_sig_cmp(header, 0, number) != 0){
        fprintf(stderr, "The isn't a png file\n");
        fclose(fp);
        exit(0);
    }

    image->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(image->png_ptr == NULL){
        fprintf(stderr, "Initialize struct error\n");
        fclose(fp);
        exit(0);
    }
    image->info_ptr = png_create_info_struct(image->png_ptr);

    printf("Success\n");
}

int main(int argc, char** argv){
    struct PNG image;
    if(argc != 3){
        fprintf(stderr, "Usage: fali name <file_in> <file_out>");
        exit(0);
    }

    read_file(argv[1], &image);

}