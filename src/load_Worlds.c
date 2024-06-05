#include "../headers/mazemania.h"

int worldMap1[mapHeight][mapWidth];
int worldMap2[mapHeight][mapWidth];
int worldMap3[mapHeight][mapWidth];
int worldMap4[mapHeight][mapWidth];
int worldMap5[mapHeight][mapWidth];
int worldMap6[mapHeight][mapWidth];

int load_up_world(FILE *file, int worldMap[mapHeight][mapWidth])
{
	for (int i = 0; i < mapHeight; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			if (fscanf(file, "%d", &worldMap[i][j]) != 1)
			{
				fprintf(stderr, "Could not load world at (%d, %d)\n", i, j);
				return -1;
			}
		}
	}
	return (0);
}


int load_worlds_from_file(void)
{
	FILE *file = fopen("worlds.txt", "r");

	if (file == NULL)
	{
		printf("Error opening file");
		return (1);
	}
	
	if (load_up_world(file, worldMap1) != 0)
		return (1);
	if (load_up_world(file, worldMap2) != 0)
		return (1);
	if (load_up_world(file, worldMap3) != 0)
		return (1);
	if (load_up_world(file, worldMap4) != 0)
		return (1);
	if (load_up_world(file, worldMap5) != 0)
		return (1);
	if (load_up_world(file, worldMap6) != 0)
		return (1);

	fclose(file);

	return (0);
}
