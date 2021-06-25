#include "Animation.h"

Animation::Animation() {}

Animation::Animation(const Animation & other) {
	copy(*this, other);
}

Animation::Animation(sf::Texture &t, int x, int y, int w, int h, float scaleX, float scaleY, int frameCount, float frameSpeed) {
	frame = 0;
	speed = frameSpeed;
	size = frameCount;
	for (int i = 0; i < size; i++) {
		frames[i] = sf::IntRect(x + i*w, y, w, h);
	}
	
	sprite.setTexture(t);
	sprite.setOrigin((float) w / 2, (float) h / 2);
	sprite.setScale(scaleX, scaleY);
	sprite.setTextureRect(frames[0]);
}

void Animation::update() {
	frame += speed;
	// loops the animation
	if (frame >= size) {
		frame -= size;
	}
	if (size > 0) {
		sprite.setTextureRect(frames[(int)frame]);
	}
}

bool Animation::ended() {
	return frame+speed >= size;
}

void Animation::operator=(const Animation & other)
{
	copy(*this, other);
}

void copy(Animation& A, const Animation& B) {
	A.frame = B.frame;
	A.speed = B.speed;
	A.size = B.size;
	A.sprite = B.sprite;
	if (B.sprite.getTexture() != NULL) {
		A.sprite.setTexture(*B.sprite.getTexture());
		A.sprite.setOrigin(B.sprite.getOrigin());
		A.sprite.setScale(B.sprite.getScale());
		A.sprite.setTextureRect(B.sprite.getTextureRect());
	}
	for (int i = 0; i < A.size; i++) {
		A.frames[i] = B.frames[i];
	}
}