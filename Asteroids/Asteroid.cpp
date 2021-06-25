#include "Asteroid.h"

Asteroid::Asteroid()
{
	dx = rand() % 6 - 3.0f;
	dy = rand() % 6 - 3.0f;
	name = Entity::asteroid;
}

void Asteroid::update()
{
	x += dx;
	y += dy;

	// fake "wrapping around" the edges
	if (x >= W) x = 0;  if (x < 0) x = W;
	if (y >= H) y = 0;  if (y < 0) y = H;
}
