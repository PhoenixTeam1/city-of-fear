#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "constants.h"
#include "civilian.h"
#include "entity.h"
#include "entity_list.h"
#include "cell.h"
#include "block_generator.h"
#include "populator.h"
#include "visualization.h"

// Globals
int running;
int lattice_height = LATTICE_HEIGHT;
int lattice_width = LATTICE_WIDTH;
cell_t lattice[LATTICE_HEIGHT][LATTICE_WIDTH];
list_t* entity_list;
int civilian_max;
int civilian_count;
int police_max;
int police_count;
int zombie_max;
int zombie_count;
int dead_count;


// Simulation functions
void initializeLattice(void);
void dumbInteract(void);

int main() {
	int timestep;
	pthread_t visualizerThread;
	running = 1;
	timestep = 0;

	srand(time(NULL));
	initializeLattice();
	pthread_create(&visualizerThread, NULL, visualizer, NULL);

	saveLatticeSnapshot("demo", timestep);
	// Run simulation
	while (running) {
		timestep++;
		usleep(SLEEP_TIME);
		dumbInteract();
		//saveLatticeSnapshot("demo", timestep);
	}

	unpopulateCity();
	pthread_join(visualizerThread, NULL);
	return 0;
}

void dumbInteract(void) {
	list_act(entity_list);
	return;
}


void initializeLattice(void) {
	int i;
	int j;
	for (i = 0; i < lattice_height; i++) {
		for (j = 0; j < lattice_width; j++) {
			lattice[i][j].type = type_open;
		}
	}

	makeSimpleClosedBuildings();
	populateCity();

	return;
}


