#ifndef ENTITY_H
#define ENTITY_H

typedef enum entity_type_t {
	civilian,
	police,
	zombie,
} entity_type_t;

typedef struct entity_t {
	entity_type_t type;
	struct entity_t* next;
	void* data;
	void* (*init)(int x, int y);
	int (*act)(void* data);
	int (*die)(void* data);
} entity_t;

entity_t* create_entity(entity_type_t type);

#endif
