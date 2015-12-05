#ifndef CELL_H
#define CELL_H

#include "entity.h"

typedef enum cell_type_t {
	type_barrier,
	type_open,
} cell_type_t;

typedef struct cell_t {
	cell_type_t type;
	entity_t* occupant;
} cell_t;

#endif
