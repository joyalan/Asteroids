#pragma once
#include "Entity.h"
#include "EntityList.h"
#include <cmath>

// extern Entity entityStack[1000000];		//	extra space
extern Node* buckets[300][100];	//	making a LOT of space for pointers
extern int bucketSizes[300];		// the size of each bucket

class SpatialHash
{
public:
	SpatialHash(int newCellsize) { Setup(newCellsize); }
	void Setup(int newCellSize);
	void clearBuckets();
	void getIDforEnt(Node* thisEntity);
	void registerObject(Node * thisEntity);

	Node** getCollidables(Node* thisEntity, int &colSize);

private:
	int cellsize;
	int cols;
	int rows;

	bool dupCheck(Node* thisEntity, int newLoc);
	void checkBucket(Node** theseCollidables, int &colSize, Node* thisEntity, int thisBucket);
	int hashLoc(int x, int y);

};

