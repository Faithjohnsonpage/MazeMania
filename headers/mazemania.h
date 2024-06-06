#ifndef MAZEMANIA_H_
#define MAZEMANIA_H_

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define mapWidth 21
#define mapHeight 12
#define TILE_SIZE 60
#define SCREEN_WIDTH 1260
#define SCREEN_HEIGHT 720
#define NUM_RAYS SCREEN_WIDTH
#define FOV_ANGLE 60
#define DEG_TO_RAD(angle) ((angle) * M_PI / 180.0)
#define DIST_TO_PROJ_PLANE ((NUM_RAYS / 2) / tan(DEG_TO_RAD(FOV_ANGLE) / 2))
#define MINIMAP_SCALE 0.2f
#define MAX_ENEMIES 24

extern int worldMap[mapHeight][mapWidth];

extern int worldMap1[mapHeight][mapWidth];
extern int worldMap2[mapHeight][mapWidth];
extern int worldMap3[mapHeight][mapWidth];
extern int worldMap4[mapHeight][mapWidth];
extern int worldMap5[mapHeight][mapWidth];
extern int worldMap6[mapHeight][mapWidth];

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

/**
 * struct wallTexture - Structure to store texture information for walls.
 * @texture: SDL_Texture pointer that holds the actual texture data.
 * @width: The width of the texture in pixels.
 * @height: The height of the texture in pixels.
 * @pixels: Pointer to the pixel data of the texture, typically used
 * for direct pixel manipulation.
 *
 * Description: This structure holds all necessary information related
 * to a wall's texture in a rendering context. It includes dimensions of
 * the texture and pointers to the texture itself and its pixel data, allowing
 * for detailed texture management and manipulation within the game or
 * graphics application.
 */

typedef struct wallTexture
{
	SDL_Texture *texture;
	int width;
	int height;
	Uint32 *pixels;
} wallTexture;

/**
 * struct LevelManager - Manages the levels and their corresponding
 * maps in the game.
 * @worldMap: Pointer to a dynamically allocated 2D array representing
 * the map of the current level.
 * @current_Level: The index of the currently active level in the game.
 *
 * Description: The LevelManager is responsible for handling and switching
 * between different levels in a game. It stores the maps of levels as 2D
 * arrays and tracks the current level being played, facilitating level
 * transitions and map data retrieval.
 */

typedef struct LevelManager
{
	int **worldMap;
	int current_Level;
} LevelManager;

/**
 * struct Enemy - Represents an enemy character in the game.
 * @rect: SDL_Rect structure defining the position and dimensions of the enemy.
 * @texture: SDL_Texture pointer for the enemy's visual representation.
 * @speed: Floating-point value representing the movement speed of the enemy.
 * @health: Integer value representing the health points of the enemy.
 * @direction: Floating-point value indicating the movement direction of the
 * enemy in degrees.
 *
 * Description: This structure is used to manage the properties and behavior of
 * enemy entities within the game. It includes their graphical representation,
 * physical characteristics, and gameplay attributes such as speed, health, and
 * direction of movement, essential for dynamic and interactive game
 * environments.
 */

typedef struct Enemy
{
	SDL_Rect rect;
	SDL_Texture *texture;
	float speed;
	int health;
	float direction;
} Enemy;

int init_instance(SDL_Instance *instance);
void initTexture(Texture *t);
void freeTexture(Texture *t);
void renderTopHalf(SDL_Instance *instance);
void renderBottomHalf(SDL_Instance *instance);
int loadTexture(SDL_Renderer *renderer, const char *path, Texture *texture,
		bool is_miniPlayer);
void cleanup(SDL_Instance *instance);
int checkIntersection(const SDL_Rect *A, const SDL_Rect *B);
void render_world(SDL_Instance *instance, SDL_Rect *rect, bool isMinimap);
void handleEvent(SDL_Event *event, SDL_Rect *object, Texture *texture,
		float speed, double *degrees, float deltaTime, bool *isMinimap);
int truncateDivisionFloat(float value, float divisor);
void castRays(SDL_Instance *instance, float playerX, float playerY,
		float playerRotation, bool isMiniMap, wallTexture *wallTexture);
void castSingleRay(float playerX, float playerY, float rayAngle, float scale,
		SDL_Instance *instance, float playerRotation, bool isMiniMap,
		wallTexture *wallTexture, int ray);
void drawRay(SDL_Renderer *renderer, float playerX, float playerY,
		float rayAngle, float rayDistance);
void drawWallSlice(SDL_Renderer *renderer, int rayIndex, int wallHeight,
		int horizontalRay, int verticalRay);

/* Loading Worlds */
int load_up_world(FILE *file, int worldMap[mapHeight][mapWidth]);
int load_worlds_from_file(void);
void loadCurrentLevel(LevelManager *levelManager);
void free_LevelManager(LevelManager *levelManager);
void init_LevelManager(LevelManager *levelManager);
void nextLevel(LevelManager *levelManager);

/* Handling wall texture */
void init_wallTexture(wallTexture *t);
void free_wallTexture(wallTexture *t);
Uint32 getTexturePixel(wallTexture *texture, int x, int y);
int load_wallTexture(SDL_Renderer *renderer, const char *path,
		wallTexture *texture);
void drawWallTexture(SDL_Renderer *renderer, int rayIndex, int wallHeight,
		wallTexture *texture, int texX);

/* Handling enemies */
void init_Enemy(Enemy *enemy, int x, int y, const char *texturePath,
		SDL_Renderer *renderer);
void findSpawnPoints(int *spawnPointsX, int *spawnPointsY,
		int *numSpawnPoints);
int load_enemies(Enemy *enemies, int level, SDL_Instance *instance);
void renderEnemy(SDL_Instance *instance, Enemy *enemy);
int load_EnemyTexture(SDL_Renderer *renderer, const char *file,
		SDL_Texture **texture, bool colorKey);

#endif /* MAZEMANIA_H_ */
