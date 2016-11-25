#include "head.h"

Uint32 change_image(SDL_Window *screen, SDL_Renderer *renderer,
                 SDL_Surface *image, SDL_Texture *texture,
								 char *name) 
{
	if (!image) {
		fprintf(stderr, "IMG_Load: %s\n", IMG_GetError());
		exit(1);
	}

	screen = SDL_CreateWindow(name, 
	                 SDL_WINDOWPOS_UNDEFINED,
									 SDL_WINDOWPOS_UNDEFINED,
									 image->w,
									 image->h,
									 SDL_WINDOW_BORDERLESS);
	if (!screen) {
		fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
		exit(1);
	}

	renderer = SDL_CreateRenderer(screen, -1, 0);
	if (!renderer) {
		fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
		exit(1);
	}

	texture = SDL_CreateTextureFromSurface(renderer, image);
	if (!texture) {
		fprintf(stderr, "Failed create texture\n");
		exit(1);
	}
	SDL_FreeSurface(image);

	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);	
	
	return SDL_GetWindowID(screen);
}
