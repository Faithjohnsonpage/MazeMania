#include "../headers/mazemania.h"

/**
 * depthBuffer - Array to store the corrected perpendicular distance for each
 * ray cast in the rendering process.
 *
 * This buffer stores the corrected perpendicular distance (corrected for the
 * fisheye effect) for each ray cast, corresponding to each vertical column on
 * the screen. It ensures that closer objects are rendered in front of farther
 * ones, aids in realistic scaling of objects based on their distance from the
 * player, and enhances the 3D effect and rendering performance.
 */

float depthBuffer[SCREEN_WIDTH];

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

void renderEnemies3D(SDL_Instance *instance, Enemy *enemies, int numEnemies,
		float playerX, float playerY, float playerAngle)
{
    for (int i = 0; i < numEnemies; i++)
    {
        // Calculate the distance and angle of the enemy relative to the player
        float dx = enemies[i].rect.x - playerX;
        float dy = enemies[i].rect.y - playerY;
        float distanceToEnemy = sqrtf(dx * dx + dy * dy);
        float angleToEnemy = atan2f(dy, dx) - DEG_TO_RAD(playerAngle);

        // Ensure the angle is within the player's field of view
        if (angleToEnemy > M_PI)
            angleToEnemy -= 2 * M_PI;
        if (angleToEnemy < -M_PI)
            angleToEnemy += 2 * M_PI;

        // If the enemy is not within the FOV, skip rendering
        if (fabs(angleToEnemy) > DEG_TO_RAD(FOV_ANGLE) / 2)
            continue;

        // Calculate the projected screen position of the enemy
        float screenX = (SCREEN_WIDTH / 2) * (1 + tan(angleToEnemy) / tan(DEG_TO_RAD(FOV_ANGLE) / 2));
        float screenY = (SCREEN_HEIGHT / 2);

        // Calculate the height of the enemy on the screen
        float enemyHeight = (TILE_SIZE / distanceToEnemy) * (SCREEN_WIDTH / (2 * tan(DEG_TO_RAD(FOV_ANGLE) / 2)));
        float enemyWidth = enemyHeight;

        // Only render if the enemy is in front of the player and within the depth buffer
        if (distanceToEnemy > 0 && depthBuffer[(int)screenX] > distanceToEnemy)
        {
            SDL_Rect enemyRect;
            enemyRect.x = screenX - enemyWidth / 2;
            enemyRect.y = screenY - enemyHeight / 2;
            enemyRect.w = enemyWidth;
            enemyRect.h = enemyHeight;

            // Render the enemy
            SDL_RenderCopyEx(instance->renderer, enemies[i].texture, NULL, &enemyRect, 0, NULL, SDL_FLIP_NONE);
        }
    }
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
				"../images/Enemy2.png", instance->renderer);

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
