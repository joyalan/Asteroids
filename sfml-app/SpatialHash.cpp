#include "SpatialHash.h"
#include <iostream>

Node* buckets[300][100];
int bucketSizes[300];

void SpatialHash::Setup(int newCellSize)
{
	cellsize = newCellSize;
	cols = W / newCellSize;
	rows = H / newCellSize;
}
// empties all buckets
void SpatialHash::clearBuckets(){
	for (int i = 0; i < cols*rows; i++) {
		for (int j = 0; j < bucketSizes[i]; j++) {
			buckets[i][j] = nullptr;	
		}
		bucketSizes[i] = 0;
	}
}
// places the entity into the buckets it's supposed to go in
void SpatialHash::registerObject(Node * thisEntity)
{
	getIDforEnt(thisEntity);
	if (thisEntity->NWbucket > -1) {
		buckets[thisEntity->NWbucket][bucketSizes[thisEntity->NWbucket]] = thisEntity;
		bucketSizes[thisEntity->NWbucket]++;
	}
	if (thisEntity->NEbucket > -1) {
		buckets[thisEntity->NEbucket][bucketSizes[thisEntity->NEbucket]] = thisEntity;
		bucketSizes[thisEntity->NEbucket]++;
	}
	if (thisEntity->SWbucket > -1) {
		buckets[thisEntity->SWbucket][bucketSizes[thisEntity->SWbucket]] = thisEntity;
		bucketSizes[thisEntity->SWbucket]++;
	}
	if (thisEntity->SEbucket > -1) {
		buckets[thisEntity->SEbucket][bucketSizes[thisEntity->SEbucket]] = thisEntity;
		bucketSizes[thisEntity->SEbucket]++;
	}
}

// checks a bucket for duplicate entries
bool SpatialHash::dupCheck(Node* thisEntity, int newLoc) {
	for (int i = 0; i < bucketSizes[newLoc]; i++) {
		if (buckets[newLoc][i]->value == thisEntity->value) {
			return false;
		}
	}
	return true;
}

// gets buckets that this entity belongs in
void SpatialHash::getIDforEnt(Node * thisEntity)
{
	// have to multiply all these numbers by the scale of the sprite
	int topLeftX = (int)(thisEntity->value->x - thisEntity->value->radius * thisEntity->value->animation.sprite.getScale().x);
	if (topLeftX < 0) {
		topLeftX = W - topLeftX;
	}
	int topLeftY = (int)(thisEntity->value->y - thisEntity->value->radius * thisEntity->value->animation.sprite.getScale().y);
	if (topLeftY < 0) {
		topLeftY = H - topLeftY;
	}
	int bottomRightX = (int)(thisEntity->value->x + thisEntity->value->radius * thisEntity->value->animation.sprite.getScale().x);
	if (bottomRightX > W) {
		bottomRightX = bottomRightX - W;
	}
	int bottomRightY = (int)(thisEntity->value->y + thisEntity->value->radius * thisEntity->value->animation.sprite.getScale().y);
	if (bottomRightY > H) {
		bottomRightY = bottomRightY - H;
	}

	int hashNW = hashLoc(topLeftX, topLeftY);
	int hashNE = hashLoc(bottomRightX, topLeftY);
	int hashSE = hashLoc(topLeftX, bottomRightY);
	int hashSW = hashLoc(bottomRightX, bottomRightY);
	

	// check NW bucket for duplicates
	if (dupCheck(thisEntity, hashNW)) 
		thisEntity->NWbucket = hashNW;
	else 
		thisEntity->NWbucket = -1;

	// check NE bucket for duplicates
	if (hashNE == hashNW)			// first check if it's the same as the other value	
		thisEntity->NEbucket = -1;
	else if (dupCheck(thisEntity, hashNE)) 
		thisEntity->NEbucket = hashNE;
	else
		thisEntity->NEbucket = -1;

	// check SW bucket for duplicates
	if (hashSW == hashNW || hashSW == hashNE) // first check if it's the same as the other values	
		thisEntity->SWbucket = -1;	
	else if (dupCheck(thisEntity, hashSW)) 
		thisEntity->SWbucket = hashSW;
	else
		thisEntity->SWbucket = -1;

	// check SE bucket for duplicates
	if (hashSE == hashNW || hashSE == hashNE || hashSE == hashSW) // first check if it's the same as the other values	
		thisEntity->SEbucket = -1;	
	else if (dupCheck(thisEntity, hashSE)) 
		thisEntity->SEbucket = hashSE;
	else
		thisEntity->SEbucket = -1;

	// final check just to make sure that every entity ends up with a bucket it belongs to. 
	// otherwise it's possible that collisions could be skipped entirely
	if (thisEntity->NWbucket == -1 && thisEntity->NEbucket == -1 && thisEntity->SWbucket == -1 && thisEntity->SEbucket == -1)
		thisEntity->NWbucket = hashNW;
	/*
	std::cout << thisEntity->value->name << std::endl;
	std::cout << "NW: " << thisEntity->NWbucket;
	std::cout << "\tNE: " << thisEntity->NEbucket << std::endl;
	std::cout << "SW: " << thisEntity->SWbucket;
	std::cout << "\tSE: " << thisEntity->SEbucket << std::endl;
	*/
}

int SpatialHash::hashLoc(int x, int y)
{
	int width = int(floor(W / cellsize));
	double xCalc = floor(x / cellsize);
	double yCalc = floor(y / cellsize);
	return (int)(xCalc + yCalc * width);
}

// this function gathers all the possible entities from the buckets that this entity is in and stores it in collidables
// bucket 0 = NW, 1 = NE, 2 = SW, 3 = SE
void SpatialHash::checkBucket(Node** theseCollidables, int &colSize, Node* thisEntity, int thisBucket) {
	// players and bullets don't need to calculate collisions for bullets, so save some calc time
	bool ignoreBullets = false;
	if (thisEntity->value->name == Entity::bullet || thisEntity->value->name == Entity::player)
		ignoreBullets = true;
	// determine bucket based on thisBucket input
	int bucket = -1;
	switch (thisBucket) {
	case 0:
		bucket = thisEntity->NWbucket;
		break;
	case 1:
		bucket = thisEntity->NEbucket;
		break;
	case 2:
		bucket = thisEntity->SWbucket;
		break;
	case 3:
		bucket = thisEntity->SEbucket;
		break;
	}
	// checks if a bucket is valid (not a duplicate of another bucket) first
	if (bucket > -1) {
		for (int j = 0; j < bucketSizes[bucket]; j++) {
			// check that they're not the same exact entity and that the opposite thing is actually alive
			if (thisEntity->value != buckets[bucket][j]->value && buckets[bucket][j]->value->life) {
				// check that you're not adding bullets when you shouldn't and not testing explosions at all
				if (!(ignoreBullets && buckets[bucket][j]->value->name == Entity::bullet) && !(buckets[bucket][j]->value->name == Entity::explosion)) {
					theseCollidables[colSize] = buckets[bucket][j];
					colSize++;
				}
			}
		}
	}
	return;
}

//  get the list of other objects that are in the buckets that his entity is in
Node ** SpatialHash::getCollidables(Node * thisEntity, int &colSize) {
	colSize = 0;
	static Node* collidables[200];
	// explosions don't collide with anything
	if (thisEntity->value->name == Entity::explosion) {
		colSize = 0;
		return collidables;
	}
	for (int i = 0; i < 4; i++)
		checkBucket(collidables, colSize, thisEntity, i);
	return collidables;
}