#include "../headers/mazemania.h"

/**
 * init_LevelManager - Initializes the LevelManager structure.
 * @levelManager: Pointer to the LevelManager structure to initialize.
 *
 * Return: 0 if success and 1 if failure.
 *
 * This function initializes the LevelManager structure by setting the current
 * level to 0 and allocating memory for the world maps of 6 levels. It assigns
 * each world map to the worldMap array.
 */

int init_LevelManager(LevelManager *levelManager)
{
	int i;

	levelManager->current_Level = 0;

	/* Allocate memory for 6 levels */
	levelManager->worldMap = malloc(6 * sizeof(int *));
	if (levelManager->worldMap == NULL)
	{
		fprintf(stderr, "Failed to allocate memory for worldMap\n");
		return (1);
	}
	for (i = 0; i < 6; ++i)
	{
		levelManager->worldMap[i] = malloc(mapHeight * mapWidth * sizeof(int));

		if (levelManager->worldMap[i] == NULL)
		{
			fprintf(stderr, "Failed to allocate memory for worldMap[%d]\n", i);
			return (1);
		}

		memcpy(levelManager->worldMap[i], getWorldMap(i + 1),
				mapHeight * mapWidth * sizeof(int));
	}

	return (0);
}

/**
 * getWorldMap - Retrieves a pointer to the world map corresponding
 * to the given index.
 * @index: The index of the world map to retrieve.
 *
 * Return: A pointer to the world map array corresponding to the given index.
 */

int *getWorldMap(int index)
{
	switch (index)
	{
		case 1:
			return ((int *)worldMap1);
		case 2:
			return ((int *)worldMap2);
		case 3:
			return ((int *)worldMap3);
		case 4:
			return ((int *)worldMap4);
		case 5:
			return ((int *)worldMap5);
		case 6:
			return ((int *)worldMap6);
		default:
			return (NULL);
	}
}


/**
 * free_LevelManager - Frees the memory allocated for the LevelManager
 * structure.
 * @levelManager: Pointer to the LevelManager structure to free.
 *
 * This function frees the memory allocated for the world maps of 6 levels and
 * the worldMap array within the LevelManager structure.
 */
void free_LevelManager(LevelManager *levelManager)
{
	int i;

	for (i = 0; i < 6; ++i)
	{
		free(levelManager->worldMap[i]);
	}
	free(levelManager->worldMap);
}

/**
 * loadCurrentLevel - Loads the current level's world map into the main
 * world map.
 * @levelManager: Pointer to the LevelManager structure containing world maps.
 *
 * This function loads the world map of the current level from the LevelManager
 * structure into the main world map.
 */

void loadCurrentLevel(LevelManager *levelManager)
{
	memcpy(worldMap, levelManager->worldMap[levelManager->current_Level],
			sizeof(worldMap));
}
