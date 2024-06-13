#include "../headers/mazemania.h"

int worldMap1[mapHeight][mapWidth];
int worldMap2[mapHeight][mapWidth];
int worldMap3[mapHeight][mapWidth];
int worldMap4[mapHeight][mapWidth];
int worldMap5[mapHeight][mapWidth];
int worldMap6[mapHeight][mapWidth];


/**
 * load_up_world - Loads a world map from a file.
 * @file: Pointer to the FILE structure representing the input file.
 * @worldMap: 2D array representing the world map to load the data into.
 *
 * This function reads the world map data from the specified file and populates
 * the given 2D array with the loaded values. It iterates over each element of
 * the array and uses fscanf to read integer values from the file. Returns 0 on
 * success, and -1 on failure.
 *
 * Return: 0 on success, -1 on failure.
 */

int load_up_world(FILE *file, int worldMap[mapHeight][mapWidth])
{
	int i, j;

	for (i = 0; i < mapHeight; i++)
	{
		for (j = 0; j < mapWidth; j++)
		{
			if (fscanf(file, "%d", &worldMap[i][j]) != 1)
			{
				fprintf(stderr, "Could not load world at (%d, %d)\n", i, j);
				return (-1);
			}
		}
	}
	return (0);
}

/**
 * load_worlds_from_file - Loads multiple world maps from a file.
 *
 * This function opens the "worlds.txt" file and sequentially loads multiple
 * world maps from it using the load_up_world function. It iterates through the
 * maps and loads each one into a corresponding worldMap array. Returns 0 on
 * success, and 1 if there was an error opening or reading the file.
 *
 * Return: 0 on success, 1 on failure.
 */

int load_worlds_from_file(void)
{
	FILE *file = fopen("worlds.txt", "r");

	if (file == NULL)
	{
		printf("Error opening file");
		return (1);
	}
	if (load_up_world(file, worldMap1) != 0)
	{
		fclose(file);
		return (1);
	}
	if (load_up_world(file, worldMap2) != 0)
	{
		fclose(file);
		return (1);
	}
	if (load_up_world(file, worldMap3) != 0)
	{
		fclose(file);
		return (1);
	}
	if (load_up_world(file, worldMap4) != 0)
	{
		fclose(file);
		return (1);
	}
	if (load_up_world(file, worldMap5) != 0)
	{
		fclose(file);
		return (1);
	}
	if (load_up_world(file, worldMap6) != 0)
	{
		fclose(file);
		return (1);
	}
	fclose(file);

	return (0);
}
