#include "../headers/mazemania.h"

void init_LevelManager(LevelManager *levelManager)
{
	int i;
    levelManager->current_Level = 0;

    /* Allocate memory for 6 levels */
    levelManager->worldMap = malloc(6 * sizeof(int *));
    if (levelManager->worldMap == NULL)
	{
        fprintf(stderr, "Failed to allocate memory for worldMap\n");
        exit(1);
    }
    for (i = 0; i < 6; ++i)
	{
        levelManager->worldMap[i] = malloc(mapHeight * mapWidth * sizeof(int));
    }

    /* Assign each world map to the worldMap array */
    levelManager->worldMap[0] = (int *)worldMap1;
    levelManager->worldMap[1] = (int *)worldMap2;
    levelManager->worldMap[2] = (int *)worldMap3;
    levelManager->worldMap[3] = (int *)worldMap4;
    levelManager->worldMap[4] = (int *)worldMap5;
    levelManager->worldMap[5] = (int *)worldMap6;
}

void free_LevelManager(LevelManager *levelManager)
{
	int i;

	for (i = 0; i < 6; ++i)
	{
        free(levelManager->worldMap[i]);
    }
    free(levelManager->worldMap);
}

void loadCurrentLevel(LevelManager *levelManager)
{
    memcpy(worldMap, levelManager->worldMap[levelManager->current_Level], sizeof(worldMap));
}
