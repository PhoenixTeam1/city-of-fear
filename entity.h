#ifndef ENTITY_H
#define ENTITY_H

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
	type_civilian,
	type_police,
	type_zombie,
} entity_type_t;

typedef struct entity_t {
	entity_type_t type;
	struct entity_t* next;
	int xpos;
	int ypos;
	int (*act)(struct entity_t* self);
	int (*die)(struct entity_t* self);
} entity_t;

void initEntity(entity_t* entity, int x, int y);
int move(entity_t* entity, direction_t direction);

#endif
