#include "Bullet.h"

Bullet::Bullet() {
	name = Entity::bullet;
}

void Bullet::update() {
	dx = (float)cos(angle*DEGTORAD) * 6;
	dy = (float)sin(angle*DEGTORAD) * 6;

	x += dx;
	y += dy;

	if (x > W || x < 0 ||
		y > H || y < 0)
	{
		life = false;
		//x -= dx;
		//y -= dy;
	}
}