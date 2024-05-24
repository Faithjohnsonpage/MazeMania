#ifndef MAZEMANIA_H_
#define MAZEMANIA_H_

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdbool.h>

#define mapWidth 21
#define mapHeight 12
#define TILE_SIZE 60
#define SCREEN_WIDTH 1260
#define SCREEN_HEIGHT 720

/**
 * struct SDL_Instance - SDL window and renderer structure.
 * @window: Pointer to the SDL_Window structure.
 * @renderer: Pointer to the SDL_Renderer structure.
 *
 * This structure represents an SDL window and its associated renderer.
 * It contains pointers to the SDL_Window and SDL_Renderer instances
 * used for rendering graphics on the screen.
 */

typedef struct SDL_Instance
{
	SDL_Window *window;
	SDL_Renderer *renderer;
} SDL_Instance;

int init_instance(SDL_Instance *instance);
void cleanup(SDL_Instance *instance);
int checkIntersection(const SDL_Rect *A, const SDL_Rect *B);
void render_world(SDL_Instance *instance, SDL_Rect *rect);
void handleEvent(SDL_Event *event, SDL_Rect *object, int speed);

#endif /* MAZEMANIA_H_ */
