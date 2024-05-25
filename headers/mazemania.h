#ifndef MAZEMANIA_H_
#define MAZEMANIA_H_

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define mapWidth 21
#define mapHeight 12
#define TILE_SIZE 60
#define SCREEN_WIDTH 1260
#define SCREEN_HEIGHT 720
#define FOV_ANGLE 60
#define PROJECTION_PLANE_WIDTH 320
#define PROJECTION_PLANE_HEIGHT 200

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

/**
 * struct SDL_Texture - A structure representing an SDL texture.
 * @texture: Pointer to an SDL_Texture object.
 * @width: Width of the texture in pixels.
 * @height: Height of the texture in pixels.
 *
 * This structure is used to encapsulate an SDL texture along with its
 * associated dimensions (width and height).
 */

typedef struct SDL_Texture
{
	SDL_Texture *texture;
	int width;
	int height;
} Texture;

int init_instance(SDL_Instance *instance);
void initTexture(Texture *t);
void freeTexture(Texture *t);
int loadTexture(SDL_Renderer *renderer, const char *path, Texture *texture);
void cleanup(SDL_Instance *instance);
int checkIntersection(const SDL_Rect *A, const SDL_Rect *B);
void render_world(SDL_Instance *instance, SDL_Rect *rect);
void handleEvent(SDL_Event *event, SDL_Rect *object, Texture *texture,
		int speed, double *degrees);
void rotateLogic(int *objX, int *objY, int *objW, int *objH, double degrees);

#endif /* MAZEMANIA_H_ */
