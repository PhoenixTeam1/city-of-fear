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
	civilian_count++;
	return &civilian->super;
}

int civilianAct(civilian_t* civilian) {
	entity_t* police;
	if (lookAhead(civilian->super, civilian->super.direction, type_zombie, 1, 5, NULL)) {
		civilian->super.direction = opposite(civilian->super.direction);
		move(&civilian->super, civilian->super.direction);
	}
	if (lookAround(civilian->super, type_police, 15, &police)) {
		civilian->super.direction = getDirection(civilian->super, *police);
		move(&civilian->super, civilian->super.direction);
	}
	else if (move(&civilian->super, civilian->super.direction) == 0) {
		civilian->super.direction = rand() % MAX_DIRECTIONS;
	}
	return 0;
}

int civilianDie(civilian_t* civilian) {
	civilian_count--;
	dead_count++;
	free(civilian);
	return 0;
}

