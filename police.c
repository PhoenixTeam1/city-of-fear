#include <stdlib.h>
#include "police.h"
#include "entity.h"

#define ACCURACY 0.80

extern int police_count;
extern int dead_count;

entity_t* policeCreate(int x, int y) {
	police_t* police;
	police = (police_t*)malloc(sizeof(police_t));
	initEntity(&police->super, x ,y);
	police->super.type = type_police;
	police->super.act = (int (*)(entity_t*))policeAct;
	police->super.die = (int (*)(entity_t*))policeDie;
	police->super.direction = rand() % MAX_DIRECTIONS;
	police_count++;
	return &police->super;
}

int policeAct(police_t* police) {
	entity_t* zombie;
	entity_t* civilian;
	entity_t* neighbor;
	entity_t* armed_civ;
	int surrounded;
	int i;
	double coin;
	coin = (double)rand() / (double)RAND_MAX;
	surrounded = 1;

	if (lookAhead(police->super, police->super.direction, type_zombie, 5, 5, &zombie)) {
		if (coin <= ACCURACY) {
			killEntity(zombie);
		}
		else {
			if (lookAround(*zombie, type_civilian, 3, &civilian)) {
				killEntity(civilian);
			}
		}
	}
	else if (lookAround(police->super, type_zombie, 8, &zombie)) {
		police->super.direction = getDirection(police->super, *zombie);
	}
	else {
		if (coin <= 0.25) {
			police->super.direction = rand() % MAX_DIRECTIONS;
		}
		move(&police->super, police->super.direction);
	}
	for (i = 0; i < MAX_DIRECTIONS; i++) {
		neighbor = getNeighbor(&police->super, i);
		if (neighbor == NULL || neighbor->type != type_civilian) {
			surrounded = 0;
		}
	}
	if (surrounded) {
		neighbor = getNeighbor(&police->super, rand() % MAX_DIRECTIONS);
		armed_civ = policeCreate(neighbor->xpos, neighbor->ypos);
		killEntity(neighbor);
		spawnEntity(armed_civ);

	}
	return 0;
}

int policeDie(police_t* police) {
	free(police);
	dead_count++;
	police_count--;
	return 0;
}

