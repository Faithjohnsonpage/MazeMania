#include "../headers/mazemania.h"

void handleEvent(SDL_Event *event, SDL_Rect *object, int speed);

int worldMap[mapHeight][mapWidth] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 1},
	{1, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 1},
	{1, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};


/**
 * init_instance - Initializes the SDL instance, including the
 * window and renderer.
 * @instance: Pointer to the SDL_Instance structure to initialize.
 *
 * Return: 0 on success, 1 on failure.
 */

int init_instance(SDL_Instance *instance)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
		return (1);
	}

	/* Create a new window */
	instance->window = SDL_CreateWindow("MazeMania", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (instance->window == NULL)
	{
		fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
		return (1);
	}

	/* Create a renderer instance linked to the window */
	instance->renderer = SDL_CreateRenderer(instance->window, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (instance->renderer == NULL)
	{
		fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
		return (1);
	}

	return (0);
}

/**
 * render_world - Renders the world map.
 * @instance: Pointer to SDL_Instance structure with window and renderer.
 * @rect: Pointer to SDL_Rect structure defining map element dimensions.
 *
 * This function renders the world map using the SDL renderer specified in the
 * SDL_Instance structure. The size of each map element is defined by
 * the dimensions provided in the SDL_Rect structure.
 */

void render_world(SDL_Instance *instance, SDL_Rect *rect)
{
	rect->w = TILE_SIZE;
	rect->h = TILE_SIZE;
	int y, x;

	for (y = 0; y < mapHeight; y++)
	{
		for (x = 0; x < mapWidth; x++)
		{
			rect->x = x * TILE_SIZE;
			rect->y = y * TILE_SIZE;

			if (worldMap[y][x] == 1) /* Brown (Fence) */
			{
				SDL_SetRenderDrawColor(instance->renderer, 139, 69, 19, 255);
			}
			else if (worldMap[y][x] == 0) /* Grey (Floor) */
			{
				SDL_SetRenderDrawColor(instance->renderer, 128, 128, 128, 255);
			}
			else if (worldMap[y][x] == 2) /* Green (Wall) */
			{
				SDL_SetRenderDrawColor(instance->renderer, 34, 139, 34, 255);
			}
			else if (worldMap[y][x] == 3) /* Cream (Additional elements) */
			{
				SDL_SetRenderDrawColor(instance->renderer, 255, 253, 208, 255);
			}

			SDL_RenderFillRect(instance->renderer, rect);
		}
	}
}

/**
 * handleEvent - Handles SDL events for object movement.
 * @event: Pointer to the SDL_Event structure containing event data.
 * @object: Pointer to the SDL_Rect structure representing the object to move.
 * @speed: Speed at which the object moves.
 *
 * This function handles SDL events such as key presses to move the object on
 * the screen. It updates the position of the object based on the event and the
 * specified speed.
 */

void handleEvent(SDL_Event *event, SDL_Rect *object, int speed)
{
	int y, x;
	SDL_Rect prevPosition = *object;

	/* Handle keyboard input for movement */
	if (event->type == SDL_KEYDOWN)
	{
		switch (event->key.keysym.sym)
		{
			case SDLK_UP:
				object->y -= speed;
				break;
			case SDLK_DOWN:
				object->y += speed;
				break;
			case SDLK_LEFT:
				object->x -= speed;
				break;
			case SDLK_RIGHT:
				object->x += speed;
				break;
		}
	}

	if (object->x < 60)
		object->x = 60;

	if ((object->x + object->w) > SCREEN_WIDTH - 60)
		object->x = SCREEN_WIDTH - 65;

	if (object->y < 60)
		object->y = 60;

	if ((object->y + object->h) > SCREEN_HEIGHT - 60)
		object->y = SCREEN_HEIGHT - 65;

	/* Check for collision with internal walls */
	for (y = 0; y < mapHeight; y++)
	{
		for (x = 0; x < mapWidth; x++)
		{
			if (worldMap[y][x] == 2)
			{
				SDL_Rect wallRect = {x * TILE_SIZE, y * TILE_SIZE,
									 TILE_SIZE, TILE_SIZE};

				if (checkIntersection(object, &wallRect))
				{
					/* Move the object back to its previous position */
					*object = prevPosition;
					return;
				}
			}
		}
	}
}


/**
 * cleanup - Cleans up and frees SDL resources.
 * @instance: Pointer to the SDL_Instance structure containing
 * the window and renderer.
 */

void cleanup(SDL_Instance *instance)
{
	if (instance->renderer)
	{
		SDL_DestroyRenderer(instance->renderer);
	}
	if (instance->window)
	{
		SDL_DestroyWindow(instance->window);
	}
	SDL_Quit();
}


/**
 * main - Entry point of the program. Initializes SDL, runs the event loop,
 * and cleans up.
 *
 * Return: 0 on success, 1 on failure.
 */

int main(void)
{
	SDL_Instance instance;

	if (init_instance(&instance) != 0)
		return (1);

	SDL_Event event;
	SDL_Rect rect;
	SDL_Rect object = {60, 60, 5, 5};
	int running = 1;
	int speed = 5;

	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = 0;
			}

			handleEvent(&event, &object, speed);
		}

		/* Clear the window with a grey color */
		SDL_SetRenderDrawColor(instance.renderer, 128, 128, 128, 255);
		SDL_RenderClear(instance.renderer);

		render_world(&instance, &rect);

		/* Set the color for the moving object (red) */
		SDL_SetRenderDrawColor(instance.renderer, 255, 0, 0, 255);

		/* Render the moving object */
		SDL_RenderFillRect(instance.renderer, &object);

		/* Present the renderer */
		SDL_RenderPresent(instance.renderer);

		/* Add a small delay to prevent high CPU usage */
		SDL_Delay(16); /* Roughly 60 frames per second */
	}

	cleanup(&instance);

	return (0);
}
