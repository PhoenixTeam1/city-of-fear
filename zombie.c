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
	return &zombie->super;
}

int zombieAct(zombie_t* zombie) {
	int i;
	entity_t* neighbor;
	for (i = 0; i < MAX_DIRECTIONS; i++) {
		neighbor = getNeighbor(&zombie->super, i);
		if (neighbor == NULL) {
			continue;
		}
		if (neighbor->type == type_civilian) {
			// XXX quick hack to get things working
			// need to redo the entity list to do this appropriately
			neighbor->act = (int (*)(entity_t*))zombieAct;
			neighbor->type = type_zombie;
		}
	}
	move(&zombie->super, rand() % MAX_DIRECTIONS);
	return 0;
}

int zombieDie(zombie_t* zombie) {
	free(zombie);
	return 0;
}
