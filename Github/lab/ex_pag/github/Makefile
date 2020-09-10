all: main.o fileIO.o colorPixel.o triangle.o rectangle.o collage.o
	gcc main.o fileIO.o colorPixel.o triangle.o rectangle.o collage.o -lpng -o cw

main.o: main.c fileIO.h structs.h colorPixel.h triangle.h rectangle.h collage.h
	gcc -c main.c

fileIO.o: fileIO.c fileIO.h structs.h
	gcc -c fileIO.c

colorPixel.o: colorPixel.c colorPixel.h structs.h
	gcc -c colorPixel.c

triangle.o: triangle.c triangle.h structs.h colorPixel.h
	gcc -c triangle.c

rectangle.o: rectangle.c rectangle.h structs.h colorPixel.h
	gcc -c rectangle.c

collage.o: collage.c collage.h structs.h
	gcc -c collage.c
	
clean:
	rm -rf *.o