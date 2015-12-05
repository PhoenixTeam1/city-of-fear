#include <time.h>
#include <stdlib.h>

#include "constants.h"
#include "entity.h"
#include "populator.h"

#define OPEN_PROBABILITY	0.800f
#define CIVILIAN_PROBABILITY	0.199f
#define POLICE_PROBABILITY	0.001f

extern int lattice_height;
extern int lattice_width;
extern entity_t lattice[][LATTICE_WIDTH];

void populateCity(void) {
	int i;
	int j;
	double coin;
	for (i = 0; i < lattice_height; i++) {
		for (j = 0; j < lattice_width; j++) {
			if (lattice[i][j].type != open) {
				continue;
			}
			coin = (double)rand()/(double)RAND_MAX;
			if (coin <= POLICE_PROBABILITY) {
				lattice[i][j].type = police;
			}
			else if (coin <= CIVILIAN_PROBABILITY) {
				lattice[i][j].type = civilian;
			}
			else {
				lattice[i][j].type = open;
			}
		}
	}
	return;
}

