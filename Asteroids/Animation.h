#pragma once
#include <SFML/Graphics.hpp>

class Animation{
public:
	float frame = 0;
	float speed = 0;
	sf::Sprite sprite;
	// the static keyword here functions similarly to declaring the array outside of the function
	sf::IntRect frames[8];	// the largest animation I have is 8 frames long so this is fine
	// number of frames in the animation
	int size;

	Animation();
	Animation(const Animation& other);
	Animation(sf::Texture &t, int x, int y, int w, int h, float scaleX, float scaleY, int frameCount, float frameSpeed);

	// tells you when this animation is over
	bool ended();
	// moves this animation forward by 1
	void update();

	void operator=(const Animation& other);
};

// copies from B to A
void copy(Animation& A, const Animation& B);