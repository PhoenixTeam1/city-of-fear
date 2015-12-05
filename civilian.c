#include <stdlib.h>
#include <time.h>
#include <stdlib.h>

#include "civilian.h"
#include "entity.h"

entity_t* civilianCreate(int x, int y) {
	civilian_t* civilian;
	civilian = (civilian_t*)malloc(sizeof(civilian_t));
	initEntity(&civilian->super, x ,y);
	civilian->super.type = type_civilian;
	civilian->super.act = (int (*)(entity_t*))civilianAct;
	civilian->super.die = (int (*)(entity_t*))civilianDie;
	civilian->direction = rand() % MAX_DIRECTIONS;
	return &civilian->super;
}

int civilianAct(civilian_t* civilian) {
	if (move(&civilian->super, civilian->direction) == 0) {
		civilian->direction = rand() % MAX_DIRECTIONS;
	}
	return 0;
}

int civilianDie(civilian_t* civilian) {
	free(civilian);
	return 0;
}

