#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "constants.h"
#include "civilian.h"
#include "entity.h"
#include "block_generator.h"
#include "populator.h"
#include "visualization.h"

// Globals
int running;
int lattice_height = LATTICE_HEIGHT;
int lattice_width = LATTICE_WIDTH;
entity_t lattice[LATTICE_HEIGHT][LATTICE_WIDTH];

// Simulation functions
void initializeLattice(void);
void dumbInteract(void);

int main() {
	int timestep;
	pthread_t visualizerThread;
	running = 1;
	timestep = 0;

	initializeLattice();
	pthread_create(&visualizerThread, NULL, visualizer, NULL);

	saveLatticeSnapshot("demo", timestep);

	while (running) {
		timestep++;
		usleep(SLEEP_TIME);
		dumbInteract();
		//saveLatticeSnapshot("demo", timestep);
		lattice[0][0].type = zombie;;
	}

	pthread_join(visualizerThread, NULL);
	return 0;
}

void dumbInteract(void) {
	int i;
	int j;
	for (i = 0; i < lattice_height; i++) {
		for (j = 0; j < lattice_width; j++) {
			//lattice[i][j].act(&lattice[i][j]);
			//lattice[i][j].react(&lattice[i][j]);
		}
	}
	return;
}


void initializeLattice(void) {
	int i;
	int j;
	for (i = 0; i < lattice_height; i++) {
		for (j = 0; j < lattice_width; j++) {
			lattice[i][j].type = open;
		}
	}

	makeSimpleClosedBuildings();
	populateCity();

	return;
}


