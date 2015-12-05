#include <time.h>
#include <stdlib.h>

#include "constants.h"
#include "entity.h"
#include "cell.h"
#include "block_generator.h"

extern int lattice_height;
extern int lattice_width;
extern cell_t lattice[][LATTICE_WIDTH];

void makeSimpleClosedBuildings(void) {
	int i;
	int j;
	int row_marker;
	int col_marker;
	int road_width;
	int building_height;
	int building_width;
	row_marker = 0;
	col_marker = 0;
	road_width = 15;
	while (row_marker < lattice_height) {
		building_height = rand() % 15 + 5;
		for (i = 0; i < building_height && (i+row_marker) < lattice_height; i++) {
			for (j = 0; j < lattice_width; j++) {
				lattice[row_marker+i][j].type = type_barrier;
			}
		}
		row_marker += road_width;
	}
	while (col_marker < lattice_width) {
		building_width = rand() % 5 + 5;
		for (j = 0; j < building_width && (j+col_marker) < lattice_width; j++) {
			for (i = 0; i < lattice_width; i++) {
				lattice[i][col_marker+j].type = type_open;
			}
		}
		col_marker += 10 + road_width;
	}
	return;
}
