#pragma once
#include "Entity.h"
#include "Player.h"
#include "Asteroid.h"
#include "Bullet.h"

class Node {
public:
	Entity* value;
	Node *next;
	Node *prev;

	// this is for placement in the Spatial Hash
	int NWbucket = -1;
	int NEbucket = -1;
	int	SWbucket = -1;
	int SEbucket = -1;
};

const int maxEntities = 100;
extern Node entityNodes[maxEntities];
extern Node* entityStack[maxEntities];

class EntityList {
typedef Node* iterator;
typedef unsigned int size_type;
public:
	EntityList();

	inline iterator begin() { return sentinel.next; }
	inline iterator end() { return &sentinel; }
	inline size_type size() { return listSize; }

	iterator createEntity();
	iterator removeEntity(iterator &thisEntity);

	void clear();

private:
	Node sentinel;

	size_type stackSize;
	size_type listSize;
};

