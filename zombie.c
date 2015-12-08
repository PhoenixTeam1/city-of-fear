#include <stdlib.h>
#include "entity.h"
#include "zombie.h"

entity_t* zombieCreate(int x, int y) {
	zombie_t* zombie;
	zombie = (zombie_t*)malloc(sizeof(zombie_t));
	initEntity(&zombie->super, x ,y);
	zombie->super.type = type_zombie;
	zombie->super.act = (int (*)(entity_t*))zombieAct;
	zombie->super.die = (int (*)(entity_t*))zombieDie;
	zombie->direction = rand() % MAX_DIRECTIONS;
	return &zombie->super;
}

int zombieAct(zombie_t* zombie) {
	entity_t* neighbor;
	entity_t* new_zombie;

	if (lookAhead(zombie->super, zombie->direction, type_civilian, 0, 10)) {
		move(&zombie->super, zombie->direction);
		move(&zombie->super, zombie->direction);
		move(&zombie->super, zombie->direction);
	}
	else {
		zombie->direction = rand() % MAX_DIRECTIONS;
	}
	neighbor = getNeighbor(&zombie->super, zombie->direction);
	if (neighbor != NULL && neighbor->type == type_civilian) {
		new_zombie = zombieCreate(neighbor->xpos, neighbor->ypos);
		killEntity(neighbor);
		spawnEntity(new_zombie);
	}

	move(&zombie->super, rand() % MAX_DIRECTIONS);
	return 0;
}

int zombieDie(zombie_t* zombie) {
	free(zombie);
	return 0;
}

