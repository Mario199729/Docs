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

void DrawlineTriangle(struct Png *image, int rgba_line[], int x0, int y0, int x1, int y1, int typecolor);
void write_triangle(struct Png *image, int rgba[], int rgba_line[], char fill);
void write_rectangle(struct Png *image, int rgba[], int rgba_rectangle[]);
void makeCollage(struct Png *image, int n, int m);
void setColors(png_byte *ptr, int rgba[]);
void colors(int *rgba, int color);
int chooseColor();


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


/*void write_triangle(struct Png *image, int rgba[], int rgba_line[], char fill){

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
}*/

//***************************************


void DrawlineTriangle(struct Png *image, int rgba_line[], int x0, int y0, int x1, int y1, int typecolor){
    int j = x0,i = y0;
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int controlx = (x0 < x1)? 1:-1;
    int controly = (y0 < y1)? 1:-1;
    int err = dx - dy;
    int pixels = 2;
    printf("foca de x0 = %d y0 = %d \nx2 = %d y2 = %d\n",x0,y0,x1,y1);
    for( ;(j != x1 || i != y1); )
    { 
        printf("valor de i = %d j = %d \n",i,j);   
        png_byte*row  = image->row_pointers[i];
        png_byte *ptr = &(row[j * typecolor]);
        setColors(ptr, rgba_line);
        for (int k = (i - pixels); k < (i + pixels); k++){
            row  = image->row_pointers[k];
            for (int l = (j - pixels); l < (j + pixels); l++){
                ptr = &(row[l * typecolor]);
                setColors(ptr, rgba_line);
            }
        }
        if (err * 2 > -dy)
        {
            err -= dy;
            j += controlx;
        }
        else if (err * 2 < dx)
        {
            err += dx;
            i += controly;
        }
    }
}

/*
void DrawlineTriangle(struct Png *image, int rgba_line[], int x0, int y0, int x1, int y1, int typecolor){
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int aux ,auy,aum;
    if(y0 > y1){
        y0 = y0 + y1;
        y1 = y0 -y1;
        y0 = y0 - y1;
        x0 = x0 + x1;
        x1 = x0 -x1;
        x0 = x0 - x1;        
    }
    if(x0 > x1)
        aum = -1;
    else
        aum = 1;

    if (dx >= dy){
        aux = (int)dx/dy;
        auy = 1;
    }else{
        auy = (int)dy/dx;
        aux = 1;
    }

    printf("X0 = %d Y0 = %d X1 = %d Y1 = %d\n",x0,y0,x1,y1);
    printf("Aux = %d auY = %d \n",aux,auy);
    int x = x0, terx = x0;
    for(int y = y0 ; y <= y1; y++){
        png_byte *row  = image->row_pointers[y];
        png_byte *ptr = &(row[x * typecolor]);
        setColors(ptr, rgba_line);        
        if(y % auy == 0){
            if(aum == 1){
                terx += aux;
                for( x =  terx - aux;  x<= terx; x++){
                    row  = image->row_pointers[y];
                    ptr = &(row[x * typecolor]);
                    setColors(ptr, rgba_line);   
                }
            }else{
                terx -= aux;
                for( x =  terx + aux;  x >= terx; x--){
                    row  = image->row_pointers[y];
                    ptr = &(row[x * typecolor]);
                    setColors(ptr, rgba_line);    
                }                 
            }
        }
    }

}*/
void organizar(int *x0, int *y0, int *x1, int *y1){
    *x0 += *x1;
    *x1 = *x0 - *x1;
    *x0 = *x0 - *x1;

    *y0 += *y1;
    *y1 = *y0 - *y1;
    *y0 = *y0 - *y1;      

}
void Filltriangle(struct Png *image, int rgba[], int x0, int y0, int x1, int y1, int x2, int y2, int typecolor)
{
    int hmax = 0;
    if (x0 >= x1 && x0 >= x2)
    {
        if (x1 >= x2)
            organizar(&x1, &y1, &x0, &y0);
        else
            organizar(&x2, &y2, &x0, &y0);
    }
    else if (x1 >= x0 && x1 >= x2)
    {
        if (x2 >= x0)
            organizar(&x2, &y2, &x0, &y0);
    }
    else if (x1 >= x0)
        organizar(&x1, &y1, &x0, &y0);

    if (y0 >= y1 && y0 >= y2)
    {
        if (y1 <= y2)
            organizar(&x1, &y1, &x2, &y2);
    }
    else if (y1 >= y0 && y1 >= y2)
        organizar(&x2, &y2, &x1, &y1);

        int j = x0, i = y0;
        int dx = abs(x1 - x0), dy = abs(y1 - y0);
        int controlx = (x0 < x1) ? 1 : -1, controly = (y0 < y1) ? 1 : -1;
        int err = dx - dy;

        printf("dentro de x0 = %d y0 = %d \nx2 = %d y2 = %d\n", x0, y0, x2, y2);
        int j1 = x0, i1 = y0;
        int dx1 = abs(x2 - x0), dy1 = abs(y2 - y0);
        int controlx1 = (x0 < x2) ? 1 : -1, controly1 = (y0 < y2) ? 1 : -1;
        int err1 = dx1 - dy1;

        int k = 0, aum = 0, ter = 0, k1 = 0, k3 = 0;
        int cont = 0;
        for (; (i != y1 || j != x1);)
        {
            if (j > j1)
                aum = -1;
            else
                aum = 1;
            k = j;
            //printf("valor de i1 = %d j1 = %d i2 = %d\n",i1,j1);
            //printf("valor de i = %d i1 = %d i2 = %d\n",i,i1,i2);
            while (k != j1)
            {
                png_byte *row = image->row_pointers[i];
                png_byte *ptr = &(row[k * typecolor]);
                setColors(ptr, rgba);
                k = k + aum;
            }
            if (err * 2 > -dy)
            {
                err -= dy;
                j += controlx;
            }
            else if (err * 2 < dx)
            {
                err += dx;
                i += controly;
            }
            //media
            if (err1 * 2 > -dy1)
            {
                err1 -= dy1;
                j1 += controlx1;
            }
            else if (err1 * 2 < dx1)
            {
                err1 += dx1;
                i1 += controly1;
            }
        }

        //********************************************************/
        
        organizar(&x0, &y0, &x2, &y2);
        j = x0, i = y0;
        dx = abs(x1 - x0), dy = abs(y1 - y0);
        controlx = (x0 < x1) ? 1 : -1, controly = (y0 < y1) ? 1 : -1;
        err = dx - dy;

       // organizar(&x0, &y0, &x2, &y2);
         j1 = x0, i1 = y0;
         dx1 = abs(x2 - x0), dy1 = abs(y2 - y0);
         controlx1 = (x0 < x2) ? 1 : -1, controly1 = (y0 < y2) ? 1 : -1;
         err1 = dx1 - dy1;
        for (; (i != y1 || j != x1);){
            if (j > j1)
                aum = -1;
            else
                aum = 1;
            k = j;
            //k1 = i;
            //for(int d = 0 ;d <2 ;d++){
            while (k != j1 ){
                png_byte *row = image->row_pointers[i1];
                png_byte *ptr = &(row[k * typecolor]);
                setColors(ptr, rgba);
                k = k + aum;
            //}k1 = i1;
            //k = j;
            }
            if (err * 2 > -dy){
                err -= dy;
                j += controlx;
            }else if (err * 2 < dx){
                err += dx;
                i += controly;
            }
            //media
            if (err1 * 2 > -dy1){
                err1 -= dy1;
                j1 += controlx1;
            }
            else if (err1 * 2 < dx1){
                err1 += dx1;
                i1 += controly1;
            }
        }        
    }

    void write_triangle(struct Png * image, int rgba[], int rgba_line[], char fill)
    {
        int typecolor;

        /*20 400) (350 550) (250 130*/
        int x0 = 20, y0 = 400;
        int x1 = 350, y1 = 550;
        int x2 = 250, y2 = 130;

        if (png_get_color_type(image->png_ptr, image->info_ptr) == PNG_COLOR_TYPE_RGB)
            typecolor = 3;

        if (png_get_color_type(image->png_ptr, image->info_ptr) == PNG_COLOR_TYPE_RGBA)
            typecolor = 4;

         DrawlineTriangle(image, rgba_line, x0, y0, x1, y1, typecolor);
         DrawlineTriangle(image, rgba_line, x1, y1, x2, y2, typecolor);
        DrawlineTriangle(image, rgba_line, x0, y0, x2, y2, typecolor);

        if (fill == 'Y' || fill == 'y')
        {
            Filltriangle(image, rgba, x0, y0, x1, y1, x2, y2, typecolor);
        };
    }
    //***************************************

    void write_rectangle(struct Png * image, int rgba[], int rgba_rectangle[])
    {
        int x, y, pixels = 8, count = 0, k = 0;
        char fill = 'y';

        if (png_get_color_type(image->png_ptr, image->info_ptr) == PNG_COLOR_TYPE_RGB)
        {
        }

        if (png_get_color_type(image->png_ptr, image->info_ptr) != PNG_COLOR_TYPE_RGBA)
        {
            // Some error handling: color_type of input file must be PNG_COLOR_TYPE_RGBA
        }
        for (int h = (int)image->width / 2; h >= 0; h--, count++)
            ;

        for (y = 0; y < image->height; y++)
        {
            png_byte *row = image->row_pointers[y];
            for (x = 0; x < image->width; x++)
            {
                png_byte *ptr = &(row[x * 4]);
                if (y <= 4)
                {
                    if ((x >= (int)(image->width / 2 - y)) && (x <= (int)(image->width / 2 + y)) && ((int)(image->width / 2) - y >= 0))
                    {
                        setColors(ptr, rgba);
                    }
                }
                else
                {
                    if (fill == 'y' || fill == 'Y')
                    {
                        if ((x >= (int)(image->width / 2 - y)) && (x <= (int)(image->width / 2 + y)) && ((int)(image->width / 2) - y >= 0))
                        {
                            setColors(ptr, rgba);
                        }
                    }
                }
            }
        }

        int control = 0;
        for (y = 0, k = (int)image->width / 2; y < image->height; y++, k--)
        {
            png_byte *row = image->row_pointers[y];
            for (x = 0; x < image->width; x++)
            {
                png_byte *ptr = &(row[x * 4]);

                if (y >= (int)count / 2)
                {

                    if (control == 0)
                    {

                        if (fill == 'Y' || fill == 'y')
                            control = k;
                        else
                            control = k + pixels;
                    }

                    if ((x > control) && (x < (image->width - control)) && y <= (count - 1))
                    {
                        setColors(ptr, rgba_rectangle);
                    }
                }
            }
        }
    }

    void makeCollage(struct Png * image, int n, int m)
    {
        int height = image->height;
        int width = image->width;
        png_bytep *rowPointers = (png_bytep *)malloc(height * sizeof(png_bytep));
        for (int y = 0; y < height; y++)
        {
            rowPointers[y] = (png_byte *)malloc(width * 4 * sizeof(png_byte));
            for (int x = 0; x < width * 4; x++)
                rowPointers[y][x] = image->row_pointers[y][x];
        }
        image->height *= m;
        image->width *= n;
        image->row_pointers = (png_bytep *)calloc(image->height, sizeof(png_bytep));

        for (int y = 0; y < height; y++)
            image->row_pointers[y] = (png_byte *)calloc(image->width * 4, sizeof(png_byte));
        for (int y = height; y < image->height; y++)
            image->row_pointers[y] = (png_byte *)malloc(image->width * 4 * sizeof(png_byte));
        int x = 0, y = 0;
        for (int j = 0; j < m; j++)
            for (int i = 0; i < n; i++)
                for (int y = 0; y < height; y++)
                    for (int x = 0; x < width * 4; x++)
                        image->row_pointers[y + (j * height)][x + (i * width * 4)] = rowPointers[y][x];
        for (int y = 0; y < height; y++)
            free(rowPointers[y]);
        free(rowPointers);
    }

    void colors(int *rgba, int color)
    {
        switch (color)
        {
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
        case 3:
            rgba[0] = 255;
            rgba[1] = 255;
            rgba[2] = 0;
            rgba[3] = 255;
            break;

        case 4:
            rgba[0] = 1255;
            rgba[1] = 165;
            rgba[2] = 0;
            rgba[3] = 255;
            break;

        case 5:
            rgba[0] = 128;
            rgba[1] = 0;
            rgba[2] = 128;
            rgba[3] = 255;
            break;

        case 6:
            rgba[0] = 0;
            rgba[1] = 128;
            rgba[2] = 128;
            rgba[3] = 255;
            break;

        case 7:
            rgba[0] = 50;
            rgba[1] = 110;
            rgba[2] = 50;
            rgba[3] = 255;
            break;

        case 8:
            rgba[0] = 165;
            rgba[1] = 40;
            rgba[2] = 40;
            rgba[3] = 255;
            break;

        case 9:
            rgba[0] = 255;
            rgba[1] = 0;
            rgba[2] = 255;
            rgba[3] = 255;
            break;

        case 10:
            rgba[0] = 255;
            rgba[1] = 193;
            rgba[2] = 203;
            rgba[3] = 255;
            break;

        case 11:
            rgba[0] = 64;
            rgba[1] = 224;
            rgba[2] = 208;
            rgba[3] = 255;
            break;

        case 12:
            rgba[0] = 255;
            rgba[1] = 215;
            rgba[2] = 0;
            rgba[3] = 255;
            break;

        case 13:
            rgba[0] = 192;
            rgba[1] = 192;
            rgba[2] = 192;
            rgba[3] = 255;
            break;

        case 14:
            rgba[0] = 0;
            rgba[1] = 0;
            rgba[2] = 0;
            rgba[3] = 255;
            break;

        case 15:
            rgba[0] = 255;
            rgba[1] = 255;
            rgba[2] = 255;
            rgba[3] = 255;
            break;

	}
}

void setColors(png_byte *ptr, int rgba[]){
    ptr[0] = rgba[0];
    ptr[1] = rgba[1];
    ptr[2] = rgba[2];
    ptr[3] = rgba[3];
}

/*Menu para escolher uma cor de linha */
int chooseColor(){

    int color_line;
    printf("[0] --> for red\n");
    printf("[1] --> for green\n");
    printf("[2] --> for blue\n");
    printf("[3] --> for yellow\n");
    printf("[4] --> for orange\n");
    printf("[5] --> for purple\n");
    printf("[6] --> for teal\n");
    printf("[7] --> for lime\n");
    printf("[8] --> for brown\n");
    printf("[9] --> for magenta\n");
    printf("[10] --> for pink\n");
    printf("[11] --> for turquoise\n");
    printf("[12] --> for gold\n");
    printf("[13] --> for silver\n");
    printf("[14] --> for black\n");
    printf("[15] --> for white\n");
    printf(" Type your option: ");
    scanf("%d", &color_line);
    
    return color_line;
}

int main(int argc, char **argv) {

    if (argc != 3){
        fprintf(stderr,"Usage: program_name <file_in> <file_out>\n");
	return 0;
    }

    char fill;
    int color, color_line, color_rectangle, op, N, M;
    int *rgba = calloc(4, sizeof(int));
    int *rgba_line = calloc(4, sizeof(int));
    int *rgba_rectangle = calloc(4, sizeof(int));

    struct Png image;
    read_png_file(argv[1], &image);

    printf("\t========== Welcome to our programm ==========\n");
    printf("\t\t[1] -- To write triangle into photo\n");
    printf("\t\t[2] -- To write rectangle into photo\n");
    printf("\t\t[3] -- To collage of photo\n");
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
                printf("\nLine color\n");
                color_line = chooseColor();
    			
	    		if(color_line < 0 || color_line > 15)
	    			printf("Invalid option! try again.\n");
    			
    		}while(color_line < 0 || color_line > 15);
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
                    
                    color = chooseColor();
                    
                    if(color < 0 || color > 15)
                        printf("Invalid option! try again.\n\n");
                    
                }while(color < 0 || color > 15);
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
            
        case 3:
            do {
                printf("How much photos on vertical: \n");
                scanf("%d", &N);
                printf("How much photos on horizontal: \n");
                scanf("%d", &M);

                if(N < 0 || M < 0)
                    printf("Please, put number >= 0 ");
            }while(N < 0 || M < 0);
            makeCollage(&image, N, M);
            write_png_file(argv[2], &image);
            break;
            
    	case 0:
            printf("bye, bye!\n"); 
            break;
    }

    // for(int i = 0; i < 4*sizeof(int); i++){
    //     free(rgba[i]);
    //     free(rgba_line[i]);
    //     free(rgba_rectangle[i]);
    // }

    free(rgba);
    free(rgba_line);
    free(rgba_rectangle);
    return 0;
}