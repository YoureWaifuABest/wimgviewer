#include "head.h"
//#include "math.c"

struct asprat {
	unsigned int x;
	unsigned int y;
} aspect_ratio;

unsigned int gcd(unsigned int u, unsigned int v)
{
	/* simple cases (termination) */
	if (u == v)
		return u;
	if (u == 0)
		return v;
	if (v == 0)
		return u;
	
	/* look for factors of 2 */
	if (~u & 1) // u is even
	{
		if (v & 1) // v is odd
			return gcd(u >> 1, v);
		else // both u and v are even
			return gcd(u >> 1, v >> 1) << 1;
	}
	
	if (~v & 1) // u is odd, v is even
		return gcd(u, v >> 1);
	
	// reduce larger argument
	if (u > v)
		return gcd((u - v) >> 1, v);
	
	return gcd((v - u) >> 1, u);
}

Uint32 change_image(SDL_Window *screen, SDL_Renderer *renderer,
                 SDL_Surface *image, SDL_Texture *texture,
								 char *name) 
{
	unsigned int gcds;
	SDL_Rect srcrect;
	SDL_Rect dstrect;

	if (!image) {
		fprintf(stderr, "IMG_Load: %s\n", IMG_GetError());
		exit(1);
	}

	gcds = gcd(image->h, image->w);
	aspect_ratio.x = (image->w)/gcds;
	aspect_ratio.y = (image->h)/gcds;

	srcrect.x = 0;
	srcrect.y = 0;
	srcrect.w = image->w;
	srcrect.h = image->h;

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
