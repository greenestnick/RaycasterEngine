main: main.o
	gcc -o MAIN main.o -lm -lSDL2 -lSDL2_image

main.o: main.c
	gcc -c main.c

clean:
	rm *.o