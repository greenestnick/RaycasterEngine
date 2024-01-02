main: main.o
	gcc -o MAIN main.o -lm -lSDL2 -lSDL2_image

main.o: main.c main.h stack.h map.h RenderList.h
	gcc -c main.c

clean:
	rm *.o