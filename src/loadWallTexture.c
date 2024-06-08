#include "../headers/mazemania.h"

/**
 * init_wallTexture - Initializes a wallTexture structure.
 * @t: Pointer to the wallTexture structure to initialize.
 *
 * This function initializes the members of a wallTexture structure,
 * setting the texture, width, height, and pixels to their default values.
 */

void init_wallTexture(wallTexture *t)
{
	t->texture = NULL;
	t->width = 0;
	t->height = 0;
	t->pixels = NULL;
}

/**
 * free_wallTexture - Frees resources allocated for a wallTexture structure.
 * @t: Pointer to the wallTexture structure to free.
 *
 * This function frees the resources allocated for a wallTexture structure,
 * including the texture and pixels, and resets the width and height.
 */

void free_wallTexture(wallTexture *t)
{
	if (t->texture)
	{
		SDL_DestroyTexture(t->texture);
		t->texture = NULL;
	}
	t->width = 0;
	t->height = 0;
	t->pixels = NULL;
}

/**
 * load_wallTexture - Loads a wall texture from a file.
 * @renderer: Pointer to the SDL_Renderer structure for rendering.
 * @path: Path to the image file containing the texture.
 * @texture: Pointer to the wallTexture structure to store the loaded texture.
 *
 * This function loads a wall texture from specified file using the provided
 * renderer. It creates a texture from the loaded surface and stores it in the
 * wallTexture structure. Returns 0 on success, and non-zero on failure.
 *
 * Return: 0 on success, non-zero on failure.
 */

int load_wallTexture(SDL_Renderer *renderer, const char *path, wallTexture *texture)
{
	SDL_Surface *loadedSurface = IMG_Load(path);
	if (!loadedSurface)
	{
		printf("Failed to load texture: %s\n", IMG_GetError());
		return 1;
	}

	texture->texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
	if (!texture->texture)
	{
		printf("Failed to create texture: %s\n", SDL_GetError());
		SDL_FreeSurface(loadedSurface);
		return 1;
	}

	texture->width = loadedSurface->w;
	texture->height = loadedSurface->h;
	texture->pixels = (Uint32 *)malloc(texture->width * texture->height *
			sizeof(Uint32));
	memcpy(texture->pixels, loadedSurface->pixels, texture->width *
			texture->height * sizeof(Uint32));
	SDL_FreeSurface(loadedSurface);

	return 0;
}
