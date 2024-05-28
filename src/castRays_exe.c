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
 *
 * This function casts multiple rays from the player's position in different
 * directions based on the player's rotation. It utilizes the SDL_Instance
 * to manage rendering and game state, and it calculates the distance each
 * ray travels before hitting an obstacle. The function is used to simulate
 * the player's field of view and render the environment accordingly.
 */

void castRays(SDL_Instance *instance, float playerX, float playerY, float playerRotation)
{
	float rayAngle;
	float rayDistance;
	float angleIncrement = FOV_ANGLE / (float)NUM_RAYS;

	/* Normalize the player's rotation angle to [0, 360) range */
	playerRotation = fmod(playerRotation, 360);
	if (playerRotation < 0)
	{
		playerRotation += 360;
	}

	for (int ray = 0; ray < NUM_RAYS; ray++)
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
		rayDistance = castSingleRay(playerX, playerY, rayAngle);

		/* Draw the ray */
		drawRay(instance->renderer, playerX, playerY, rayAngle, rayDistance);
	}
}

/**
 * castSingleRay - Casts a single ray and calculates its distance
 * @playerX: The x-coordinate of the player's position
 * @playerY: The y-coordinate of the player's position
 * @rayAngle: The angle at which the ray is cast
 *
 * This function casts a single ray from the player's position at the specified
 * angle and calculates the distance the ray travels before hitting an obstacle
 * The function returns the distance from the player's position to the point of
 * intersection with the obstacle.
 *
 * Return: The distance the ray travels before hitting an obstacle.
 */

float castSingleRay(float playerX, float playerY, float rayAngle)
{
	/* Convert angle to radians for trigonometric functions */
	float rayAngleRad = DEG_TO_RAD(rayAngle);

	/* Define variables to track intersection points */
	float horizontalHitX, horizontalHitY, verticalHitX, verticalHitY;
	float horizontalDistance = INFINITY, verticalDistance = INFINITY;
	int foundHorizontalWallHit = 0, foundVerticalWallHit = 0;

	int isRayFacingDown = (rayAngle > 0 && rayAngle < 180);
	int isRayFacingUp = !isRayFacingDown;
	int isRayFacingRight = (rayAngle < 90 || rayAngle > 270);
	int isRayFacingLeft = !isRayFacingRight;

	/* Horizontal intersections */
	float ystep = TILE_SIZE;
	float xstep = TILE_SIZE / tan(rayAngleRad);

	float nextHorizontalTouchX, nextHorizontalTouchY;

	if (isRayFacingDown)
	{
		nextHorizontalTouchY = truncateDivisionFloat(playerY, TILE_SIZE) * TILE_SIZE + TILE_SIZE;
		nextHorizontalTouchX = playerX + (nextHorizontalTouchY - playerY) / tan(rayAngleRad);
		ystep = TILE_SIZE;
		xstep = TILE_SIZE / tan(rayAngleRad);
	}
	else
	{
		nextHorizontalTouchY = truncateDivisionFloat(playerY, TILE_SIZE) * TILE_SIZE - 1;
		nextHorizontalTouchX = playerX + (nextHorizontalTouchY - playerY) / tan(rayAngleRad);
		ystep = -TILE_SIZE;
		xstep = -TILE_SIZE / tan(rayAngleRad);
	}

	while (nextHorizontalTouchX >= 0 && nextHorizontalTouchX < mapWidth * TILE_SIZE &&
			nextHorizontalTouchY >= 0 && nextHorizontalTouchY < mapHeight * TILE_SIZE)
	{
		int gridX = truncateDivisionFloat(nextHorizontalTouchX, TILE_SIZE);
		int gridY = truncateDivisionFloat(nextHorizontalTouchY + (isRayFacingUp ? -1 : 0), TILE_SIZE);

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
	ystep = TILE_SIZE * tan(rayAngleRad);
	xstep = TILE_SIZE;

	float nextVerticalTouchX, nextVerticalTouchY;

	if (isRayFacingRight)
	{
		nextVerticalTouchX = truncateDivisionFloat(playerX, TILE_SIZE) * TILE_SIZE + TILE_SIZE;
		nextVerticalTouchY = playerY + (nextVerticalTouchX - playerX) * tan(rayAngleRad);
		xstep = TILE_SIZE;
		ystep = TILE_SIZE * tan(rayAngleRad);
	}
	else
	{
		nextVerticalTouchX = truncateDivisionFloat(playerX, TILE_SIZE) * TILE_SIZE - 1;
		nextVerticalTouchY = playerY + (nextVerticalTouchX - playerX) * tan(rayAngleRad);
		xstep = -TILE_SIZE;
		ystep = -TILE_SIZE * tan(rayAngleRad);
	}

	while (nextVerticalTouchX >= 0 && nextVerticalTouchX < mapWidth * TILE_SIZE &&
			nextVerticalTouchY >= 0 && nextVerticalTouchY < mapHeight * TILE_SIZE)
	{
		int gridX = truncateDivisionFloat(nextVerticalTouchX + (isRayFacingLeft ? -1 : 0), TILE_SIZE);
		int gridY = truncateDivisionFloat(nextVerticalTouchY, TILE_SIZE);

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
