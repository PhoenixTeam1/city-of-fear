#include <sys/time.h>
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
double when();

int main() {
	int timestep;
	double timer;
	double total;
	pthread_t visualizerThread;
	running = 1;
	timestep = 0;

	srand(time(NULL));
	initializeLattice();
	pthread_create(&visualizerThread, NULL, visualizer, NULL);

	saveLatticeSnapshot("demo", timestep);
	// Run simulation
	total = 0;
	while (running && ((((double)civilian_count / (double)civilian_max) > 0.01) && zombie_count > 0)) {
		timer = when();
		timestep++;
		usleep(SLEEP_TIME);
		dumbInteract();
		//saveLatticeSnapshot("demo", timestep);
		total += when() - timer;
	}
	printf("Avg. Time: %lf\n", total/(double)timestep);
	running = 0;
	if(zombie_count > 0) {
		printf("Zombies Win!!!\n");
	}
	else {
		printf("Humans wind!!!\n");
	}
	unpopulateCity();
	pthread_join(visualizerThread, NULL);
	return 0;
}

double when() {
        struct timeval tp;
        gettimeofday(&tp, NULL);
        return ((double) tp.tv_sec + (double) tp.tv_usec * 1e-6);
}

void dumbInteract(void) {
	int i;
	int j;
	entity_t* entity;
	#pragma omp parallel for default(shared) private(i, j, entity) firstprivate(lattice_height, lattice_width)
	for (i = 0; i < lattice_height; i++) {
		for (j = 0; j < lattice_width; j++) {
			if (lattice[i][j].occupant == NULL) continue;
			entity = lattice[i][j].occupant;
			entity->act(entity);
		}
	}
	//list_act(entity_list);
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


