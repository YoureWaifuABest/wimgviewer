all: 
	gcc -Wall -o imgviewer main.c $(shell sdl2-config --cflags --libs) -lSDL2_image
