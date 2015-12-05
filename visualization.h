#ifndef VISUALIZATION_H
#define VISUALIZATION_H
#include <stdint.h>
#include "entity.h"
#include "cell.h"

typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} pixel_t;

void intHandler();
pixel_t getColorRepresentation(cell_t* cell);
void createLatticeImage(char* filename);
void saveLatticeSnapshot(char* basename, int timestep);
int savePNG(pixel_t *pixels, const char *path);
void *visualizer(void* args);
void drawLattice(void);

#endif
