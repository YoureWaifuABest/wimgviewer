#include "head.h"

Uint32 change_image(SDL_Window *screen, SDL_Renderer *renderer,
                 SDL_Surface *image, SDL_Texture *texture,
								 char *name) 
{
	SDL_Rect srcrect;
	SDL_Rect dstrect;

	if (!image) {
		fprintf(stderr, "IMG_Load: %s\n", IMG_GetError());
		exit(1);
	}

	/* Define srcrect with properties of image */
	srcrect.x = 0;
	srcrect.y = 0;
	srcrect.w = image->w;
	srcrect.h = image->h;

	/* 
	 * Define dstrect, change as necessary.
	 * 
	 * This is to scale the image to be smaller than one's monitor.
	 *
	 * I could make a config for this instead of hard coding it, but
	 * I don't have any monitor that's larger than this res, and
	 * I'm the only one who will ever even consider using this program
	 * so I'm not going to.
	 */
	dstrect.x = 0;
	dstrect.y = 0;
	if (image->w >= 1920) {
		dstrect.w = (1920/(double)image->w)*(image->w);
		dstrect.h = (1920/(double)image->w)*(image->h);
	} 
	if (image->h >= 1080) {
		dstrect.w = (1064/(double)image->h)*(image->w);
		dstrect.h = (1064/(double)image->h)*(image->h);
	} else {
		dstrect.w = image->w;
		dstrect.h = image->h;
	}

	screen = SDL_CreateWindow(name, 
	                 SDL_WINDOWPOS_UNDEFINED,
									 SDL_WINDOWPOS_UNDEFINED,
									 dstrect.w,
									 dstrect.h,
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
	SDL_RenderCopy(renderer, texture, &srcrect, &dstrect); 
	SDL_RenderPresent(renderer);	
	
	return SDL_GetWindowID(screen);
}
