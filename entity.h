#ifndef ENTITY_H
#define ENTITY_H

typedef enum entity_type_t {
	civilian,
	police,
	zombie,
	barrier,
	open,
} entity_type_t;

typedef struct entity_t {
	entity_type_t type;
	void* data;
	int (*init)(struct entity_t* self);
	int (*act)(struct entity_t* self);
	int (*react)(struct entity_t* self);
	int (*die)(struct entity_t* self);
} entity_t;

#endif
