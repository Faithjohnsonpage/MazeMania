#include "../headers/mazemania.h"

int worldMap[mapHeight][mapWidth];

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

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n",
				IMG_GetError());
		return (1);
	}

	return (0);
}

/**
 * render_world - Renders the world map.
 * @instance: Pointer to SDL_Instance structure with window and renderer.
 * @rect: Pointer to SDL_Rect structure defining map element dimensions.
 * @isMinimap: Indicates whether the function is being called for a mini-map.
 * If true, the function performs operations specific to the mini-map.
 * If false, it performs the standard operations.
 *
 * This function renders the world map using the SDL renderer specified in the
 * SDL_Instance structure. The size of each map element is defined by
 * the dimensions provided in the SDL_Rect structure.
 */

void render_world(SDL_Instance *instance, SDL_Rect *rect, bool isMinimap)
{
	float scale = isMinimap ? MINIMAP_SCALE : 1.0f;
	int y, x;
	rect->w = TILE_SIZE * scale;
	rect->h = TILE_SIZE * scale;

	if (isMinimap)
	{
		for (y = 0; y < mapHeight; y++)
		{
			for (x = 0; x < mapWidth; x++)
			{
				rect->x = x * TILE_SIZE * scale;
				rect->y = y * TILE_SIZE * scale;

				if (worldMap[y][x] == 1)
				{
					SDL_SetRenderDrawColor(instance->renderer, 34, 139, 34,
							255);
				}
				else if (worldMap[y][x] == 0)
				{
					SDL_SetRenderDrawColor(instance->renderer, 128, 128, 128,
							255);
				}
				else if (worldMap[y][x] == 2)
				{
					SDL_SetRenderDrawColor(instance->renderer, 34, 139, 34,
							255);
				}
				else if (worldMap[y][x] == 3)
				{
					SDL_SetRenderDrawColor(instance->renderer, 255, 253, 208,
							255);
				}

				SDL_RenderFillRect(instance->renderer, rect);
			}
		}
	}
}

/**
 * handleEvent - Handles SDL events for object movement and rotation.
 * @event: Pointer to the SDL_Event structure containing event data.
 * @object: Pointer to the SDL_Rect structure representing object's position.
 * @texture: Pointer to the Texture structure representing object's texture.
 * @speed: Speed at which the object moves.
 * @degrees: Pointer to the angle in degrees to rotate the object.
 * @deltaTime: Time elapsed since the last frame.
 * @isMinimap: Pointer to a boolean indicating if the minimap is enabled.
 *
 * This function processes SDL events such as keyboard input to move and
 * rotate the object within the game world. It updates the object's position
 * and rotation angle based on the input events, ensuring the movement is
 * frame-rate independent by using the deltaTime parameter.
 */

void handleEvent(SDL_Event *event, SDL_Rect *object, Texture *texture,
		float speed, double *degrees, float deltaTime, bool *isMinimap)
{
	SDL_Rect prevPosition = *object;
	int y, x;

	/* Define key states */
	static bool keyW, keyS, keyA, keyD;

	/* Handle keyboard input for movement */
	if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP)
	{
		bool isKeyDown = (event->type == SDL_KEYDOWN);

		switch (event->key.keysym.sym)
		{
			case SDLK_w:
				keyW = isKeyDown;
				break;
			case SDLK_s:
				keyS = isKeyDown;
				break;
			case SDLK_a:
				keyA = isKeyDown;
				break;
			case SDLK_d:
				keyD = isKeyDown;
				break;
			case SDLK_LEFT:
				if (isKeyDown)
					*degrees -= 180 * deltaTime;
				break;
			case SDLK_RIGHT:
				if (isKeyDown)
					*degrees += 180 * deltaTime;
				break;
			case SDLK_ESCAPE:
				if (isKeyDown)
					*isMinimap = !*isMinimap; /* Toggle mini-map state */
				break;
			case SDLK_RETURN:
				if (isKeyDown)
					*isMinimap = true;
				break;
		}
	}

	/* Determine the resulting movement based on key states */
	float moveX = 0, moveY = 0;

	if ((keyW && keyS) || (keyA && keyD))
	{
		moveX = 0;
		moveY = 0;
	}
	else
	{
		if (keyW && keyA)
		{
			moveY = -speed * deltaTime;
			moveX = -speed * deltaTime;
		}
		else if (keyW && keyD)
		{
			moveY = -speed * deltaTime;
			moveX = speed * deltaTime;
		}
		else if (keyA && keyS)
		{
			moveX = -speed * deltaTime;
			moveY = speed * deltaTime;
		}
		else if (keyS && keyD)
		{
			moveX = speed * deltaTime;
			moveY = speed * deltaTime;
		}
		else if (keyW && !keyS)
		{
			moveY = -speed * deltaTime;
		}
		else if (keyA && !keyD)
		{
			moveX = -speed * deltaTime;
		}
		else if (keyS && !keyW)
		{
			moveY = speed * deltaTime;
		}
		else if (keyD && !keyA)
		{
			moveX = speed * deltaTime;
		}
	}

	object->x += moveX;
	object->y += moveY;

	/* Boundary checks. Added 10 pixels so that the player will go into open
	space when the world is rendered with textures */
	if (object->x < TILE_SIZE + 10)
		object->x = TILE_SIZE + 10;

	if ((object->x + texture->width) > SCREEN_WIDTH - TILE_SIZE - 10)
		object->x = SCREEN_WIDTH - TILE_SIZE - 10 - texture->width;

	if (object->y < TILE_SIZE + 10)
		object->y = TILE_SIZE + 10;

	if ((object->y + texture->height) > SCREEN_HEIGHT - TILE_SIZE - 10)
		object->y = SCREEN_HEIGHT - TILE_SIZE - 10 - texture->height;

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
 * main - Entry point of the program.
 * Initializes SDL, manages the game loop,
 * handles events, updates the game state, and cleans up resources.
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
	SDL_Rect object = {80, 80, 0, 0};
	SDL_Rect miniobject = {96, 96, 0, 0};
	int running = 1, level = 1;
	double degrees = 0;
	Texture objectTexture, miniTexture;
	wallTexture wall1Texture;
	float speed = 200;
	float deltaTime;
	Uint32 lastFrameTime = SDL_GetTicks();
	bool isMinimap = true;
	LevelManager LevelManager;
	Enemy enemies[MAX_ENEMIES];

	if ((load_worlds_from_file()) != 0)
	{
		fprintf(stderr, "Could not complete loading the worlds");
		return (1);
	}

	initTexture(&objectTexture);
	initTexture(&miniTexture);
	init_wallTexture(&wall1Texture);
	init_LevelManager(&LevelManager);
	loadCurrentLevel(&LevelManager);

	if (load_enemies(enemies, level, &instance) != 0)
	{
		fprintf(stderr, "Could not complete loading the enemies\n");
		return (1);
	}

	if ((loadTexture(instance.renderer, "../images/dot.bmp", &objectTexture, false) != 0) ||
			(loadTexture(instance.renderer, "../images/dot.bmp", &miniTexture, true) != 0) ||
			(load_wallTexture(instance.renderer, "../images/wall1.png", &wall1Texture) != 0))
	{
		printf("Failed to load wall texture.\n");
		return (1);
	}


	/* Update the object rectangle with the texture's dimensions */
	object.w = objectTexture.width;
	object.h = objectTexture.height;

	/* Update the miniobject rectangle with the texture's dimensions */
	miniobject.w = miniTexture.width;
	miniobject.h = miniTexture.height;

	while (running)
	{
		Uint32 currentTime = SDL_GetTicks();
		deltaTime = (currentTime - lastFrameTime) / 1000.0f;
		lastFrameTime = currentTime;

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = 0;
			}

			handleEvent(&event, &object, &objectTexture, speed, &degrees, deltaTime, &isMinimap);

			/* Update miniobject based on object position */
			miniobject.x = object.x * MINIMAP_SCALE;
			miniobject.y = object.y * MINIMAP_SCALE;
		}

		/* Clear the window with a grey color */
		SDL_SetRenderDrawColor(instance.renderer, 128, 128, 128, 255);
		SDL_RenderClear(instance.renderer);

		renderTopHalf(&instance);

		/* Main game rendering */
		render_world(&instance, &rect, false);

		/* Render enemies */
		/*for (int i = 0; i < 4 * level; i++)
		{
			renderEnemy(&instance, &enemies[i]);
		}*/

		/* Render the moving object with rotation */
		/*SDL_RenderCopyEx(instance.renderer, objectTexture.texture, NULL,
		  &object, degrees, NULL, SDL_FLIP_NONE);*/

		/* Cast rays for lighting effect for main map */
		castRays(&instance, object.x, object.y, degrees, false, &wall1Texture);

		if (isMinimap)
		{
			/* Minimap rendering */
			render_world(&instance, &rect, true);

			/* Render the moving miniobject with rotation */
			SDL_RenderCopyEx(instance.renderer, miniTexture.texture, NULL,
					&miniobject, degrees, NULL, SDL_FLIP_NONE);

			/* Cast rays for lighting effect in mini_map */
			castRays(&instance, miniobject.x, miniobject.y, degrees, true, &wall1Texture);
		}

		/* Present the renderer */
		SDL_RenderPresent(instance.renderer);

		/* Add a small delay to prevent high CPU usage */
		SDL_Delay(16); /* Roughly 60 frames per second */

		/* Check for level completion */
		if (worldMap[object.y / TILE_SIZE][object.x / TILE_SIZE] == 3)
		{
			if (LevelManager.current_Level < 5)
			{
				LevelManager.current_Level++;
				loadCurrentLevel(&LevelManager);
				object.x = 80;
				object.y = 80;
			}
			else
			{
				printf("You have completed all levels!\n");
				running = false;
			}
		}
	}

	freeTexture(&objectTexture);
	freeTexture(&miniTexture);
	free_wallTexture(&wall1Texture);
	free_LevelManager(&LevelManager);
	for (int i = 0; i < 4 * level; i++)
	{
		freeTexture(enemies[i].texture);
	}
	cleanup(&instance);

	return (0);
}
