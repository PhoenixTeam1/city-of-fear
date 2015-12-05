#ifndef CIVILIAN_H
#define CIVILIAN_H

#include "entity.h"

typedef struct civilian_t {
	int xpos;
	int ypos;
} civilian_t;

civilian_t* civ_init(int x, int y);
int civ_act(civilian_t* civilian);
int civ_die(civilian_t* civilian);

#endif
