#include "Entity.h"

// Constructor
Entity::Entity() {
	life = true;
}

Entity::Entity(Animation & newAnim, int newX, int newY, float newAngle, float newRadius)
{
	Entity::settings(newAnim, newX, newY, newAngle, newRadius);
}

// lets you change the settings for entities
void Entity::settings(Animation &newAnim, int newX, int newY, float newAngle, float newRadius) {
  	this->x = (float)newX;
	this->y = (float)newY;
	this->angle = newAngle;
	this->radius = newRadius;
	this->animation = newAnim;
	this->life = true;
}

// lets you update the position of an entity every tick.
// it's virtual so it doesn't need to be expanded upon here
void Entity::update() {}

void Entity::draw(sf::RenderTarget& app) {
	animation.sprite.setPosition(x, y);
	animation.sprite.setRotation(angle + 90);
	app.draw(this->animation.sprite);

	sf::CircleShape circle(this->radius * this->animation.sprite.getScale().x);
	circle.setFillColor(sf::Color(255, 0, 0, 170));
	circle.setPosition(x, y);
	circle.setOrigin(this->radius * this->animation.sprite.getScale().x, this->radius * this->animation.sprite.getScale().x);
	//app.draw(circle);
}

bool hasCollided(Entity A, Entity B) {
	// compares distance between the two objects with the combined radiuses of the two objects.
	float AscaleX = A.animation.sprite.getScale().x;
	float AscaleY = A.animation.sprite.getScale().y;
	float BscaleX = B.animation.sprite.getScale().x;
	float BscaleY = B.animation.sprite.getScale().y;
	float distanceSq = ((B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y));
	float distanceBtwn = (A.radius * AscaleX) + (B.radius * BscaleY);
	return distanceSq < (distanceBtwn * distanceBtwn);
			
}