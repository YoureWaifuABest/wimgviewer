#include "head.h"
#include "change_image.c"

Uint32 change_image(SDL_Window*, SDL_Renderer*,
                    SDL_Surface*, SDL_Texture*, char *);

int main(int argc, char **argv)
{
	SDL_Window *screen;
	SDL_Renderer *renderer;
	SDL_Surface *image;
	SDL_Texture *texture;
	SDL_Event event;
	char ls[1000][1000], arghold[500], *pch;
	int done, i, lslen;
	Uint32 current;

	done = i = 0;

	/*
	 * Copy argv[1] into arghold so as to not modify
	 * argv[1].
	 *
	 * Set pch to point to the location of '/' in arghold,
	 * and replace that value with '\0', cutting off the string.
	 * 
	 * We only want the values before the final slash (the directory).
	 */
	strcpy(arghold, argv[1]);
	if (strchr(arghold, '/')) {
		pch = strrchr(arghold, '/');
		*(pch+1) = '\0';
	} else {
		arghold[0] = '.';
		arghold[1] = '/';
		arghold[2] = '\0';
	}

	/* 
	 * Set up an array which contains all of the files
	 * ending in .png, .jpg. or .gif
	 *
	 * This uses dirent, which may or may not be compatible
	 * with other platforms. Definitely works with
	 * linux though, so the rest doesn't matter.
	 */
	DIR *dir;
	struct dirent *ent;
	/* Check if the directory exists */
	if ((dir = opendir(arghold)) != NULL) {
		/* While there are files in the directory */
		while ((ent = readdir(dir)) != NULL) {
			/* Read the files into an array if they are images */
			if (strstr(ent->d_name, ".png") != NULL) {
				strcpy(ls[i], arghold);
				strcat(ls[i], ent->d_name);
				++i;
			} else if (strstr(ent->d_name, ".jpg") != NULL) {
				strcpy(ls[i], arghold);
				strcat(ls[i], ent->d_name);
				++i;
			} else if (strstr(ent->d_name, ".gif") != NULL) {
				strcpy(ls[i], arghold);
				strcat(ls[i], ent->d_name);
				++i;
			}
		}
		closedir(dir);
	} else {
		perror("");
		return EXIT_FAILURE;
	}

	/* 
	 * Length of the list.
	 * There are other ways to do this, but this is the most
	 * efficient.
	 */
	lslen = i-1;

	if (argc < 2) {
		printf("Error: arg should be image\n");
		return -1;
	}

	/* 
	 * Sets i equal to the location of argv[1] in ls
	 * This is to tell where to increment from when changing images
	 * via <- and ->. 
	 * Loop until argv[1] equals ls[i]
	 * Stop if argv[1] is not present in ls
	 */
	for (i = 0; strcmp(argv[1], ls[i]) && i != lslen; ++i)
		;

	SDL_Init(SDL_INIT_VIDEO);

	/* 
	 * Load whatever file argv[1] refers to into image
	 * See SDL2_image's docs.
	 */
	image = IMG_Load(argv[1]);
	/* Sets all variables to null (to stop Wall's complaints) */
	renderer = NULL;
	texture  = NULL;
	screen   = NULL;
	current = change_image(screen, renderer, image, texture, argv[1]);

	/* Gets events from SDL while done is = 0 */
	while (!done && SDL_WaitEvent(&event)!=-1)
	{
		//SDL_DestroyWindow(current);
		switch(event.type)
		{
			/* 
			 * If the event is a keypress, checks to see
			 * if the press is an escape, right, or left.
			 * Otherwise breaks.
			 */
			case SDL_KEYUP:
				switch(event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						done = 1;
						break;
					case SDLK_RIGHT:
						if (i < lslen) {
							SDL_DestroyWindow(SDL_GetWindowFromID(current));	

							image = IMG_Load(ls[++i]);
							current = change_image(screen, renderer, image, texture, ls[i]);
						} else {
							i = 0;
							SDL_DestroyWindow(SDL_GetWindowFromID(current));

							image = IMG_Load(ls[i]);
							current = change_image(screen, renderer, image, texture, ls[i]);
						}
						break;
					case SDLK_LEFT:
						if (i != 0) {
							SDL_DestroyWindow(SDL_GetWindowFromID(current));

							image = IMG_Load(ls[--i]);
							current = change_image(screen, renderer, image, texture, ls[i]);
						}
						else {
							i = lslen;
							SDL_DestroyWindow(SDL_GetWindowFromID(current));

							image = IMG_Load(ls[i]);
							current = change_image(screen, renderer, image, texture, ls[i]);
						}
						break;
					default:
						break;
				}
				break;
			case SDL_QUIT:
				done = 1;
				break;
			default:
				break;
		}
	}

	SDL_Quit();
	return 0;
}
