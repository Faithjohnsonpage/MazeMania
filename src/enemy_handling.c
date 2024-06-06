#include "../headers/mazemania.h"

/**
 * init_Enemy - Initializes an enemy with the specified parameters.
 * @enemy: Pointer to the Enemy structure to initialize.
 * @x: The x-coordinate of the enemy's position.
 * @y: The y-coordinate of the enemy's position.
 * @texturePath: The path to the texture file for the enemy.
 * @renderer: Pointer to the SDL_Renderer structure for rendering.
 *
 * This function initializes an enemy with the specified position, size, speed,
 * health, and direction. It also loads the enemy texture from the specified
 * file path using the provided renderer.
 */

void init_Enemy(Enemy *enemy, int x, int y, const char *texturePath,
		SDL_Renderer *renderer)
{
	enemy->rect.x = x;
	enemy->rect.y = y;
	enemy->rect.w = (TILE_SIZE / 3);
	enemy->rect.h = (TILE_SIZE / 3);
	enemy->speed = 100.0f;
	enemy->health = 100;
	enemy->direction = 0.0f;

	/* Load the enemy texture */
	if (load_EnemyTexture(renderer, texturePath, &enemy->texture, false) != 0)
	{
		fprintf(stderr, "Failed to load enemy texture: %s\n", texturePath);
	}
}

/**
 * renderEnemy - Renders the enemy on the screen.
 * @instance: Pointer to the SDL_Instance structure with window and renderer.
 * @enemy: Pointer to the Enemy structure representing the enemy to render.
 *
 * This function renders the enemy on the screen using the SDL renderer
 * specified in the SDL_Instance structure.
 */

void renderEnemy(SDL_Instance *instance, Enemy *enemy)
{
	SDL_RenderCopyEx(instance->renderer, enemy->texture, NULL, &enemy->rect,
			0, NULL, SDL_FLIP_NONE);
}

/**
 * findSpawnPoints - Finds the spawn points on the world map.
 * @spawnPointsX: Pointer to an array to store x-coordinates of spawn points.
 * @spawnPointsY: Pointer to an array to store y-coordinates of spawn points.
 * @numSpawnPoints: Pointer to an integer to store the number of spawn
 * points found.
 *
 * This function searches the world map for spawn points and stores their
 * coordinates in the provided arrays. It updates the number of spawn points
 * found in the numSpawnPoints variable.
 */

void findSpawnPoints(int *spawnPointsX, int *spawnPointsY, int *numSpawnPoints)
{
	*numSpawnPoints = 0;

	for (int i = 0; i < mapHeight; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			if (worldMap[i][j] == 4)
			{
				spawnPointsX[*numSpawnPoints] = j;
				spawnPointsY[*numSpawnPoints] = i;
				(*numSpawnPoints)++;
			}
		}
	}
}

/**
 * load_enemies - Loads enemies for the specified level.
 * @enemies: Pointer to an array of Enemy structures to store loaded enemies.
 * @level: The level for which enemies are to be loaded.
 * @instance: Pointer to the SDL_Instance structure with window and renderer.
 *
 * This function loads enemies for the specified level. It randomly selects
 * spawn points from the world map and initializes enemies at those points
 * with the specified texture and renderer. It returns 0 on success and 1
 * on failure.
 *
 * Return: 0 on success, 1 on failure.
 */

int load_enemies(Enemy *enemies, int level, SDL_Instance *instance)
{
	int spawnPointsX[mapHeight * mapWidth];
	int spawnPointsY[mapHeight * mapWidth];
	int numSpawnPoints, i, j, numEnemies, index;

	findSpawnPoints(spawnPointsX, spawnPointsY, &numSpawnPoints);

	numEnemies = 4 * level;
	if (numEnemies > numSpawnPoints)
	{
		fprintf(stderr, "Not enough spawn points for enemies.\n");
		return (1);
	}

	/* Seed the random number generator */
	srand(time(NULL));

	for (i = 0; i < numEnemies; i++)
	{
		index = rand() % numSpawnPoints;
		init_Enemy(&enemies[i], spawnPointsX[index] * TILE_SIZE +
				(TILE_SIZE / 3), spawnPointsY[index] * TILE_SIZE + (TILE_SIZE / 3),
				"../images/darklord.png", instance->renderer);

		/* Remove the selected spawn point */
		for (j = index; j < numSpawnPoints - 1; j++)
		{
			spawnPointsX[j] = spawnPointsX[j + 1];
			spawnPointsY[j] = spawnPointsY[j + 1];
		}
		numSpawnPoints--;
	}

	return (0);
}
