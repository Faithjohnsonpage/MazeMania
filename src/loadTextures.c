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
	t->width = 0;
	t->height = 0;
}


/**
 * loadTexture - Loads an image and creates a texture from it.
 * @renderer: The SDL renderer.
 * @path: The file path to the image.
 * @texture: Pointer to the Texture structure to store texture and
 * its dimensions.
 * @is_miniPlayer: Indicates whether the function is being called for a
 * mini-map. If true, the function performs operations specific to the
 * mini-map. If false, it performs the standard operations.
 *
 * Return: 0 on success, 1 on failure.
 */
int loadTexture(SDL_Renderer *renderer, const char *path, Texture *texture,
				bool is_miniPlayer)
{
	float scale = is_miniPlayer ? MINIMAP_SCALE : 1.0f;
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

	texture->width = surface->w * scale;
	texture->height = surface->h * scale;
	SDL_FreeSurface(surface);

	return (0);
}
