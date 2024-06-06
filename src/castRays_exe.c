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
 * castRays - Casts multiple rays from the player's position to simulate vision
 * @instance: Pointer to an SDL_Instance containing rendering and game
 * state information.
 * @playerX: The x-coordinate of the player's position.
 * @playerY: The y-coordinate of the player's position.
 * @playerRotation: The current rotation angle of the player.
 * @isMiniMap: Flag to determine if the casting is for a mini-map.
 * @wallTexture: Pointer to the wall texture structure.
 *
 * Description: This function casts multiple rays from the player’s
 * current position in various directions based on the player's rotation.
 * It utilizes the game's rendering context and state to determine the
 * visibility and rendering of walls and obstacles within the player’s field
 * of view. The function can be tailored to either the main game view or a
 * mini-map, adjusting the fidelity and scale of raycasting accordingly.
 */

void castRays(SDL_Instance *instance, float playerX, float playerY,
		float playerRotation, bool isMiniMap, wallTexture *wallTexture)
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
		castSingleRay(playerX, playerY, rayAngle, scale, instance,
				playerRotation, isMiniMap, wallTexture, ray);
	}
}

/**
 * castSingleRay - Casts a single ray to calculate the distance to an obstacle.
 * @playerX: The x-coordinate of the player's starting position.
 * @playerY: The y-coordinate of the player's starting position.
 * @rayAngle: The angle at which the ray is cast, relative to the player.
 * @scale: The scale factor for the ray, differentiating between main
 * view and mini-map.
 * @instance: Pointer to the SDL_Instance for rendering and state management.
 * @playerRotation: The player's current rotation for correcting perspective.
 * @isMiniMap: Flag to indicate mini-map calculation.
 * @wallTexture: Pointer to the texture of the wall.
 * @ray: The index of the current ray in the raycasting sequence.
 *
 * Description: Function performs the calculation of a single ray’s travel
 * distance until it hits an obstacle. The calculation accounts for both
 * vertical and horizontal distances, adjusting for perspective errors caused
 * by the angle of incidence and player rotation. It supports both main and
 * mini-map functionalities by adjusting the scale of measurements.
 */

void castSingleRay(float playerX, float playerY, float rayAngle, float scale,
		SDL_Instance *instance, float playerRotation, bool isMiniMap,
		wallTexture *wallTexture, int ray)
{
	/* Convert angle to radians for trigonometric functions */
	float rayAngleRad = DEG_TO_RAD(rayAngle);
	float TILE_SIZED = TILE_SIZE * scale;

	/* Define variables to track intersection points */
	float horizontalHitX, horizontalHitY, verticalHitX, verticalHitY;
	float horizontalDistance = INFINITY, verticalDistance = INFINITY;
	int foundHorizontalWallHit = 0, foundVerticalWallHit = 0;
	int gridX, gridY, wallHeight;

	int isRayFacingDown = (rayAngle > 0 && rayAngle < 180);
	int isRayFacingUp = !isRayFacingDown;
	int isRayFacingRight = (rayAngle < 90 || rayAngle > 270);
	int isRayFacingLeft = !isRayFacingRight;

	/* Horizontal intersections */
	float ystep = TILE_SIZED;
	float xstep = (TILE_SIZED) / tan(rayAngleRad);

	float nextHorizontalTouchX, nextHorizontalTouchY, nextVerticalTouchX;
	float nextVerticalTouchY;

	if (isRayFacingDown)
	{
		nextHorizontalTouchY = truncateDivisionFloat(playerY, (TILE_SIZED)) *
			TILE_SIZED + TILE_SIZED;
		nextHorizontalTouchX = playerX + (nextHorizontalTouchY - playerY) /
			tan(rayAngleRad);
		ystep = TILE_SIZED;
		xstep = TILE_SIZED / tan(rayAngleRad);
	}
	else
	{
		nextHorizontalTouchY = truncateDivisionFloat(playerY, TILE_SIZED) *
			TILE_SIZED - 1;
		nextHorizontalTouchX = playerX + (nextHorizontalTouchY - playerY) /
			tan(rayAngleRad);
		ystep = -TILE_SIZED;
		xstep = -TILE_SIZED / tan(rayAngleRad);
	}

	while (nextHorizontalTouchX >= 0 &&
			nextHorizontalTouchX < mapWidth * TILE_SIZED &&
			nextHorizontalTouchY >= 0 &&
			nextHorizontalTouchY < mapHeight * TILE_SIZED)
	{
		gridX = truncateDivisionFloat(nextHorizontalTouchX, TILE_SIZED);
		gridY = truncateDivisionFloat(nextHorizontalTouchY +
				(isRayFacingUp ? -1 : 0), TILE_SIZED);

		if (gridX >= 0 && gridX < mapWidth && gridY >= 0 &&
				gridY < mapHeight && worldMap[gridY][gridX] != 0)
		{
			horizontalHitX = nextHorizontalTouchX;
			horizontalHitY = nextHorizontalTouchY;
			horizontalDistance = hypot(horizontalHitX - playerX,
					horizontalHitY - playerY);
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
		nextVerticalTouchX = truncateDivisionFloat(playerX, TILE_SIZED) *
			TILE_SIZED + TILE_SIZED;
		nextVerticalTouchY = playerY + (nextVerticalTouchX - playerX) *
			tan(rayAngleRad);
		xstep = TILE_SIZED;
		ystep = TILE_SIZED * tan(rayAngleRad);
	}
	else
	{
		nextVerticalTouchX = truncateDivisionFloat(playerX, TILE_SIZED) *
			TILE_SIZED - 1;
		nextVerticalTouchY = playerY + (nextVerticalTouchX - playerX) *
			tan(rayAngleRad);
		xstep = -TILE_SIZED;
		ystep = -TILE_SIZED * tan(rayAngleRad);
	}

	while (nextVerticalTouchX >= 0 &&
			nextVerticalTouchX < mapWidth * TILE_SIZED &&
			nextVerticalTouchY >= 0 &&
			nextVerticalTouchY < mapHeight * TILE_SIZED)
	{
		gridX = truncateDivisionFloat(nextVerticalTouchX +
				(isRayFacingLeft ? -1 : 0), TILE_SIZED);
		gridY = truncateDivisionFloat(nextVerticalTouchY, TILE_SIZED);

		if (gridX >= 0 && gridX < mapWidth && gridY >= 0 &&
				gridY < mapHeight && worldMap[gridY][gridX] != 0)
		{
			verticalHitX = nextVerticalTouchX;
			verticalHitY = nextVerticalTouchY;
			verticalDistance = hypot(verticalHitX - playerX,
					verticalHitY - playerY);
			foundVerticalWallHit = 1;
			break;
		}
		else
		{
			nextVerticalTouchX += xstep;
			nextVerticalTouchY += ystep;
		}
	}

	/* Determine the shortest distance */
	float rayDistance;
	int verticalRay = 0, horizontalRay = 0;

	if (foundHorizontalWallHit && foundVerticalWallHit)
	{
		if (horizontalDistance < verticalDistance)
		{
			rayDistance = horizontalDistance;
			horizontalRay = 1;
		}
		else
		{
			rayDistance = verticalDistance;
			verticalRay = 1;
		}
	}
	else if (foundHorizontalWallHit)
	{
		rayDistance = horizontalDistance;
		horizontalRay = 1;
	}
	else if (foundVerticalWallHit)
	{
		rayDistance = verticalDistance;
		verticalRay = 1;
	}

	if (isMiniMap)
	{
		drawRay(instance->renderer, playerX, playerY, rayAngle, rayDistance);
	}
	else
	{
		/* Calculate the projected wall height */
		float correctedDistance = rayDistance * cos(DEG_TO_RAD
				(rayAngle - playerRotation));
		wallHeight = (int)((TILE_SIZE / correctedDistance) *
				DIST_TO_PROJ_PLANE);

		/* Calculate texture X coordinate */
		float wallX = (playerY + rayDistance * sin(DEG_TO_RAD(rayAngle))) /
			TILE_SIZE;
		int texX = (int)(wallX * (float)wallTexture->width) %
			wallTexture->width;

		/* Draw the wall slice */
		drawWallTexture(instance->renderer, ray,
				wallHeight, wallTexture, texX);

		/* Draw the wall slice */
		/*drawWallSlice(instance->renderer, ray, wallHeight,
				verticalRay, horizontalRay);*/
	}
}
