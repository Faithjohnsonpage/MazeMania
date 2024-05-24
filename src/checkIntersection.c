#include "../headers/mazemania.h"

/**
 * checkIntersection - Checks for intersection between two SDL_Rect structures.
 * @A: Pointer to the first SDL_Rect structure.
 * @B: Pointer to the second SDL_Rect structure.
 *
 * This function determines whether there is an intersection between two
 * SDL_Rect structures representing rectangular areas on the screen.
 * It returns 1 if there is an intersection, and 0 otherwise.
 *
 * Return: 1 if there is an intersection, 0 otherwise.
 */

int checkIntersection(const SDL_Rect *A, const SDL_Rect *B)
{
	/* Calculate the sides of rect A */
	int leftA = A->x;
	int rightA = A->x + A->w;
	int topA = A->y;
	int bottomA = A->y + A->h;

	/* Calculate the sides of rect B */
	int leftB = B->x;
	int rightB = B->x + B->w;
	int topB = B->y;
	int bottomB = B->y + B->h;

	/* Check for intersection */
	if (bottomA <= topB || topA >= bottomB || rightA <= leftB || leftA >= rightB)
	{
		return (0); /* No intersection */
	}
	else
	{
		return (1); /* Intersection */
	}
}
