#include <stdlib.h>

#include "police.h"
#include "entity.h"

entity_t* policeCreate(int x, int y) {
	police_t* police;
	police = (police_t*)malloc(sizeof(police_t));
	initEntity(&police->super, x ,y);
	police->super.type = type_police;
	police->super.act = (int (*)(entity_t*))policeAct;
	police->super.die = (int (*)(entity_t*))policeDie;
	return &police->super;
}

int policeAct(police_t* police) {
	return 0;
}

int policeDie(police_t* police) {
	free(police);
	return 0;
}

