#ifndef ENTITY_LIST_H
#define ENTITY_LIST_H

#include "entity.h"

typedef struct list_node_t {
	struct entity_t* entity;
	struct list_node_t* prev;
	struct list_node_t* next;
} list_node_t;

typedef struct list_t {
	struct list_node_t* head;
	struct list_node_t* tail;
} list_t;

int list_add(list_t* list, struct entity_t* entity);
struct entity_t* list_remove(list_t* list, list_node_t* node);
void list_act(list_t* list);
list_t* list_create(void);
void list_free(list_t* list);

#endif
