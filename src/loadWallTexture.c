#include "../headers/mazemania.h"

void init_wallTexture(wallTexture *t)
{
	t->texture = NULL;
	t->width = 0;
	t->height = 0;
	t->pixels = NULL;
}

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
	texture->pixels = (Uint32 *)loadedSurface->pixels;
	SDL_FreeSurface(loadedSurface);

	return 0;
}

Uint32 getTexturePixel(wallTexture *texture, int x, int y)
{
	x = x % texture->width;
	y = y % texture->height;
	return texture->pixels[y * texture->width + x];
}
