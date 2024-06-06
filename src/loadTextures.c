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

/**
 * load_EnemyTexture - Loads an enemy texture from a file.
 * @renderer: Pointer to the SDL_Renderer structure for rendering.
 * @file: Path to the image file containing the texture.
 * @texture: Pointer to the SDL_Texture pointer to store the loaded texture.
 * @colorKey: Boolean indicating whether to use color keying for transparency.
 *
 * This function loads an enemy texture from the specified file using the
 * provided renderer. If colorKey is true, it sets the color key for
 * transparency. It stores the loaded texture in the pointer provided.
 * Returns 0 on success, and non-zero on failure.
 *
 * Return: 0 on success, non-zero on failure.
 */


int load_EnemyTexture(SDL_Renderer *renderer, const char *file,
		SDL_Texture **texture, bool colorKey)
{
	SDL_Surface *surface = IMG_Load(file);

	if (!surface)
	{
		fprintf(stderr, "Could not load image: %s\n", SDL_GetError());
		return (-1);
	}

	if (colorKey)
	{
		SDL_SetColorKey(surface, SDL_TRUE,
				SDL_MapRGB(surface->format, 255, 0, 255));
	}

	*texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	if (!*texture)
	{
		fprintf(stderr, "Could not create texture: %s\n", SDL_GetError());
	}

	return (0);
}
