#include <stdlib.h>
#include <time.h>
#include <stdlib.h>

#include "civilian.h"
#include "constants.h"
#include "entity.h"
#include "cell.h"

extern cell_t lattice[][LATTICE_WIDTH];

civilian_t* civ_init(int x, int y) {
	civilian_t* civilian;
	civilian = (civilian_t*)malloc(sizeof(civilian_t));
	civilian->xpos = x;
	civilian->ypos = y;
	return civilian;
}

int civ_act(civilian_t* civilian) {
	entity_t* self;
	int direction;
	int xoffset;
	int yoffset;
	direction = rand() % 4;
	if (direction == 0) {
		xoffset = 1;
		yoffset = 1;
	}
	else if (direction == 1) {
		xoffset = -1;
		yoffset = -1;
	}
	else if (direction == 2) {
		xoffset = 1;
		yoffset = -1;
	}
	else {
		xoffset = -1;
		yoffset = 1;
	}
	if (civilian->xpos+xoffset < 0 || civilian->xpos+xoffset >= LATTICE_WIDTH) {
		xoffset = 0;
	}
	if (civilian->ypos+yoffset < 0 || civilian->ypos+yoffset >= LATTICE_HEIGHT) {
		yoffset = 0;
	}
	if (lattice[civilian->xpos+xoffset][civilian->ypos+yoffset].occupant != NULL ||
		lattice[civilian->xpos+xoffset][civilian->ypos+yoffset].type != open) {
		xoffset = 0;
		yoffset = 0;
	}
	self = lattice[civilian->xpos][civilian->ypos].occupant;
	lattice[civilian->xpos][civilian->ypos].occupant = NULL;
	civilian->ypos = civilian->ypos+yoffset;
	civilian->xpos = civilian->xpos+xoffset;
	lattice[civilian->xpos][civilian->ypos].occupant = self;
	return 0;
}

int civ_die(civilian_t* civilian) {
	/*entity_t* self;
	self = lattice[civilian->ypos][civilian->xpos].occupant;
	lattice[civilian->ypos][civilian->xpos].occupant = NULL;*/
	free(civilian);
	return 0;
}

