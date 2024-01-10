main: main.o
	gcc -o MAIN main.o -lm -lSDL2 -lSDL2_image -Wall -Wextra -pedantic

main.o: main.c map.h RenderList.h TextureMap.h FixedSizeArena.h Animator.h Definitions.h Vector.h
	gcc -c main.c

clean:
	rm *.o