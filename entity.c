#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "entity.h"
#include "entity_list.h"
#include "constants.h"
#include "cell.h"

extern int lattice_height;
extern int lattice_width;
extern cell_t lattice[][LATTICE_WIDTH];
extern list_t* entity_list;

pair_t getOffset(direction_t direction);

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

direction_t opposite(direction_t direction) {
	switch (direction) {
		case north:
			return south;
		case northeast:
			return southwest;
		case east:
			return west;
		case southeast:
			return northwest;
		case south:
			return north;
		case southwest:
			return northeast;
		case west:
			return east;
		case northwest:
			return southeast;
		default:
			return north;
	}
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

direction_t getDirection(entity_t from, entity_t to) {
	int x;
	int y;
	x = to.xpos - from.xpos;
	y = to.ypos - from.ypos;
	if (x > 0) {
		if (y > 0) return northeast;
		else if (y < 0) return northwest;
		else return north;
	}
	else if (x < 0) {
		if (y > 0) return southeast;
		else if (y < 0) return southwest;
		else return south;
	}
	else {
		if (y > 0) return east;
		else if (y < 0) return west;
		else return north;
	}
}

int getDistance(entity_t from, entity_t to) {
	double dx;
	double dy;
	double dist;
	dx = to.xpos - from.xpos;
	dy = to.ypos - from.ypos;
	dist = dx*dx + dy*dy; // XXX if need exact distance sqrt this
	return dist;
}

int lineOfSight(entity_t from, entity_t to) {
	entity_t curpos;
	pair_t direction;
	curpos.xpos = from.xpos;
	curpos.ypos = from.ypos;
	while (curpos.xpos != to.xpos && curpos.ypos != to.ypos) {
		direction = getOffset(getDirection(curpos, to));
		curpos.xpos += direction.x;
		curpos.ypos += direction.y;
		if (lattice[curpos.xpos][curpos.ypos].type == type_barrier) {
			if (lattice[curpos.xpos][curpos.ypos].occupant == NULL) {
				return 0;
			}
		}
	}
	return 1;
}

int lookAround(entity_t entity, entity_type_t lookFor, int radius, entity_t** ret) {
	pair_t start;
	pair_t end;
	int i;
	int j;
	int found;
	double dist;
	double best_dist;
	found = 0;
	best_dist = INFINITY;
	start.x = entity.xpos - radius;
	start.y = entity.ypos -  radius;
	end.x = entity.xpos + radius;
	end.y = entity.ypos + radius;
	
	for (i = start.x; i <= end.x; i++) {
		for (j = start.y; j <= end.y; j++) {
			if (!isValidLatticeCell(i, j)) continue;
			if (lattice[i][j].occupant != NULL) {
				if (lattice[i][j].occupant->type == lookFor) {
					if (!lineOfSight(entity, *lattice[i][j].occupant)) continue;
					dist = getDistance(entity, *lattice[i][j].occupant);
					if (ret != NULL && dist < best_dist) {
						found++;
						best_dist = dist;
						*ret = lattice[i][j].occupant;
					}
					else {
						return 1;
					}
				}
			}
		}
	}
	return found;
}

int lookAhead(entity_t entity, direction_t direction, entity_type_t lookFor, int fov, int range, entity_t** ret) {
	int i;
	int j;
	int cur_row;
	int cur_col;
	pair_t offset;
	offset = getOffset(direction);
	for (i = 1; i < range + 1; i++) {
		for (j = -fov; j <= fov; j++) {
			cur_row = entity.xpos + offset.x * i + (j * offset.y * (abs(offset.y) - abs(offset.x)));
			cur_col = entity.ypos + offset.y * i + (j * offset.x);
			if (!isValidLatticeCell(cur_row, cur_col)) continue;
			if (lattice[cur_row][cur_col].occupant != NULL) {
				if (lattice[cur_row][cur_col].occupant->type == lookFor) {
					if(ret != NULL) {
						*ret = lattice[cur_row][cur_col].occupant;
					}
					return 1;
				}
			}
		}
	}
	return 0;
}
