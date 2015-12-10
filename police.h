#ifndef POLICE_H
#define POLICE_H

#include "entity.h"

typedef struct police_t {
	entity_t super;
	int level;
	// police-specific members here
	// ammunition?
} police_t;

entity_t* policeCreate(int x, int y);
int policeAct(police_t* police);
int policeDie(police_t* police);

#endif
