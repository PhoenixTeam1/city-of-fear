#include <stdlib.h>
#include <time.h>

#include "civilian.h"
#include "entity.h"

extern int civilian_count;
extern int dead_count;

entity_t* civilianCreate(int x, int y) {
	civilian_t* civilian;
	civilian = (civilian_t*)malloc(sizeof(civilian_t));
	initEntity(&civilian->super, x ,y);
	civilian->super.type = type_civilian;
	civilian->super.act = (int (*)(entity_t*))civilianAct;
	civilian->super.die = (int (*)(entity_t*))civilianDie;
	civilian->super.direction = rand() % MAX_DIRECTIONS;
	civilian->fear = 0.0;
	civilian_count++;
	return &civilian->super;
}

int civilianAct(civilian_t* civilian) {
	entity_t* police;
	double coin;
	coin = (double) rand() / RAND_MAX;
	if (lookAround(civilian->super, type_zombie, 8, NULL)) {
		civilian->fear += 0.1;
		if (civilian->fear > 0.8) {
			civilian->fear = 0.8;
		}
	}
	else {
		civilian->fear -= 0.01;
		if (civilian->fear < 0) civilian->fear = 0;
	}
	if (lookAhead(civilian->super, civilian->super.direction, type_zombie, 1, 5, NULL)) {
		civilian->super.direction = opposite(civilian->super.direction);
		move(&civilian->super, civilian->super.direction);
	}
	if (lookAround(civilian->super, type_police, 5, &police)) {
		if (coin <= civilian->fear) {
			civilian->super.direction = getDirection(civilian->super, *police);
		}
	}
	if (move(&civilian->super, civilian->super.direction) == 0) {
		if (coin <= 0.5) {
			civilian->super.direction = rand() % MAX_DIRECTIONS;
		}
	}
	return 0;
}

int civilianDie(civilian_t* civilian) {
	civilian_count--;
	dead_count++;
	free(civilian);
	return 0;
}

