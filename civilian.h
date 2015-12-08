#ifndef CIVILIAN_H
#define CIVILIAN_H

#include "entity.h"

typedef struct civilian_t {
	entity_t super;
} civilian_t;

entity_t* civilianCreate(int x, int y);
int civilianAct(civilian_t* civilian);
int civilianDie(civilian_t* civilian);

#endif
