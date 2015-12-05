#include <stdlib.h>
#include "entity.h"
#include "constants.h"
#include "cell.h"

extern cell_t lattice[][LATTICE_WIDTH];

void initEntity(entity_t* entity, int x, int y) {
	entity->xpos = x;
	entity->ypos = y;
	entity->next = NULL;
	return;
}

int move(entity_t* entity, direction_t direction) {
	int xoffset;
	int yoffset;
	switch (direction) {
		case north:
			xoffset = 1;
			yoffset = 0;
			break;
		case northeast:
			xoffset = 1;
			yoffset = 1;
			break;
		case east:
			xoffset = 0;
			yoffset = 1;
			break;
		case southeast:
			xoffset = -1;
			yoffset = 1;
			break;
		case south:
			xoffset = -1;
			yoffset = 0;
			break;
		case southwest:
			xoffset = -1;
			yoffset = -1;
			break;
		case west:
			xoffset = 0;
			yoffset = -1;
			break;
		case northwest:
			xoffset = 1;
			yoffset = -1;
			break;
		default:
			xoffset = 0;
			yoffset = 0;
	}

	// Validity checks
	if (entity->xpos+xoffset < 0 || entity->xpos+xoffset >= LATTICE_HEIGHT) {
		xoffset = 0;
		return 0;
	}
	if (entity->ypos+yoffset < 0 || entity->ypos+yoffset >= LATTICE_WIDTH) {
		yoffset = 0;
		return 0;
	}

	// Only allow movement to open areas
	if (lattice[entity->xpos+xoffset][entity->ypos+yoffset].occupant != NULL ||
		lattice[entity->xpos+xoffset][entity->ypos+yoffset].type != type_open) {
		xoffset = 0;
		yoffset = 0;
		return 0;
	}

	// Perform move
	lattice[entity->xpos][entity->ypos].occupant = NULL;
	entity->xpos = entity->xpos+xoffset;
	entity->ypos = entity->ypos+yoffset;
	lattice[entity->xpos][entity->ypos].occupant = entity;
	return 1;
}
