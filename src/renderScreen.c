#include "../headers/mazemania.h"

/**
 * renderTopHalf - Renders the top half of the screen with a specified color.
 * @instance: Pointer to the SDL_Instance structure with window and renderer.
 *
 * This function renders the top half of the screen with a specified color. It
 * creates an SDL_Rect structure to define the dimensions of the top half and
 * fills it with the specified color using SDL_RenderFillRect.
 */


void renderTopHalf(SDL_Instance *instance)
{
	SDL_Rect TopRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2};

	SDL_SetRenderDrawColor(instance->renderer, 135, 206, 235, 255);

	SDL_RenderFillRect(instance->renderer, &TopRect);
}
