#include <stdlib.h>
#include "entity.h"
#include "zombie.h"

#define FEAST_TIME 1
#define WAIT_TIME 5

extern int zombie_count;

entity_t* zombieCreate(int x, int y) {
	zombie_t* zombie;
	zombie = (zombie_t*)malloc(sizeof(zombie_t));
	initEntity(&zombie->super, x ,y);
	zombie->super.type = type_zombie;
	zombie->super.act = (int (*)(entity_t*))zombieAct;
	zombie->super.die = (int (*)(entity_t*))zombieDie;
	zombie->super.direction = rand() % MAX_DIRECTIONS;
	zombie->feasting = 0;
	zombie->hibernating = 0;
	zombie->energy = 100;
	zombie_count++;
	return &zombie->super;
}

int zombieAct(zombie_t* zombie) {
	entity_t* neighbor;
	entity_t* new_zombie;
	entity_t* noise;
	double coin;

	if (zombie->feasting) {
		zombie->feasting--; return 0;
	}
	if (!zombie->energy--) {
		killEntity(&zombie->super);
		return 0;
	}
	if (lookAhead(zombie->super, zombie->super.direction, type_civilian, 0, 20, NULL)) {
		move(&zombie->super, zombie->super.direction);
		move(&zombie->super, zombie->super.direction);
		move(&zombie->super, zombie->super.direction);
	}
	else if(lookAhead(zombie->super, zombie->super.direction, type_police, 0, 20, NULL)) {
		move(&zombie->super, zombie->super.direction);
		move(&zombie->super, zombie->super.direction);
		move(&zombie->super, zombie->super.direction);
	}
	else {
		coin = (double)rand() / (double)RAND_MAX;
		if (coin < 0.25) {
			zombie->super.direction = rand() % MAX_DIRECTIONS;
		}
	}
	neighbor = getNeighbor(&zombie->super, zombie->super.direction);
	if (neighbor != NULL && (neighbor->type == type_civilian || neighbor->type == type_police)) {
		zombie->feasting = FEAST_TIME;
		zombie->energy = 100;
		new_zombie = zombieCreate(neighbor->xpos, neighbor->ypos);
		killEntity(neighbor);
		spawnEntity(new_zombie);
		((zombie_t *)new_zombie)->feasting = FEAST_TIME;
	}
	if (lookAround(zombie->super, type_police, 15, &noise)) {
		zombie->super.direction = getDirection(zombie->super, *noise);
		move(&zombie->super, zombie->super.direction);
	}
	else if (lookAround(zombie->super, type_civilian, 15, &noise)) {
		zombie->super.direction = getDirection(zombie->super, *noise);
		move(&zombie->super, zombie->super.direction);
	}
	else {
		if (zombie->hibernating == 0) {
			move(&zombie->super, zombie->super.direction);
			zombie->hibernating = WAIT_TIME;
		}
		zombie->hibernating--;
	}

	return 0;
}

int zombieDie(zombie_t* zombie) {
	free(zombie);
	zombie_count--;
	return 0;
}

