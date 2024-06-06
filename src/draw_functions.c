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
		int horizontalRay, int verticalRay)
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

void drawWallTexture(SDL_Renderer *renderer, int rayIndex,
		int wallHeight, wallTexture *texture, int texX)
{
	int drawStart = -wallHeight / 2 + SCREEN_HEIGHT / 2;
	int drawEnd = wallHeight / 2 + SCREEN_HEIGHT / 2;

	if (drawStart < 0)
		drawStart = 0;
	if (drawEnd >= SCREEN_HEIGHT)
		drawEnd = SCREEN_HEIGHT - 1;

	for (int y = drawStart; y < drawEnd; y++)
	{
		int d = y * 256 - SCREEN_HEIGHT * 128 + wallHeight * 128;
		int texY = ((d * texture->height) / wallHeight) / 256;
		Uint32 color = getTexturePixel(texture, texX, texY);

		SDL_SetRenderDrawColor(renderer, (color & 0xFF0000) >> 16,
				(color & 0xFF00) >> 8, color & 0xFF, 0xFF);
		SDL_RenderDrawPoint(renderer, rayIndex, y);
	}
}
