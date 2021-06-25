#include "Player.h"

Player::Player(){
	name = Entity::player;
}

void Player::update()
{
	if (thrust) {
		dx += cos(angle*DEGTORAD) * 0.2f;
		dy += sin(angle*DEGTORAD) * 0.2f;
	}
	else {	// if it's not moving forward, it's slowing down
		dx *= 0.99f;
		dy *= 0.99f;
	}

	int maxSpeed = 16;
	float speed = sqrt(dx * dx + dy * dy);
	if (speed > maxSpeed) {	// cap the speed
		dx *= maxSpeed / speed;
		dy *= maxSpeed / speed;
	}
	
	x += dx;
	y += dy;

	// fake "wrapping around" the edges
	if (x > W)
		x = 0; 
	if (x < 0)
		x = W;
	if (y > H)
		y = 0;
	if (y < 0)
		y = H;
}
