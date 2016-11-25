all: 
	gcc -Wall -o waifuviewer main.c $(shell sdl2-config --cflags --libs) -lSDL2_image

#gcc -o myprogram myprogram.c `sdl2-config --cflags --libs` 
