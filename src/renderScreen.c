#include "../headers/mazemania.h"

void renderTopHalf(SDL_Instance *instance)
{
	SDL_Rect TopRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2};

	SDL_SetRenderDrawColor(instance->renderer, 135, 206, 235, 255);

	SDL_RenderFillRect(instance->renderer, &TopRect);
}
