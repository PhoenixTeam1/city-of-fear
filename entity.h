#ifndef ENTITY_H
#define ENTITY_H

#include "entity_list.h"

typedef enum direction_t {
	north,
	northeast,
	east,
	southeast,
	south,
	southwest,
	west,
	northwest,
	MAX_DIRECTIONS
} direction_t;

typedef enum entity_type_t {
	type_none,
	type_civilian,
	type_police,
	type_zombie,
} entity_type_t;

typedef struct entity_t {
	entity_type_t type;
	struct list_node_t* listnode;
	int xpos;
	int ypos;
	int (*act)(struct entity_t* self);
	int (*die)(struct entity_t* self);
} entity_t;

void initEntity(entity_t* entity, int x, int y);
int move(entity_t* entity, direction_t direction);
entity_t* getNeighbor(entity_t* entity, direction_t direction);
void killEntity(entity_t* entity);
void spawnEntity(entity_t* entity);
int lookAhead(entity_t entity, direction_t direction, entity_type_t lookFor, int fov, int range);

#endif
