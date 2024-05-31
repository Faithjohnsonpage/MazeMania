#include "../headers/mazemania.h"

/**
 * truncateDivisionFloat - Performs a truncated division of a float
 * value by a divisor
 * @value: The float value to be divided
 * @divisor: The float divisor
 *
 * This function performs a division of the given float value by the specified
 * divisor and truncates the result to an integer. It is used in scenarios
 * where right shift operations are not applicable, particularly because the
 * TILE_SIZE used is not a multiple of 2. This ensures the division result is
 * an integer without rounding, effectively mimicking the behavior of integer
 * division.
 *
 * Return: The truncated result of the division as an integer.
 */

int truncateDivisionFloat(float value, float divisor)
{
	return ((int)(value / divisor));
}

/**
 * castRays - Casts multiple rays from the player's position
 * @instance: The SDL_Instance containing rendering and game state information
 * @playerX: The x-coordinate of the player's position
 * @playerY: The y-coordinate of the player's position
 * @playerRotation: The player's current rotation angle
 * @isMiniMap: Indicates whether the function is being called for a mini-map.
 * If true, the function performs operations specific to the mini-map.
 * If false, it performs the standard operations.
 *
 * This function casts multiple rays from the player's position in different
 * directions based on the player's rotation. It utilizes the SDL_Instance
 * to manage rendering and game state, and it calculates the distance each
 * ray travels before hitting an obstacle. The function is used to simulate
 * the player's field of view and render the environment accordingly.
 */

void castRays(SDL_Instance *instance, float playerX, float playerY, float playerRotation, bool isMiniMap)
{
	float scale = isMiniMap ? MINIMAP_SCALE : 1.0f;
	float rayAngle;
	float rayDistance, correctedDistance;
	float angleIncrement = FOV_ANGLE / (float)(NUM_RAYS * scale);
	int ray, wallHeight;

	/* Normalize the player's rotation angle to [0, 360) range */
	playerRotation = fmod(playerRotation, 360);
	if (playerRotation < 0)
	{
		playerRotation += 360;
	}

	for (ray = 0; ray < (NUM_RAYS * scale); ray++)
	{
		/* Calculate the angle for this ray, considering player's rotation */
		rayAngle = playerRotation - (FOV_ANGLE / 2) + ray * angleIncrement;

		/* Normalize the angle to keep it within [0, 360) range */
		rayAngle = fmod(rayAngle, 360);
		if (rayAngle < 0)
		{
			rayAngle += 360;
		}

		/* Calculate the distance of the ray */
		rayDistance = castSingleRay(playerX, playerY, rayAngle, scale);

		if (isMiniMap)
			drawRay(instance->renderer, playerX, playerY, rayAngle, rayDistance);

		if (!isMiniMap)
		{
			/* Calculate the projected wall height */
			correctedDistance = rayDistance * cos(DEG_TO_RAD(rayAngle - playerRotation));
			wallHeight = (int)((TILE_SIZE / correctedDistance) * DIST_TO_PROJ_PLANE);

			/* Draw the wall slice */
			drawWallSlice(instance->renderer, ray, wallHeight);
		}
	}
}

/**
 * castSingleRay - Casts a single ray and calculates its distance
 * @playerX: The x-coordinate of the player's position
 * @playerY: The y-coordinate of the player's position
 * @rayAngle: The angle at which the ray is cast
 * @scale: Specifies the scaling factor to be applied.
 *
 * This function casts a single ray from the player's position at the specified
 * angle and calculates the distance the ray travels before hitting an obstacle
 * The function returns the distance from the player's position to the point of
 * intersection with the obstacle.
 *
 * Return: The distance the ray travels before hitting an obstacle.
 */

float castSingleRay(float playerX, float playerY, float rayAngle, float scale)
{
	/* Convert angle to radians for trigonometric functions */
	float rayAngleRad = DEG_TO_RAD(rayAngle);
	float TILE_SIZED = TILE_SIZE * scale;

	/* Define variables to track intersection points */
	float horizontalHitX, horizontalHitY, verticalHitX, verticalHitY;
	float horizontalDistance = INFINITY, verticalDistance = INFINITY;
	int foundHorizontalWallHit = 0, foundVerticalWallHit = 0;
	int gridX, gridY;

	int isRayFacingDown = (rayAngle > 0 && rayAngle < 180);
	int isRayFacingUp = !isRayFacingDown;
	int isRayFacingRight = (rayAngle < 90 || rayAngle > 270);
	int isRayFacingLeft = !isRayFacingRight;

	/* Horizontal intersections */
	float ystep = TILE_SIZED;
	float xstep = (TILE_SIZED) / tan(rayAngleRad);

	float nextHorizontalTouchX, nextHorizontalTouchY, nextVerticalTouchX, nextVerticalTouchY;

	if (isRayFacingDown)
	{
		nextHorizontalTouchY = truncateDivisionFloat(playerY, (TILE_SIZED)) * TILE_SIZED + TILE_SIZED;
		nextHorizontalTouchX = playerX + (nextHorizontalTouchY - playerY) / tan(rayAngleRad);
		ystep = TILE_SIZED;
		xstep = TILE_SIZED / tan(rayAngleRad);
	}
	else
	{
		nextHorizontalTouchY = truncateDivisionFloat(playerY, TILE_SIZED) * TILE_SIZED - 1;
		nextHorizontalTouchX = playerX + (nextHorizontalTouchY - playerY) / tan(rayAngleRad);
		ystep = -TILE_SIZED;
		xstep = -TILE_SIZED / tan(rayAngleRad);
	}

	while (nextHorizontalTouchX >= 0 && nextHorizontalTouchX < mapWidth * TILE_SIZED &&
			nextHorizontalTouchY >= 0 && nextHorizontalTouchY < mapHeight * TILE_SIZED)
	{
		gridX = truncateDivisionFloat(nextHorizontalTouchX, TILE_SIZED);
		gridY = truncateDivisionFloat(nextHorizontalTouchY + (isRayFacingUp ? -1 : 0), TILE_SIZED);

		if (gridX >= 0 && gridX < mapWidth && gridY >= 0 && gridY < mapHeight &&
				worldMap[gridY][gridX] != 0)
		{
			horizontalHitX = nextHorizontalTouchX;
			horizontalHitY = nextHorizontalTouchY;
			horizontalDistance = hypot(horizontalHitX - playerX, horizontalHitY - playerY);
			foundHorizontalWallHit = 1;
			break;
		}
		else
		{
			nextHorizontalTouchX += xstep;
			nextHorizontalTouchY += ystep;
		}
	}

	/* Vertical intersections */
	ystep = TILE_SIZED * tan(rayAngleRad);
	xstep = TILE_SIZED;

	if (isRayFacingRight)
	{
		nextVerticalTouchX = truncateDivisionFloat(playerX, TILE_SIZED) * TILE_SIZED + TILE_SIZED;
		nextVerticalTouchY = playerY + (nextVerticalTouchX - playerX) * tan(rayAngleRad);
		xstep = TILE_SIZED;
		ystep = TILE_SIZED * tan(rayAngleRad);
	}
	else
	{
		nextVerticalTouchX = truncateDivisionFloat(playerX, TILE_SIZED) * TILE_SIZED - 1;
		nextVerticalTouchY = playerY + (nextVerticalTouchX - playerX) * tan(rayAngleRad);
		xstep = -TILE_SIZED;
		ystep = -TILE_SIZED * tan(rayAngleRad);
	}

	while (nextVerticalTouchX >= 0 && nextVerticalTouchX < mapWidth * TILE_SIZED &&
			nextVerticalTouchY >= 0 && nextVerticalTouchY < mapHeight * TILE_SIZED)
	{
		gridX = truncateDivisionFloat(nextVerticalTouchX + (isRayFacingLeft ? -1 : 0), TILE_SIZED);
		gridY = truncateDivisionFloat(nextVerticalTouchY, TILE_SIZED);

		if (gridX >= 0 && gridX < mapWidth && gridY >= 0 && gridY < mapHeight &&
				worldMap[gridY][gridX] != 0)
		{
			verticalHitX = nextVerticalTouchX;
			verticalHitY = nextVerticalTouchY;
			verticalDistance = hypot(verticalHitX - playerX, verticalHitY - playerY);
			foundVerticalWallHit = 1;
			break;
		}
		else
		{
			nextVerticalTouchX += xstep;
			nextVerticalTouchY += ystep;
		}
	}

	/* Return the shortest distance */
	return (foundHorizontalWallHit && foundVerticalWallHit) ?
		fmin(horizontalDistance, verticalDistance) :
		(foundHorizontalWallHit ? horizontalDistance : verticalDistance);
}

/**
 * drawRay - Renders a ray on the screen from a player's position
 * @renderer: The SDL_Renderer to render the ray on
 * @playerX: The x-coordinate of the player's position
 * @playerY: The y-coordinate of the player's position
 * @rayAngle: The angle at which the ray is cast
 * @rayDistance: The distance the ray travels
 *
 * This function uses the SDL_Renderer to draw a ray originating from the
 * player's position, extending out at the specified angle and distance.
 * It calculates the end point of the ray based on the player's position,
 * the angle of the ray, and the distance the ray travels, then renders
 * a line from the player's position to the calculated end point.
 */

void drawRay(SDL_Renderer *renderer, float playerX, float playerY, float rayAngle, float rayDistance)
{
	float rayAngleRad = DEG_TO_RAD(rayAngle);

	/* Calculate the end point of the ray */
	float rayEndX = playerX + cos(rayAngleRad) * rayDistance;
	float rayEndY = playerY + sin(rayAngleRad) * rayDistance;

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); /* Red color */

	/* Draw the line representing the ray from the player to the end point */
	SDL_RenderDrawLine(renderer, (int)playerX, (int)playerY, (int)rayEndX, (int)rayEndY);
}

/**
 * drawWallSlice - Draws a vertical wall slice in the renderer.
 * @renderer: The SDL renderer used to draw the wall slice.
 * @rayIndex: The index of the ray corresponding to the column being drawn.
 * @wallHeight: The height of the wall slice to be drawn.
 *
 * This function draws a vertical line representing a wall slice in a
 * raycasting engine. The slice is drawn at the position specified by the ray
 * index, with the given height. The top and bottom of the wall slice are
 * calculated based on the height of the screen and the height of the
 * wall slice.
 */

void drawWallSlice(SDL_Renderer *renderer, int rayIndex, int wallHeight)
{
	int wallTop = (SCREEN_HEIGHT / 2) - (wallHeight / 2);
	int wallBottom = (SCREEN_HEIGHT / 2) + (wallHeight / 2);

	SDL_SetRenderDrawColor(renderer, 255, 253, 208, 255);
	SDL_RenderDrawLine(renderer, rayIndex, wallTop, rayIndex, wallBottom);
}
