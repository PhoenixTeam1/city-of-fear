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
#include "simulate.h"

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

void dropBomb(int x, int y) {
	int i;
	int j;
	int radius = 15;
	float d;
	for (i = -radius; i <= radius; i++) {
		for (j = -radius; j <= radius; j++) {
			d = ((float)j/(float)radius)*((float)j/(float)radius) +
			    ((float)i/(float)radius)*((float)i/(float)radius);
			if ((x+i) > lattice_height-1 || (x+i) < 0) continue;
			if ((y+j) > lattice_width-1 || (y+j) < 0) continue;
			if (d < 1.1) {
				if (lattice[x+i][y+j].type == type_barrier) {
					lattice[x+i][y+j].type = type_open;
				}
				if (lattice[x+i][y+j].occupant != NULL) {
					killEntity(lattice[x+i][y+j].occupant);
					//lattice[x+i][y+j].type = type_barrier;
				}
			}
		}
	}
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


