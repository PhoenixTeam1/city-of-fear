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
	entity_t* zombie;
	double coin;
	int num_zombies;

	coin = (double) rand() / RAND_MAX;

	num_zombies = lookAround(civilian->super, type_zombie, 8, &zombie);
	// Zombies cause fear
	if (num_zombies) {
		civilian->fear += 0.025 * (double) num_zombies;
	}
	// Police calm us
	if (lookAround(civilian->super, type_police, 8, &police)) { 
		civilian->fear -= 0.005;
		if (civilian->fear < 0) civilian->fear = 0;
	}
	// Move
	
	if (coin <= civilian->fear) {
		civilian->super.direction = getDirection(civilian->super, *police);
	}
	else if (num_zombies) {
		civilian->super.direction = opposite(getDirection(civilian->super, *zombie));
	}
	if (move(&civilian->super, civilian->super.direction) == 0) {
		if (coin <= 0.5) {
			civilian->super.direction = rand() % MAX_DIRECTIONS;
		}
	}
	if (civilian->fear > 0.8) { // PANIC!!!
		civilian->fear = 0.8;
		move(&civilian->super, civilian->super.direction);
	}
	
	return 0;
}

int civilianDie(civilian_t* civilian) {
	civilian_count--;
	dead_count++;
	free(civilian);
	return 0;
}

