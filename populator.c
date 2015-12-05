#include <time.h>
#include <stdlib.h>

#include "constants.h"
#include "entity.h"
#include "cell.h"
#include "populator.h"
#include "civilian.h"

#define OPEN_PROBABILITY	0.800f
#define CIVILIAN_PROBABILITY	0.199f
#define POLICE_PROBABILITY	0.001f

extern int lattice_height;
extern int lattice_width;
extern cell_t lattice[][LATTICE_WIDTH];
extern entity_t* entity_head;

/* Creates entities and populates lattice with them.  Assumes lattice occupants
 * are all null */
void populateCity(void) {
	int i;
	int j;
	double coin;
	entity_t* prev_entity;
	entity_t* new_entity;
	prev_entity = NULL;
	new_entity = NULL;
	for (i = 0; i < lattice_height; i++) {
		for (j = 0; j < lattice_width; j++) {
			if (lattice[i][j].type != open) {
				continue;
			}
			coin = (double)rand()/(double)RAND_MAX;
			if (coin <= POLICE_PROBABILITY) {
				new_entity = create_entity(police);
			}
			else if (coin <= CIVILIAN_PROBABILITY) {
				new_entity = create_entity(civilian);
			}
			if (new_entity) {
				if (prev_entity) {
					prev_entity->next = new_entity;
				}
				else {
					entity_head = new_entity;
				}
				lattice[i][j].occupant = new_entity;
				lattice[i][j].occupant->data = new_entity->init(i,j);
				prev_entity = new_entity;
				new_entity = NULL;
			}
		}
	}
	return;
}

void unpopulateCity(void) {
	/* XXX call die on all entities */
	return;
}

