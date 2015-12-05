#include <stdlib.h>
#include "entity.h"
#include "civilian.h"

void* default_init(int x, int y);
int default_act(void* data);
int default_die(void* data);

entity_t* create_entity(entity_type_t type) {
	entity_t* entity;
	entity = (entity_t*)malloc(sizeof(entity_t));
	entity->type = type;
	entity->next = NULL;
	switch (entity->type) {
		case civilian:
			entity->init = (void* (*)(int, int))civ_init;
			entity->act = (int (*)(void*))civ_act;
			entity->die = (int (*)(void*))civ_die;
			break;
		case police:
			entity->init = default_init;
			entity->act = default_act;
			entity->die = default_die;
			break;
		case zombie:
			entity->init = default_init;
			entity->act = default_act;
			entity->die = default_die;
			break;
		default:
			entity->init = default_init;
			entity->act = default_act;
			entity->die = default_die;
			break;
	}
	return entity;
}

void* default_init(int x, int y) {
	return 0;
}
int default_act(void* data) {
	return 0;
}
int default_die(void* data) {
	return 0;
}

