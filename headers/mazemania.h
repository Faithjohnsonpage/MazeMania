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
#define NUM_RAYS SCREEN_WIDTH
#define FOV_ANGLE 60
#define DEG_TO_RAD(angle) ((angle) * M_PI / 180.0)
#define DIST_TO_PROJ_PLANE ((NUM_RAYS / 2) / tan(DEG_TO_RAD(FOV_ANGLE) / 2))

extern int worldMap[mapHeight][mapWidth];

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
void handleEvent(SDL_Event *event, SDL_Rect *object, Texture *texture, float speed, double *degrees, float deltaTime);
int truncateDivisionFloat(float value, float divisor);
void castRays(SDL_Instance *instance, float playerX, float playerY, float playerRotation);
float castSingleRay(float playerX, float playerY, float rayAngle);
void drawRay(SDL_Renderer *renderer, float playerX, float playerY, float rayAngle, float rayDistance);
void drawWallSlice(SDL_Renderer *renderer, int x, int wallHeight);
void drawWallSlice(SDL_Renderer *renderer, int rayIndex, int wallHeight);

#endif /* MAZEMANIA_H_ */
