#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "entity.h"
#include "cell.h"
#include "block_generator.h"

#define NUM_TILES 11
#define TILE_HEIGHT 3
#define TILE_WIDTH 3
#define SCALAR 8

extern int lattice_height;
extern int lattice_width;
extern cell_t lattice[][LATTICE_WIDTH];

int tileset[NUM_TILES][TILE_HEIGHT][TILE_WIDTH] = { { {1,0,1}, {0,0,0}, {1,0,1} },
						    { {1,1,1}, {0,0,0}, {1,1,1} },
						    { {1,0,1}, {1,0,1}, {1,0,1} },
						    { {1,0,1}, {0,0,1}, {1,1,1} },
						    { {1,0,1}, {1,0,0}, {1,1,1} },
						    { {1,1,1}, {1,0,0}, {1,0,1} },
						    { {1,1,1}, {0,0,1}, {1,0,1} },
						    { {1,0,1}, {0,0,0}, {1,1,1} },
						    { {1,1,1}, {0,0,0}, {1,0,1} },
						    { {0,0,0}, {0,0,0}, {0,0,0} },
						    { {1,1,1}, {1,1,1}, {1,1,1} } };

int check_left(int tile, int i, int j) {
	int left_barrier;
	if (lattice[i+SCALAR][j-1].type == type_barrier) {
		left_barrier = 1;
	}
	else {
		left_barrier = 0;
	}
	if(tileset[tile][1][0] != left_barrier) {
		return 1;
	}
	return 0;
}

int check_down(int tile, int i, int j) {
	int up_barrier;
	if (lattice[i-1][j+SCALAR].type == type_barrier) {
		up_barrier = 1;
	}
	else {
		up_barrier = 0;
	}

	if(tileset[tile][0][1] != up_barrier) {
		return 1;
	}
	return 0;
}

int getTile() {
	double coin = (double)rand() / (double)RAND_MAX;
	if(coin < 0.5) {
		return rand() % (NUM_TILES - 1);
	}
	if(coin < 0.65) {
		return NUM_TILES;
	}
	else {
		return (rand() % 2) + 1;
	}
}

void makeSimpleClosedBuildings(void) {
	int i;
	int j;
	int t_r;
	int t_c;
	int row;
	int col;
	int tile = getTile();
	int tile_height = TILE_HEIGHT * SCALAR;
	int tile_width = TILE_WIDTH * SCALAR;

	for (i = 0; i < lattice_height; i += tile_height) {
		for (j = 0; j < lattice_width; j += tile_width) {
			if (!isValidLatticeCell(i+SCALAR, j+SCALAR)) continue;
			row = 0;
			if (i > 0 && j > 0) {
				while(check_down(tile, i, j) || check_left(tile, i, j)) {
					tile = getTile();
				}
			}
			else if (i > 0) {
				while(check_down(tile, i, j)) {
					tile = getTile();
				}
			}
			else if (j > 0) {
				while(check_left(tile, i, j)) {
					tile = getTile();
				}
			}
			for (t_r = 0; t_r < tile_height; t_r++) {
				if (t_r > 0 && t_r % SCALAR == 0) {
					row++;
				}
				col = 0;
				for (t_c = 0; t_c < tile_width; t_c++) {
					if (!isValidLatticeCell(i+t_r, j+t_c)) continue;
					if (t_c > 0 && t_c % SCALAR == 0) {
						col++;
					}
					if (tileset[tile][row][col]) {
						lattice[i+t_r][j+t_c].type = type_open;
					}
					else {
						lattice[i+t_r][j+t_c].type = type_open;
					}
				}
			}
			tile = getTile();
		}
	}
	return;
}
