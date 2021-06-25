#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>
#include "Animation.h"
#include "Entity.h"
#include "Player.h"
#include "Asteroid.h"
#include "Bullet.h"
#include "EntityList.h"
#include "SpatialHash.h"

// creates the explosion animation
void createExplosion(Entity eExplosions[10], EntityList& eList, Entity*& targetEntity, Animation& aExplosion, int& currentExplosion) {
	eExplosions[currentExplosion].settings(aExplosion, (int) targetEntity->x, (int) targetEntity->y, 0.0f, 8.0f);
	eExplosions[currentExplosion].name = Entity::explosion;
	eList.createEntity()->value = &eExplosions[currentExplosion];
	currentExplosion++;
	if (currentExplosion >= 10) {
		currentExplosion = 0;
	}
}

// handles when a bullet hits an asteroid
void bulletCollision(Entity* eAsteroids, EntityList& eList, Entity*& targetEntity, Animation& aAsteroid, int& currentSize, int& sizeCount){
	// calculate positions of child asteroids
	float initialAngle;
	// angles have to be adjusted, because the origin is in the top left, and positive Y is towards the bottom of the screen
	if (targetEntity->dx != 0) {	// avoid division by 0 
		initialAngle = RADTODEG * atanf(targetEntity->dy / targetEntity->dx);	// angle of initial asteroid, before being broken
		//if (targetEntity->dx < 0 && targetEntity->dy < 0) initialAngle += 180;	// the range of angles where both x and y are negative is 180 to 270
		if (targetEntity->dx < 0) initialAngle += 180;						// the range of angles where x is negative is 90 to 270
	}
	else {
		if (targetEntity->dy > 0)	initialAngle = 90;		// straight down = 90
		else						initialAngle = 270;		// straight up = 270
	}
	int astX = (int) targetEntity->x;
	int astY = (int) targetEntity->y;
	int astScale = (int) targetEntity->animation.sprite.getScale().x;
	// calculate angles for both asteroids
	float angle1 = (initialAngle - 30) * DEGTORAD;
	float angle2 = (initialAngle + 30) * DEGTORAD;
	// adjust their positions from the initial asteroid's position relative to its movement vector and size
	int posMod = 12 * astScale;
	int astX1 = astX + (int) (posMod * cos(angle1));
	int astY1 = astY + (int) (posMod * sin(angle1));
	int astX2 = astX + (int) (posMod * cos(angle2));
	int astY2 = astY + (int) (posMod * sin(angle2));

	// configure the asteroid
	eAsteroids[currentSize].settings(aAsteroid, astX1, astY1, (float)(rand() % 360), 8.0f);
	eAsteroids[currentSize + 1].settings(aAsteroid, astX2, astY2, (float)(rand() % 360), 8.0f);
	// scale them down
	eAsteroids[currentSize].animation.sprite.setScale(astScale - 1.0f, astScale - 1.0f);
	eAsteroids[currentSize + 1].animation.sprite.setScale(astScale - 1.0f, astScale - 1.0f);
	// calculate angle and speed of child asteroids
	float initialMag = sqrt((targetEntity->dx * targetEntity->dx) + (targetEntity->dy * targetEntity->dy));
	float finalMag = initialMag * (2.0f - .2f * astScale);
	float ast1dx = finalMag * cos(angle1);
	float ast1dy = finalMag * sin(angle1);
	float ast2dx = finalMag * cos(angle2);
	float ast2dy = finalMag * sin(angle2);
	eAsteroids[currentSize].dx = ast1dx;
	eAsteroids[currentSize].dy = ast1dy;
	eAsteroids[currentSize+1].dx = ast2dx;
	eAsteroids[currentSize+1].dy = ast2dy;
	// add them to the entity list
	eList.createEntity()->value = &eAsteroids[currentSize];
	eList.createEntity()->value = &eAsteroids[currentSize + 1];
	// increment the list
	currentSize += 2;
	sizeCount += 2;
}

// uses momentum and kinetic energy equations to handle when one asteroid hits another asteroid
void asteroidBounce(Entity*& A, Entity*& B) {
	float Ascale = A->animation.sprite.getScale().x;
	float Bscale = B->animation.sprite.getScale().x;
	// make it so that the mass decreases by half every time an asteroid gets smaller
	if (Ascale != 4) Ascale -= 1;
	if (Bscale != 4) Bscale -= 1;
	// treating scale as mass
	float massSum = Ascale + Bscale;
	// momentum & kinetic energy eq uations combined for this
	float Adxf = ((Ascale - Bscale)*A->dx + (2 * Bscale)*B->dx) / (massSum);
	float Adyf = ((Ascale - Bscale)*A->dy + (2 * Bscale)*B->dy) / (massSum);
	float Bdxf = ((2 * Ascale)*A->dx + (Bscale - Ascale)*B->dx) / massSum;
	float Bdyf = ((2 * Ascale)*A->dy + (Bscale - Ascale)*B->dy) / massSum;
	// change the values to the calculated values
	A->dx = Adxf;
	A->dy = Adyf;
	B->dx = Bdxf;
	B->dy = Bdyf;
	// immediately change the positions of the asteroids
	A->x += A->dx;
	A->y += A->dy;
	B->x += B->dx;
	B->y += B->dy;
}


int main()
{
	// set up rand 
	srand(static_cast<unsigned int>(time(0)));

	// set up the window 
	sf::RenderWindow app(sf::VideoMode(W, H), "asteroids");
	app.setFramerateLimit(60);

	// set up textures
	sf::Texture t1, t2, t3, t4, t5;
	t1.loadFromFile("images/SF-ST.png");	// space ship from https://kpdwyer.itch.io/lo-fi-stellar-skirmish
	t2.loadFromFile("images/space.png");	// background
	t3.loadFromFile("images/Large Shot.png");	// bullets from https://kpdwyer.itch.io/lo-fi-stellar-skirmish
	t4.loadFromFile("images/Crafting_79.png");	// asteroid from https://s4m-ur4i.itch.io/huge-pixelart-asset-pack
	t5.loadFromFile("images/Explosion.png");	// explosion from https://kpdwyer.itch.io/lo-fi-stellar-skirmish
	sf::Sprite sBackground(t2);
	Animation aPlayer(t1, 0, 0, 32, 32, 1.0f, 1.0f, 1, 1.0f);
	Animation aBullet(t3, 0, 0, 16, 16, 1.5f, 1.5f, 6, .5f);
	Animation aAsteroid(t4, 0, 0, 16, 16, 4.0f, 4.0f, 1, 1.0f);
	Animation aExplosion(t5, 0, 0, 16, 16, 3.0f, 3.0f, 8, .5f);
	// set up entity list
	EntityList entities;

	// set up player
	Player ePlayer;

	// set up asteroids
	int astX, astY;
	const int initialCount = 2;	// max number of asteroids
	float astAngle;
	sf::Vector2f ast1, ast2;
	Asteroid eLargeAsteroids[initialCount];
	Asteroid eMediumAsteroids[initialCount * 2];
	Asteroid eSmallAsteroids[initialCount * 4];
	int largeCount = initialCount, mediumCount = initialCount*2, smallCount = initialCount*4;
	int currentMedium, currentSmall;

	Entity eExplosions[10];
	int currentExplosion, currentBullet, bulletCount;
	// set up bullets
	Bullet eBullets[5];

	// set up collision management
	SpatialHash SH(64);

	// set up font
	sf::Font font;
	if (!font.loadFromFile("font/Kpddefault.ttf")) {
		return EXIT_FAILURE;
	}

	sf::FloatRect stringLocalBounds;
	
	sf::Text restartText;
	restartText.setString("press 'R' to restart");
	restartText.setFont(font);
	restartText.setCharacterSize(48);
	restartText.setFillColor(sf::Color::White);
	stringLocalBounds = restartText.getLocalBounds();
	restartText.setOrigin(stringLocalBounds.width/2.0f, stringLocalBounds.height/2.0f);
	restartText.setPosition(W / 2, H / 2 + 36);

	// set up score system
	sf::Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(42);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(0, -21);
	sf::String scoreString = "SCORE: ";

	int score = 0;

	// game over text
	sf::Text gameOverText;
	gameOverText.setString("GAME OVER");
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(72);
	gameOverText.setFillColor(sf::Color::White);
	stringLocalBounds = gameOverText.getLocalBounds();
	gameOverText.setOrigin(stringLocalBounds.width / 2.0f, stringLocalBounds.height / 2.0f);

	gameOverText.setPosition(W / 2, H / 2 - 72);

	bool drawGameOver = false;

	// win text
	sf::Text youWinText;
	youWinText.setString("YOU WIN!");
	youWinText.setFont(font);
	youWinText.setCharacterSize(72);
	youWinText.setFillColor(sf::Color::White);
	stringLocalBounds = youWinText.getLocalBounds();
	youWinText.setOrigin(stringLocalBounds.width / 2.0f, stringLocalBounds.height / 2.0f);
	youWinText.setPosition(W / 2, H / 2 - 72);

	bool drawWin = false;

	bool resetGame = true;	// we'll use this also as a start state for the game

	// actual game rendering
	while (app.isOpen()) {
		if (resetGame) {	// when the game either starts for the first time or you press the "R" button
			SH.clearBuckets();
			entities.clear();

			ePlayer.settings(aPlayer, W / 2 - 16, H / 2 - 16, 270.0f, 16.0f);
			ePlayer.life = true;
			entities.createEntity()->value = &ePlayer;
			// spawn asteroids
			for (int i = 0; i < initialCount; i++) {
				astX = rand() % W;
				astY = rand() % H;
				astAngle = (float)(rand() % 360);
				eLargeAsteroids[i].settings(aAsteroid, astX, astY, astAngle, 8.0f);
				entities.createEntity()->value = &eLargeAsteroids[i];
			}
			// set a TON of variables to starting values.
			largeCount = initialCount;
			mediumCount = 0;
			smallCount = 0;
			currentMedium = 0;
			currentSmall = 0;

			score = 0;
			currentExplosion = 0;
			currentBullet = 0;
			bulletCount = 0;

			drawWin = false;
			drawGameOver = false;
			resetGame = false;
		}
		
		// player presses "R"
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
			resetGame = true;
			continue;	// skip everything else in this loop
		}


		sf::Event event;
		while (app.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				app.close();
			if (bulletCount < 5 && ePlayer.life) {	// bullet input
				if (event.type == sf::Event::KeyPressed) {
					if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S) {
						eBullets[currentBullet].settings(aBullet, (int)ePlayer.x, (int)ePlayer.y, ePlayer.angle, 8.0f);
						entities.createEntity()->value = &eBullets[currentBullet];

						bulletCount++;
						currentBullet++;
						if (currentBullet >= 5)
							currentBullet = 0;
					}
				}
			}
		}
		// draw the background
		app.clear();
		app.draw(sBackground);

		// check if there are any remaining asteroids
		if (largeCount == 0 && mediumCount == 0 && smallCount == 0) {
			drawWin = true;
		}

		// handle player input
		if (ePlayer.life) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				ePlayer.angle += 3;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				ePlayer.angle -= 3;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
				ePlayer.thrust = true;
			}
			else ePlayer.thrust = false;
		}

		// clear the spatial hash buckets
		SH.clearBuckets();

		// draw all entities
		auto itr = entities.begin();
		while (itr != entities.end()) {
			itr->value->update();
			itr->value->draw(app);
			itr->value->animation.update();
			SH.registerObject(itr);
			if (itr->value->name == Entity::explosion) {
				if (itr->value->animation.ended())
					itr->value->life = false;
			}
			if (itr->value->life == false) {
				if (itr->value->name == Entity::bullet && bulletCount > 0) {
					bulletCount--;
				}
				if (itr->value->name == Entity::asteroid) {
					if (itr->value->animation.sprite.getScale().x == 4) {
						largeCount--;
					}
					if (itr->value->animation.sprite.getScale().x == 3) {
						mediumCount--;
					}
					if (itr->value->animation.sprite.getScale().x == 2) {
						smallCount--;
					}
				}
				itr = entities.removeEntity(itr);
				continue;
			}
			itr = itr->next;
		}

		// handle collisions
		itr = entities.begin();
		int colSize;
		while (itr != entities.end()) {			
			// only check cases where asteroids are involved, because there are no other cases
			if (itr->value->name == Entity::asteroid) {
				// faster to get collidables only when 
				colSize = 0;
				Node** collidables = SH.getCollidables(itr, colSize);

				int i = 0;
				while (i < colSize) {
					if (collidables[i]->value->life == true) {
						if (hasCollided(*(itr->value), *(collidables[i]->value))) {
							
							// player hits asteroid 
							if (collidables[i]->value->name == Entity::player) {
								// spawn an explosion
								createExplosion(eExplosions, entities, collidables[i]->value, aExplosion, currentExplosion);
								drawGameOver = true;
								collidables[i]->value->life = false;
								break;
							}

							// bullet hits asteroid
							if (collidables[i]->value->name == Entity::bullet) {
								// increment score
								score += 1000;

								// spawn an explosion
								createExplosion(eExplosions, entities, collidables[i]->value, aExplosion, currentExplosion);

								// first check if it's a large asteroid
								if (itr->value->animation.sprite.getScale().x == 4) {
									bulletCollision(eMediumAsteroids, entities, itr->value, aAsteroid, currentMedium, mediumCount);
								}
								// then check if it's a medium asteroid
								if (itr->value->animation.sprite.getScale().x == 3) {
									bulletCollision(eSmallAsteroids, entities, itr->value, aAsteroid, currentSmall, smallCount);
								}

								collidables[i]->value->life = false;
								itr->value->life = false;
								break;
							}

							// asteroid hits asteroid
							if (collidables[i]->value->name == Entity::asteroid) {
								asteroidBounce(itr->value, collidables[i]->value);
								break;
							}

						}
					}
					// make sure it doesn't waste time checking collidables for dead objects
					if (itr->value->life == false) break;
					i++;
				}
			}
			itr = itr->next;
		}

		if (score > 0 && !drawGameOver && !drawWin) {
			score -= 1;
		}

		scoreText.setString(scoreString + std::to_string(score));
		if (drawGameOver) {
			app.draw(gameOverText);
			app.draw(restartText);
		}
		if (drawWin) {
			app.draw(youWinText);
			app.draw(restartText);
		}
		app.draw(scoreText);
		app.display();
	}

	return 0;
}