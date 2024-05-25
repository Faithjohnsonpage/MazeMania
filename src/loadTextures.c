#include "../headers/mazemania.h"

/**
 * initTexture - Initializes a Texture structure with default values.
 * @t: Pointer to the Texture structure to initialize.
 */

void initTexture(Texture *t)
{
	t->texture = NULL;
	t->width = 0;
	t->height = 0;
}

/**
 * freeTexture - Frees memory allocated for a Texture structure.
 * @t: Pointer to the Texture structure to free.
 */

void freeTexture(Texture *t)
{
	if (t->texture)
	{
		SDL_DestroyTexture(t->texture);
		t->texture = NULL;
	}
}


/**
 * loadTexture - Loads an image and creates a texture from it.
 * @renderer: The SDL renderer.
 * @path: The file path to the image.
 * @texture: Pointer to the Texture structure to store texture and
 * its dimensions.
 *
 * Return: 0 on success, 1 on failure.
 */
int loadTexture(SDL_Renderer *renderer, const char *path, Texture *texture)
{
	SDL_Surface *surface = IMG_Load(path);

	if (!surface)
	{
		fprintf(stderr, "Unable to load image %s: %s\n", path, IMG_GetError());
		return (1);
	}

	texture->texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture->texture)
	{
		fprintf(stderr, "Unable to create texture from %s: %s\n", path,
				SDL_GetError());
		SDL_FreeSurface(surface);
		return (1);
	}

	texture->width = surface->w;
	texture->height = surface->h;
	SDL_FreeSurface(surface);

	return (0);
}
