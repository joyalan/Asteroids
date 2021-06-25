#pragma once
#include "Entity.h"
class Player : public Entity {
public:
	bool thrust = false;

	Player();
	void update();
};

