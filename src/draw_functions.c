#include "../headers/mazemania.h"

/**
 * drawRay - Renders a ray from the player's position on the screen.
 * @renderer: The SDL_Renderer to render the ray on.
 * @playerX: The x-coordinate of the player's position.
 * @playerY: The y-coordinate of the player's position.
 * @rayAngle: The angle at which the ray is cast.
 * @rayDistance: The distance the ray travels.
 *
 * Description: This function takes the computed ray distance and angle, and
 * draws a representation of the ray from the player's position to its
 * termination point. This is used for visual debugging or for rendering
 * rays on a mini-map.
 */

void drawRay(SDL_Renderer *renderer, float playerX, float playerY,
		float rayAngle, float rayDistance)
{
	float rayAngleRad = DEG_TO_RAD(rayAngle);

	/* Calculate the end point of the ray */
	float rayEndX = playerX + cos(rayAngleRad) * rayDistance;
	float rayEndY = playerY + sin(rayAngleRad) * rayDistance;

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); /* Red color */

	/* Draw the line representing the ray from the player to the end point */
	SDL_RenderDrawLine(renderer, (int)playerX, (int)playerY,
			(int)rayEndX, (int)rayEndY);
}

/**
 * drawWallSlice - Draws a vertical slice of a wall on the renderer.
 * @renderer: The SDL_Renderer used for drawing.
 * @rayIndex: The index of the ray corresponding to the column being drawn.
 * @wallHeight: The calculated height of the wall slice.
 * @horizontalRay: Flag indicating if the hit was on a horizontal surface.
 * @verticalRay: Flag indicating if the hit was on a vertical surface.
 *
 * Description: This function renders a vertical line that represents a wall
 * slice in the rendered scene. The slice's position and height are calculated
 * from the raycasting process, and the function adjusts the rendering based on
 * whether the ray hit a horizontal or vertical surface, which can influence
 * the shading or texturing of the slice.
 */

void drawWallSlice(SDL_Renderer *renderer, int rayIndex, int wallHeight,
		int horizontalRay, int verticalRay, int level)
{
	int wallTop = (SCREEN_HEIGHT / 2) - (wallHeight / 2);
	int wallBottom = (SCREEN_HEIGHT / 2) + (wallHeight / 2);


	if (horizontalRay)
	{
		SDL_SetRenderDrawColor(renderer, 64, 64, 64, 255);
		SDL_RenderDrawLine(renderer, rayIndex, wallTop, rayIndex, wallBottom);
	}
	else if (verticalRay)
	{
		SDL_SetRenderDrawColor(renderer, 255, 253, 208, 255);
		SDL_RenderDrawLine(renderer, rayIndex, wallTop, rayIndex, wallBottom);
	}
}

/**
 * drawWallTexture - Renders a textured slice of a wall on the screen.
 * @renderer: Pointer to the SDL_Renderer used for drawing.
 * @rayIndex: The index of the ray corresponding to the wall slice being
 * rendered.
 * @wallHeight: The height of the wall slice to be drawn.
 * @texture: Pointer to the wallTexture structure containing texture data.
 * @texX: The x-coordinate on the texture to start drawing from.
 *
 * Description: This function is responsible for drawing a vertical slice of
 * a wall using a specific texture. The slice's height and position are
 * determined by raycasting calculations. The function uses the provided
 * texture data to render the wall with appropriate texturing effects,
 * simulating realistic surfaces within the scene. The `texX` parameter
 * allows the function to select the correct vertical strip of the texture,
 * aligning it with the ray's impact point on the wall, ensuring that texture
 * alignment appears continuous and accurate across multiple slices.
 */

void drawWallTexture(SDL_Renderer *renderer, int rayIndex, int wallHeight,
		wallTexture *texture, int texX)
{
	int drawStart = (SCREEN_HEIGHT / 2) - (wallHeight / 2);
	int drawEnd = (SCREEN_HEIGHT / 2) + (wallHeight / 2);
	if (drawStart < 0)
		drawStart = 0;
	if (drawEnd >= SCREEN_HEIGHT)
		drawEnd = SCREEN_HEIGHT - 1;

	for (int y = drawStart; y < drawEnd; y++)
	{
		int d = y * 256 - SCREEN_HEIGHT * 128 + wallHeight * 128;
		int texY = ((d * texture->height) / wallHeight) / 256;
		Uint32 color = texture->pixels[texY * texture->width + texX];

		Uint8 r, g, b;
		r = (color >> 16) & 0xFF;
		g = (color >> 8) & 0xFF;
		b = color & 0xFF;

		SDL_SetRenderDrawColor(renderer, r, g, b, 255);
		SDL_RenderDrawPoint(renderer, rayIndex, y);
	}
}

/**
 * drawFloor - Renders the floor texture.
 * @instance: Pointer to an SDL_Instance containing rendering and
 * game state information.
 * @playerX: The x-coordinate of the player's position.
 * @playerY: The y-coordinate of the player's position.
 * @playerRotation: The current rotation angle of the player.
 * @floorTexture: Pointer to the floor texture structure.
 */

void drawFloor(SDL_Instance *instance, float playerX, float playerY,
		float playerAngle, wallTexture *floorTexture)
{
	for (int y = SCREEN_HEIGHT / 2 + 1; y < SCREEN_HEIGHT; y++)
	{
		float rayDirX0 = cos(DEG_TO_RAD(playerAngle - FOV_ANGLE / 2));
		float rayDirY0 = sin(DEG_TO_RAD(playerAngle - FOV_ANGLE / 2));
		float rayDirX1 = cos(DEG_TO_RAD(playerAngle + FOV_ANGLE / 2));
		float rayDirY1 = sin(DEG_TO_RAD(playerAngle + FOV_ANGLE / 2));

		int p = y - SCREEN_HEIGHT / 2;
		float posZ = 0.5 * SCREEN_HEIGHT;
		float rowDistance = posZ / p;

		float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / SCREEN_WIDTH;
		float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / SCREEN_WIDTH;

		float floorX = playerX + rowDistance * rayDirX0;
		float floorY = playerY + rowDistance * rayDirY0;

		for (int x = 0; x < SCREEN_WIDTH; x++) {
			int cellX = (int)floorX;
			int cellY = (int)floorY;

			int tx = (int)(floorTexture->width *
					(floorX - cellX)) & (floorTexture->width - 1);
			int ty = (int)(floorTexture->height *
					(floorY - cellY)) & (floorTexture->height - 1);

			Uint32 floorColor = ((Uint32 *)floorTexture->pixels)[ty *
				floorTexture->width + tx];

			Uint8 r, g, b;
			r = (floorColor >> 16) & 0xFF;
			g = (floorColor >> 8) & 0xFF;
			b = floorColor & 0xFF;

			SDL_SetRenderDrawColor(instance->renderer, r, g, b, 255);
			SDL_RenderDrawPoint(instance->renderer, x, y);

			floorX += floorStepX;
			floorY += floorStepY;
		}
	}
}


/**
 * drawCeiling - Renders the ceiling texture.
 * @instance: Pointer to an SDL_Instance containing rendering and
 * game state information.
 * @playerX: The x-coordinate of the player's position.
 * @playerY: The y-coordinate of the player's position.
 * @playerRotation: The current rotation angle of the player.
 * @ceilingTexture: Pointer to the ceiling texture structure.
 */

void drawCeiling(SDL_Instance *instance, float playerX, float playerY,
		float playerAngle, wallTexture *ceilingTexture)
{
	for (int y = 0; y < SCREEN_HEIGHT / 2; y++)
	{
		float rayDirX0 = cos(DEG_TO_RAD(playerAngle - FOV_ANGLE / 2));
		float rayDirY0 = sin(DEG_TO_RAD(playerAngle - FOV_ANGLE / 2));
		float rayDirX1 = cos(DEG_TO_RAD(playerAngle + FOV_ANGLE / 2));
		float rayDirY1 = sin(DEG_TO_RAD(playerAngle + FOV_ANGLE / 2));

		int p = SCREEN_HEIGHT / 2 - y;  /* Invert p for ceiling */
		float posZ = 0.5 * SCREEN_HEIGHT;
		float rowDistance = posZ / p;

		float ceilingStepX = rowDistance * (rayDirX1 - rayDirX0) / SCREEN_WIDTH;
		float ceilingStepY = rowDistance * (rayDirY1 - rayDirY0) / SCREEN_WIDTH;

		float ceilingX = playerX + rowDistance * rayDirX0;
		float ceilingY = playerY + rowDistance * rayDirY0;

		for (int x = 0; x < SCREEN_WIDTH; x++) {
			int cellX = (int)ceilingX;
			int cellY = (int)ceilingY;

			int tx = (int)(ceilingTexture->width *
					(ceilingX - cellX)) & (ceilingTexture->width - 1);
			int ty = (int)(ceilingTexture->height
					* (ceilingY - cellY)) & (ceilingTexture->height - 1);

			Uint32 ceilingColor = ((Uint32 *)ceilingTexture->pixels)[ty *
				ceilingTexture->width + tx];

			Uint8 r, g, b;
			r = (ceilingColor >> 16) & 0xFF;
			g = (ceilingColor >> 8) & 0xFF;
			b = ceilingColor & 0xFF;

			SDL_SetRenderDrawColor(instance->renderer, r, g, b, 255);
			SDL_RenderDrawPoint(instance->renderer, x, y);

			ceilingX += ceilingStepX;
			ceilingY += ceilingStepY;
		}
	}
}
