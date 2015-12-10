#include <time.h>
#include <stdlib.h>
#include <assert.h>

#include "constants.h"
#include "entity.h"
#include "entity_list.h"
#include "cell.h"
#include "populator.h"
#include "civilian.h"
#include "police.h"
#include "zombie.h"

#define OPEN_PROBABILITY	0.9573f
#define CIVILIAN_PROBABILITY	0.0400f
#define POLICE_PROBABILITY	0.001f
#define ZOMBIE_PROBABILITY	0.0001f

extern int lattice_height;
extern int lattice_width;
extern cell_t lattice[][LATTICE_WIDTH];
extern list_t* entity_list;

/* Creates entities and populates lattice with them.  Assumes lattice occupants
 * are all null */
void populateCity(void) {
	int i;
	int j;
	double coin;
	entity_t* new_entity;
	new_entity = NULL;
	assert(entity_list == NULL); // list should be empty before population
	entity_list = list_create();
	for (i = 0; i < lattice_height; i++) {
		for (j = 0; j < lattice_width; j++) {
			if (lattice[i][j].type != type_open) {
				continue;
			}
			coin = (double)rand()/(double)RAND_MAX;
			if (coin <= ZOMBIE_PROBABILITY) {
				new_entity = zombieCreate(i, j);
			}
			else if (coin <= POLICE_PROBABILITY) {
				new_entity = policeCreate(i, j);
			}
			else if (coin <= CIVILIAN_PROBABILITY) {
				new_entity = civilianCreate(i, j);
			}

			// create linked list of entities
			if (new_entity) {
				list_add(entity_list, new_entity);
				lattice[i][j].occupant = new_entity;
				new_entity = NULL;
			}
		}
	}
	return;
}

void unpopulateCity(void) {
	/* XXX call die on all entities and list_free*/
	return;
}

