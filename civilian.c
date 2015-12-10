#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "civilian.h"
#include "entity.h"

#define ZOMBIE_FEAR 0.025
#define POLIEC_FEAR -0.005
#define BIRTH_RATE .001

extern int civilian_count;
extern int civilian_max;

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
	entity_t* police = NULL;
	entity_t* zombie = NULL;
	entity_t* child;
	double coin;
	int num_zombies;
	int i;

	coin = (double) rand() / RAND_MAX;

	num_zombies = lookAround(civilian->super, type_zombie, 8, &zombie);
	// Zombies cause fear
	if (num_zombies) {
		civilian->fear += ZOMBIE_FEAR * (double) num_zombies;
	}
	else {	// Birth
		if (lookAround(civilian->super, type_civilian, 1, NULL)) {
			for (i = 0; i < rand() % MAX_DIRECTIONS; i++) {
				child = getNeighbor(&civilian->super, i);
				if (child == NULL && coin < BIRTH_RATE) {
					if (isValidLatticeCell(civilian->super.xpos + getOffset(i).x, civilian->super.ypos + getOffset(i).y)) {
						child = civilianCreate(civilian->super.xpos + getOffset(i).x, civilian->super.ypos + getOffset(i).y);
						civilian_max++;
						spawnEntity(child);
					}
				}
			}
		}
	}
	coin = (double) rand() / RAND_MAX;
	// Police calm us
	if (lookAround(civilian->super, type_police, 8, &police)) { 
		civilian->fear -= 0.005;
		if (civilian->fear < 0) civilian->fear = 0;
	}
	// Move
	
	if (coin <= civilian->fear && police != NULL) {
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

