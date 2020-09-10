#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#define PNG_DEBUG 3
#include <png.h>



struct Png{
    int width, height;
    png_byte color_type;
    png_byte bit_depth;

    png_structp png_ptr;
    png_infop info_ptr;
    int number_of_passes;
    png_bytep *row_pointers;
};


void read_png_file(char *file_name, struct Png *image) {
    int x,y;
    char header[8];    // 8 is the maximum size that can be checked

    /* open file and test for it being a png */
    FILE *fp = fopen(file_name, "rb");
    if (!fp){
        // Some error handling: file could not be opened
    }
    

    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8)){
        // Some error handling: file is not recognized as a PNG
    }

    /* initialize stuff */
    image->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!image->png_ptr){
        // Some error handling: png_create_read_struct failed
    }

    image->info_ptr = png_create_info_struct(image->png_ptr);
    
    if (!image->info_ptr){
        // Some error handling: png_create_info_struct failed
    }

    if (setjmp(png_jmpbuf(image->png_ptr))){
        // Some error handling: error during init_io
    }

    png_init_io(image->png_ptr, fp);
    png_set_sig_bytes(image->png_ptr, 8);

    png_read_info(image->png_ptr, image->info_ptr);

    image->width = png_get_image_width(image->png_ptr, image->info_ptr);
    image->height = png_get_image_height(image->png_ptr, image->info_ptr);
    image->color_type = png_get_color_type(image->png_ptr, image->info_ptr);
    image->bit_depth = png_get_bit_depth(image->png_ptr, image->info_ptr);

    image->number_of_passes = png_set_interlace_handling(image->png_ptr);
    png_read_update_info(image->png_ptr, image->info_ptr);

    /* read file */
    if (setjmp(png_jmpbuf(image->png_ptr))){
        // Some error handling: error during read_image
    }

    
    image->row_pointers = (png_bytep *) malloc(sizeof(png_bytep) * image->height);
    for (y = 0; y < image->height; y++)
        image->row_pointers[y] = (png_byte *) malloc(png_get_rowbytes(image->png_ptr, image->info_ptr));

    png_read_image(image->png_ptr, image->row_pointers);

    fclose(fp);
}


void write_png_file(char *file_name, struct Png *image) {
    int x,y;
    /* create file */
    FILE *fp = fopen(file_name, "wb");
    if (!fp){
        // Some error handling: file could not be opened
    }

    /* initialize stuff */
    image->png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!image->png_ptr){
        // Some error handling: png_create_write_struct failed
    }

    image->info_ptr = png_create_info_struct(image->png_ptr);
    if (!image->info_ptr){
        // Some error handling: png_create_info_struct failed
    }

    if (setjmp(png_jmpbuf(image->png_ptr))){
        // Some error handling: error during init_io
    }

    png_init_io(image->png_ptr, fp);


    /* write header */
    if (setjmp(png_jmpbuf(image->png_ptr))){
        // Some error handling: error during writing header
    }

    png_set_IHDR(image->png_ptr, image->info_ptr, image->width, image->height,
                 image->bit_depth, image->color_type, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(image->png_ptr, image->info_ptr);


    /* write bytes */
    if (setjmp(png_jmpbuf(image->png_ptr))){
        // Some error handling: error during writing bytes
    }

    png_write_image(image->png_ptr, image->row_pointers);


    /* end write */
    if (setjmp(png_jmpbuf(image->png_ptr))){
        // Some error handling: error during end of write
    }

    png_write_end(image->png_ptr, NULL);

    /* cleanup heap allocation */
    for (y = 0; y < image->height; y++)
        free(image->row_pointers[y]);
    free(image->row_pointers);

    fclose(fp);
}

void process_file(struct Png *image) {
    int x,y;
    if (png_get_color_type(image->png_ptr, image->info_ptr) == PNG_COLOR_TYPE_RGB){
        // Some error handling: input file is PNG_COLOR_TYPE_RGB but must be PNG_COLOR_TYPE_RGBA
    }
    
    if (png_get_color_type(image->png_ptr, image->info_ptr) != PNG_COLOR_TYPE_RGBA){
        // Some error handling: color_type of input file must be PNG_COLOR_TYPE_RGBA
    }

    for (y = 0; y < image->height; y++) {
        png_byte *row = image->row_pointers[y];
        for (x = 0; x < image->width; x++) {
             png_byte *ptr = &(row[x * 4]);
	     
	     printf("Pixel at position [ %d - %d ] has RGBA values: %d - %d - %d - %d\n",
	     x, y, ptr[0], ptr[1], ptr[2], ptr[3]);
      	     //set red value to 0 and green value to the blue one
	     ptr[0] = 0;
	     ptr[1] = ptr[2];
        }
    }
}

void setColors(png_byte *ptr, int rgba[]){
    ptr[0] = rgba[0];
    ptr[1] = rgba[1];
    ptr[2] = rgba[2];
    ptr[3] = rgba[3];
}

void write_triangle(struct Png *image, int rgba[], int rgba_line[], char fill){

	int x, y, pixels = 8;
	
	if (png_get_color_type(image->png_ptr, image->info_ptr) == PNG_COLOR_TYPE_RGB){
        	// Some error handling: input file is PNG_COLOR_TYPE_RGB but must be 			PNG_COLOR_TYPE_RGBA
    }
    
    if (png_get_color_type(image->png_ptr, image->info_ptr) != PNG_COLOR_TYPE_RGBA){
    	// Some error handling: color_type of input file must be PNG_COLOR_TYPE_RGBA
    }

    for(y = 0; y < image->height; y++){
        png_byte *row = image->row_pointers[y];
        for (x = 0; x < image->width; x++){
            png_byte *ptr = &(row[x * 4]);
            if(y <= 4){
                if((x >= (int)(image->width/2 - y)) && (x <= (int)(image->width/2 + y)) &&  ((int)(image->width/2) - y >= 0)){
                    setColors(ptr, rgba_line);
                }
            }else{
                if(fill == 'y' || fill == 'Y'){
                    if((x >= (int)(image->width/2 - y + pixels)) && (x <= (int)(image->width/2 + y -pixels)) &&  ((int)(image->width/2) - y >= 0)){
                        setColors(ptr, rgba);
                    }else{
                        if((x >= (int)(image->width/2 - y)) && (x <= (int)(image->width/2 - y + pixels))){
                            setColors(ptr, rgba_line);				
                        }
                        if((x >= (int)(image->width/2 + y - pixels)) && (x <= (int)(image->width/2 + y))){
                            setColors(ptr, rgba_line);					
                        }
                        if(((int)(image->width/2) - y <= 0)&&((int)(image->width/2) - y >= -pixels)){
                            setColors(ptr, rgba_line);					
                        }
                    }
                }else{
                    if((x >= (int)(image->width/2 - y)) && (x <= (int)(image->width/2 - y + pixels))){
                        setColors(ptr, rgba_line);				
                    }
                    if((x >= (int)(image->width/2 + y - pixels)) && (x <= (int)(image->width/2 + y))){
                        setColors(ptr, rgba_line);					
                    }
                    if(((int)(image->width/2) - y <= 0)&&((int)(image->width/2) - y >= -pixels)){
                        setColors(ptr, rgba_line);					
                    }
                }		
            }
        }
    }
}

void write_rectangle(struct Png *image, int rgba[], int rgba_rectangle[]){ 
	int x, y, pixels = 8, count = 0, k = 0;
    char  fill = 'y';
	
	if (png_get_color_type(image->png_ptr, image->info_ptr) == PNG_COLOR_TYPE_RGB){
        
    }
    
    if (png_get_color_type(image->png_ptr, image->info_ptr) != PNG_COLOR_TYPE_RGBA){
    	// Some error handling: color_type of input file must be PNG_COLOR_TYPE_RGBA
    }
	for(int h = (int)image->width/2; h >= 0; h--, count++);
    
    for(y = 0; y < image->height; y++){
        png_byte *row = image->row_pointers[y];
        for (x = 0; x < image->width; x++){
            png_byte *ptr = &(row[x * 4]);
            if(y <= 4){
                if((x >= (int)(image->width/2 - y)) && (x <= (int)(image->width/2 + y)) &&  ((int)(image->width/2) - y >= 0)){
                    setColors(ptr, rgba);
                }
            }else{
                if(fill == 'y' || fill == 'Y'){
                    if((x >= (int)(image->width/2 - y)) && (x <= (int)(image->width/2 + y)) &&  ((int)(image->width/2) - y >= 0)){
                        setColors(ptr, rgba);
                    }
                }		
            }
        }
    }
    
    int control = 0; 
	for (y = 0, k = (int) image->width/2; y < image->height; y++, k--) {
        png_byte *row = image->row_pointers[y];
        for (x = 0; x < image->width; x++) {
            png_byte *ptr = &(row[x * 4]);
	     	
	     	if(y >= (int)count/2){
	     	
	     		if(control == 0){
	     		
	     		if(fill == 'Y' || fill == 'y')
	     			control = k;
	     		else
	     			control = k + pixels;
	     		}
	     			
	     		if((x > control) && (x < (image->width - control)) && y <= (count-1)){
			 		setColors(ptr, rgba_rectangle);
			 	}
			}
		}
	}
}

void colors(int *rgba, int color){
	switch(color){
		case 0:
            rgba[0] = 255;
            rgba[1] = 0;
            rgba[2] = 0;
            rgba[3] = 255;
            break;
		     
		case 1:
            rgba[0] = 0;
            rgba[1] = 255;
            rgba[2] = 0;
            rgba[3] = 255;
            break;
		     
		case 2:
            rgba[0] = 0;
            rgba[1] = 0;
            rgba[2] = 255;
            rgba[3] = 255;
            break;     
	}
}

void makeCollage(struct Png *image, int n, int m, int colorType)
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

int main(int argc, char **argv) {

    if (argc != 3){
        fprintf(stderr,"Usage: program_name <file_in> <file_out>\n");
	return 0;
    }

    char fill;
    int color, color_line, color_rectangle, op;
    int *rgba = calloc(4, sizeof(int));
    int *rgba_line = calloc(4, sizeof(int));
    int *rgba_rectangle = calloc(4, sizeof(int));

    struct Png image;
    read_png_file(argv[1], &image);
    makeCollage(&image, 2, 3, 4);
        write_png_file(argv[2], &image);
    return 0;
    
    printf("\t========== Welcome to our programm ==========\n");
    printf("\t\t[1] -- To write triangle into photo\n");
    printf("\t\t[2] -- To write rectangle into photo\n");
    printf("\t\t[3] -- To show a normal photo\n");
    //printf("\t\t[4] -- TO collage\n");
    printf("\t\t[0] -- To shot down the programm\n");
    
    do{
        printf("\t\tType your option: ");
        scanf("%d", &op);

        if((op != 0) && (op != 1) && (op != 2) && (op != 3))
            printf("\nInvalid option, try again.\n");

    }while((op != 0) && (op != 1) && (op != 2) && (op != 3));

    switch(op){
    	case 1:
            do {
    			printf("\nColor line\n");
    			printf("[0] <--> for red\n");
    			printf("[1] <--> for green\n");
    			printf("[2] <--> for blue\n");
    			scanf("%d", &color_line);
    			
	    		if(color_line != 0 && color_line != 1 && color_line != 2)
	    			printf("Invalid option! try again.\n");
    			
    		}while(color_line != 0 && color_line != 1 && color_line != 2);
            colors(rgba_line, color_line);

    		do {
    			printf("Do you want to fill the triangle?[y/n]: ");
	    		scanf("%c", &fill);

	    		if(fill != 'y' && fill != 'Y' && fill != 'n' && fill != 'N')
	    			printf("Invalid option! try again.\n");
	    		
    		}while(fill != 'y' && fill != 'Y' && fill != 'n' && fill != 'N'); 
    		if(fill == 'y' || fill == 'Y'){
                do {
                    printf("\n\nWhich color do want to fill the traingle?\n");
                    printf("[0] <--> for red\n");
                    printf("[1] <--> for green\n");
                    printf("[2] <--> for blue\n");
                    scanf("%d", &color);
                    
                    if(color != 0 && color != 1 && color != 2)
                        printf("Invalid option! try again.\n\n");
                    
                }while(color != 0 && color != 1 && color != 2);
                colors(rgba, color);
            }
            write_triangle(&image, rgba, rgba_line, fill);
            write_png_file(argv[2], &image);
            break;
    	     
    	 case 2:
    	 	do {
    			printf("\nIn which color do you want to find a rectangle?\n");
    			printf("\t[0] <--> for red\n");
    			printf("\t[1] <--> for green\n");
    			printf("\t[2] <--> for blue\n");
    			scanf("%d", &color);
    			
	    		if(color != 0 && color != 1 && color != 2)
	    			printf("Invalid option! try again.\n");
	    		
    		}while(color != 0 && color != 1 && color != 2);
    		colors(rgba, color);
    		
    		do {
    			printf("\nWhich color do you want to fill a rectangle?\n");
    			printf("\t[0] <--> for red\n");
    			printf("\t[1] <--> for green\n");
    			printf("\t[2] <--> for blue\n");
    			scanf("%d", &color_rectangle);
    			
	    		if(color_rectangle != 0 && color_rectangle != 1 && color_rectangle != 2)
	    			printf("Invalid option! try again.\n");
	    		
    		}while(color_rectangle != 0 && color_rectangle != 1 && color_rectangle != 2);
    		colors(rgba_rectangle, color_rectangle);
    	
			write_rectangle(&image, rgba, rgba_rectangle);
			write_png_file(argv[2], &image);
			break;
    	    
        /*case 4:
            collage(&image, 2, 2);
            write_png_file(argv[2], &image);
            break; */
            
        case 3:
            process_file(&image);
            write_png_file(argv[2], &image);
            break;
            
    	case 0:
            printf("bye, bye!\n"); 
            break;
    }
    return 0;
}
