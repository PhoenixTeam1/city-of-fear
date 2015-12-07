#include <stdlib.h>
#include <assert.h>
#include "entity.h"
#include "entity_list.h"
#include "constants.h"
#include "cell.h"

typedef struct pair_t {
	int x;
	int y;
} pair_t;

extern cell_t lattice[][LATTICE_WIDTH];
extern list_t* entity_list;

pair_t getOffset(direction_t direction);
int isValidLatticeCell(int x, int y);

void initEntity(entity_t* entity, int x, int y) {
	entity->xpos = x;
	entity->ypos = y;
	entity->listnode = NULL;
	return;
}

int move(entity_t* entity, direction_t direction) {
	pair_t offset;
	offset = getOffset(direction);


	// Validity checks
	if (isValidLatticeCell(entity->xpos+offset.x, entity->ypos+offset.y) == 0) {
		return 0;
	}

	// Only allow movement to open areas
	if (lattice[entity->xpos+offset.x][entity->ypos+offset.y].occupant != NULL ||
		lattice[entity->xpos+offset.x][entity->ypos+offset.y].type != type_open) {
		offset.x = 0;
		offset.y = 0;
		return 0;
	}

	// Perform move
	lattice[entity->xpos][entity->ypos].occupant = NULL;
	entity->xpos = entity->xpos+offset.x;
	entity->ypos = entity->ypos+offset.y;
	lattice[entity->xpos][entity->ypos].occupant = entity;
	return 1;
}

entity_t* getNeighbor(entity_t* entity, direction_t direction) {
	pair_t offset;
	entity_t* neighbor;
	offset = getOffset(direction);
	if (isValidLatticeCell(entity->xpos+offset.x, entity->ypos+offset.y) == 0) {
		return NULL;
	}
	neighbor = lattice[entity->xpos+offset.x][entity->ypos+offset.y].occupant;
	return neighbor;
}

void killEntity(entity_t* entity) {
	list_remove(entity_list, entity->listnode);
	entity->die(entity);
	lattice[entity->xpos][entity->ypos].occupant = NULL;
	return;
}
void spawnEntity(entity_t* entity) {
	// no spawning on top of others!
	assert(lattice[entity->xpos][entity->ypos].occupant == NULL);
	lattice[entity->xpos][entity->ypos].occupant = entity;
	list_add(entity_list, entity);
	return;
}

int isValidLatticeCell(int x, int y) {
	// Validity checks
	if (x < 0 || x >= LATTICE_HEIGHT) {
		return 0;
	}
	if (y < 0 || y >= LATTICE_WIDTH) {
		return 0;
	}
	return 1;
}

pair_t getOffset(direction_t direction) {
	switch (direction) {
		case north:
			return (pair_t){1,0};
		case northeast:
			return (pair_t){1,1};
		case east:
			return (pair_t){0,1};
		case southeast:
			return (pair_t){-1,1};
		case south:
			return (pair_t){-1,0};
		case southwest:
			return (pair_t){-1,-1};
		case west:
			return (pair_t){0,-1};
		case northwest:
			return (pair_t){1,-1};
		default:
			return (pair_t){0,0};
	}
	return (pair_t){0,0};
}

