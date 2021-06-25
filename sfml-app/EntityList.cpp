#include "EntityList.h"
typedef Node* iterator;
typedef unsigned int size_type;

Node entityNodes[maxEntities];
Node* entityStack[maxEntities];

EntityList::EntityList() {
	for (int i = 0; i < maxEntities; i++) {
		entityStack[i] = &entityNodes[i];
	}

	sentinel.next = sentinel.prev = &sentinel;
	stackSize = maxEntities;
	listSize = 0;
}

iterator EntityList::createEntity() {
	// grab from the top of the stack
	Node* newEntityNode = entityStack[stackSize - 1];
	stackSize -= 1;

	newEntityNode->next = sentinel.prev->next;
	newEntityNode->prev = sentinel.prev;
	sentinel.prev->next = newEntityNode;
	sentinel.prev = newEntityNode;
	listSize += 1;
	return sentinel.prev;
}

// returns the next value so that iteration does not get interrupted
iterator EntityList::removeEntity(iterator &thisEntity) {
	iterator result = thisEntity->next;
	// redirect the pointers around the node you're removing
	thisEntity->prev->next = thisEntity->next;
	thisEntity->next->prev = thisEntity->prev;
	// return the node to the stack
	entityStack[stackSize] = thisEntity;
	stackSize++;
	listSize--;
	return result;
}

void EntityList::clear() {
	iterator itr = sentinel.next;
	while (itr != &sentinel) {
		itr = removeEntity(itr);	// simply iterate through, and remove all entities. this should return their data to the stack.
	}
	sentinel.next = sentinel.prev = &sentinel;
}
