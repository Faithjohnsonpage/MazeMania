#include "../headers/mazemania.h"


void init_Enemy(Enemy *enemy, int x, int y, const char *texturePath, SDL_Renderer *renderer)
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

void renderEnemy(SDL_Instance *instance, Enemy *enemy)
{
    SDL_RenderCopyEx(instance->renderer, enemy->texture, NULL, &enemy->rect, 0, NULL, SDL_FLIP_NONE);
}

/*void updateEnemy(Enemy *enemy, float deltaTime) {
    // Example movement logic: move in the current direction
    enemy->rect.x += enemy->speed * cos(enemy->direction) * deltaTime;
    enemy->rect.y += enemy->speed * sin(enemy->direction) * deltaTime;

    // Add logic to change direction or respond to environment
    // For example, you can add collision detection or AI logic here
}*/


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

int load_enemies(Enemy *enemies, int level, SDL_Instance *instance)
{
    int spawnPointsX[mapHeight * mapWidth];
    int spawnPointsY[mapHeight * mapWidth];
    int numSpawnPoints, i, j;

    findSpawnPoints(spawnPointsX, spawnPointsY, &numSpawnPoints);

    int numEnemies = 4 * level;
    if (numEnemies > numSpawnPoints)
	{
        fprintf(stderr, "Not enough spawn points for enemies.\n");
        return 1;
    }

    /* Seed the random number generator */
    srand(time(NULL));

    for (i = 0; i < numEnemies; i++)
	{
        int index = rand() % numSpawnPoints;
        init_Enemy(&enemies[i], spawnPointsX[index] * TILE_SIZE + (TILE_SIZE / 3),
                  spawnPointsY[index] * TILE_SIZE + (TILE_SIZE / 3), 
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
