#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"

// these are used for calculating rotations
const float PI = 3.14159265358979f;
const float DEGTORAD = PI / 180;
const float RADTODEG = 180 / PI;
// these are used to deal with entities going off screen, as well as selecting the dimensions of the whole app in the sfml-app.cpp.
const int W = 768;
const int H = 768;

class Entity
{
public:
	float x, y, dx, dy, radius, angle;
	bool life;
	Animation animation;
	enum entityName { player, asteroid, bullet, explosion };
	entityName name;

	// Constructor
	Entity();
	Entity(Animation &newAnim, int newX, int newY, float newAngle, float newRadius);

	// lets you change the settings for entities
	void settings(Animation &newAnim, int newX, int newY, float newAngle, float newRadius);

	// Updates the positions of the entity according to the speed defined by dx and dy
	virtual void update();
	
	// draws the entity in the window
	void draw(sf::RenderTarget &app);
};

// compares 2 entities and determines if they have collided
bool hasCollided(Entity A, Entity B);