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


void setColor(png_bytep ptr, int typecolor){
    ptr[0] = 120;
    ptr[1] = ptr[2] = 0;
    if(typecolor == 3)
        ptr[3] = 255;
}

void drawLine(struct Png *image, int x0, int y0, int x1, int y1, int typecolor){
    int j = x0, i = y0;
    int dx = abs(x1 - x0), dy = abs(y1 - y0);
    int controlx = (x0 < x1)? 1:-1, controly = (y0 < y1)? 1:-1;
    int err = dx - dy;
    int pixels = 4;

    for( ;( j != x1 || i != y1 ); ){  

        png_bytep row  = image->row_pointers[i];
        png_bytep ptr = &(row[j * typecolor]);
        setColor(ptr, typecolor);

        for (int k = i - pixels; k < i+ pixels; k++){
            row  = image->row_pointers[k];
            for (int l = j - pixels; l < j + pixels; l++){
                ptr = &(row[l * typecolor]);
                setColor(ptr, typecolor);
            }
        }

        if (err * 2 > -dy){
            err -= dy;
            j += controlx;
        }
        else if (err * 2 < dx){
            err += dx;
            i += controly;
        }
    }

}

void organizar(int *x0, int *y0, int *x1, int *y1){
    *x0 += *x1;
    *x1 = *x0 - *x1;
    *x0 = *x0 - *x1;

    *y0 += *y1;
    *y1 = *y0 - *y1;
    *y0 = *y0 - *y1;      

}

void filltriangle(struct Png *image, int x0, int y0, int x1, int y1, int x2, int y2, int typecolor){
   
    if ( x0 >= x1 && x0 >= x2 ){
        if (x1 >= x2)
            organizar(&x1, &y1, &x0, &y0);
        else
            organizar(&x2, &y2, &x0, &y0);
    }
    else if (x1 >= x0 && x1 >= x2){
        if (x2 >= x0)
            organizar(&x2, &y2, &x0, &y0);
    }
    else if (x1 >= x0)
        organizar(&x1, &y1, &x0, &y0);

    if (y0 >= y1 && y0 >= y2){
        if (y1 <= y2)
            organizar(&x1, &y1, &x2, &y2);
    }else if (y1 >= y0 && y1 >= y2)
        organizar(&x2, &y2, &x1, &y1);

    int j = x0, i = y0;
    int dx = abs(x1 - x0), dy = abs(y1 - y0);
    int controlx = (x0 < x1) ? 1 : -1, controly = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    int j1 = x0, i1 = y0;
    int dx1 = abs(x2 - x0), dy1 = abs(y2 - y0);
    int controlx1 = (x0 < x2) ? 1 : -1, controly1 = (y0 < y2) ? 1 : -1;
    int err1 = dx1 - dy1;
    int k = 0, aum = 0, cont = 0 , k1 = 0, k2 = 0;   
    
    for (; (i1 != y2 || j1 != x2);){
        if (j > j1)
            aum = -1;
        else
            aum = 1;
        k = j;
        while (k != j1){
            png_byte *row = image->row_pointers[i1];
            png_byte *ptr = &(row[k * typecolor]);
            ptr[0] = 0; ptr[1] = ptr[2] = 200;
            if(typecolor == 4)
                ptr[3] = 255;
            k += aum;
        } 
        if(i > i1)
            k1 = 1;
        else if(i1 > i)
            k2 = 1;
        else{
                k2 = 0; k1 =0;
        }
        
        if(k1 == 0){
            if (err * 2 > -dy){
                err -= dy;
                j += controlx;
            }
            else if (err * 2 < dx){
                err += dx;
                i += controly;
            }
        }
        //media
        if(k2 == 0){  
            if (err1 * 2 > -dy1){
                err1 -= dy1;
                j1 += controlx1;
            }
            else if (err1 * 2 < dx1){
                err1 += dx1;
                i1 += controly1;
            }
        }
        if((i == y1) && (cont == 0)){
            cont = 2;
                j = x1, i = y1;
                dx = abs(x2 - x1), dy = abs(y2 - y1);
                controlx = (x1 < x2) ? 1 : -1, controly = (y1 < y2) ? 1 : -1;
                err = dx - dy;   
        }
    } 
}

void triangle(struct Png *image){
/* int x0 = 20, y0 = 400;
    int x1 = 350, y1 = 550;
    int x2 = 250, y2 = 130;

    int x0 = 100, y0 = 30;
    int x1 = 50, y1 = 100;
    int x2 = 600, y2 = 450;
 */ 

    int x0 = 690, y0 = 30;
    int x1 = 30, y1 = 250;
    int x2 = 750, y2 = 690;

    int typecolor;

    if(png_get_color_type(image->png_ptr, image->info_ptr) == PNG_COLOR_TYPE_RGB){
        typecolor = 3;
    }else if(png_get_color_type(image->png_ptr, image->info_ptr) == PNG_COLOR_TYPE_RGBA){
        typecolor = 4;
    }else{
        fprintf(stderr, "Color type of photo, must be RGB o, typecolor\n");
        exit(1);
    }

    drawLine(image, x0, y0, x1, y1, typecolor);
    drawLine(image, x2, y2, x1, y1, typecolor);
    drawLine(image, x0, y0, x2, y2, typecolor);

    filltriangle(image, x0, y0, x1, y1, x2, y2, typecolor);
}

void rectang(struct Png *image){
    int image_color [3] = {0, 0, 0}; 
    int typecolor;
    int A[2] = {0};
    int height = 0, h,hf = 0, heightf  = 0 , lengthk = 0 , lengthj = 0;

    if(png_get_color_type(image->png_ptr, image->info_ptr) == PNG_COLOR_TYPE_RGBA)
        typecolor = 4;
    else if (png_get_color_type(image->png_ptr, image->info_ptr) == PNG_COLOR_TYPE_RGB)
        typecolor = 3;
    else{
        fprintf(stderr, "error\n");
        exit(0);
    }
    int k = 0, contador = 0;
    for(int i = 0; i < image->height; i++){
        png_bytep row = image->row_pointers[i];
        for(int j = 0; j < image->width; j++){
            png_bytep ptr = &(row[j * typecolor]);
            if(image_color[0] == ptr[0] && image_color[1] == ptr[1] && image_color[2] == ptr[2]){
                k = j;
                while((image_color[0] == ptr[0] && image_color[1] == ptr[1] && image_color[2] == ptr[2]) && (k < image->width)){
                    k++;
                    ptr = &row[k * typecolor];
                }
                if((j != lengthj && k != lengthk) || (j == lengthj && k != lengthk)/* || (j != lengthj && k == lengthk)*/){
                    
                    for(int t = i + 1; t < image->height; t++){
                        row = image->row_pointers[t];
                        for(int cont = j; cont < k; cont++){
                            ptr = &(row[cont * typecolor]);
                            if(!(image_color[0] == ptr[0] && image_color[1] == ptr[1] && image_color[2] == ptr[2])){
                                h = i; hf = t;
                                t = image->height;
                                break;
                            }
                        }
                    }
                    contador++;
                    printf("%d) i = %d j = %d \n",contador,i,j);

                    if(A[0] == 0){
                        A[0] = (k - j - 1) * (hf - i -1);
                        lengthj = j;
                        lengthk = k;
                        heightf = hf;
                        height = i;
                        
                    }else {
                        int altura = 0;
                        if(j >= lengthj && lengthk > k){
                            altura = height;
                        }else{
                            altura = i;
                            A[1] = (k - j - 1) * (hf - altura - 1); 
                            if(A[0] < A[1]){
                                heightf = hf;
                                A[0] = A[1];
                                A[1] = 0;
                                lengthj = j;
                                height = altura;
                                lengthk = k;
                            }  
                        }                                            
                    }

                }j = k;
            }
            //hhhhhhhhhhhhhh
        }
    }


    for(int i = height; i < heightf; i++){
        png_bytep row = image->row_pointers[i];
        for(int j = lengthj; j < lengthk; j++){
            png_bytep ptr = &(row[j * typecolor]);
            ptr[0] = ptr[1] = ptr[2] = 255;
            if(typecolor == 4 )
                ptr[3] = 255;
        }

    }

} 

void Collage(struct Png *image, int n, int m){

    int height = image->height;
    int width = image->width;

    int typecolor;

    if(png_get_color_type(image->png_ptr, image->info_ptr) == PNG_COLOR_TYPE_RGBA)
        typecolor = 4;
    else if (png_get_color_type(image->png_ptr, image->info_ptr) == PNG_COLOR_TYPE_RGB)
        typecolor = 3;
    else{
        fprintf(stderr, "error\n");
        exit(0);
    }
    
    png_bytep *rowPointers = (png_bytep *)malloc(height * sizeof(png_bytep));
    for (int y = 0; y < height; y++)
    {
        rowPointers[y] = (png_byte *)malloc(width * typecolor * sizeof(png_byte));
        for (int x = 0; x < width * typecolor; x++)
            rowPointers[y][x] = image->row_pointers[y][x];
    }
    image->height *= m;
    image->width *= n;
    image->row_pointers = (png_bytep *)calloc(image->height, sizeof(png_bytep));

    for (int y = 0; y < height; y++)
        image->row_pointers[y] = (png_byte *)calloc(image->width * typecolor, sizeof(png_byte));
    for (int y = height; y < image->height; y++)
        image->row_pointers[y] = (png_byte *)malloc(image->width * typecolor * sizeof(png_byte));
    int x = 0, y = 0;
    for (int j = 0; j < m; j++)
        for (int i = 0; i < n; i++)
            for (int y = 0; y < height; y++)
                for (int x = 0; x < width * typecolor; x++)
                    image->row_pointers[y + (j * height)][x + (i * width * typecolor)] = rowPointers[y][x];
    for (int y = 0; y < height; y++)
        free(rowPointers[y]);
    free(rowPointers);
}

int main(int argc, char **argv){

    if (argc != 3){
        fprintf(stderr, "Usage: <file_in> <file_out>\n");
        return 0;
    }

    struct Png image;
    read_png_file(argv[1], &image);
    // triangle(&image);
    rectang(&image);
    // Collage(&image, 2, 1);
    write_png_file(argv[2], &image);
    return 0;
}