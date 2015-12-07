#include <assert.h>
#include <stdlib.h>

#include "entity_list.h"
#include "entity.h"

static list_node_t* list_node_create(entity_t* entity);
static void list_node_free(list_node_t* node);

list_node_t* list_node_create(entity_t* entity) {
	list_node_t* node;
	node = (list_node_t*)malloc(sizeof(list_node_t));
	if (node == NULL) {
		return NULL;
	}
	node->prev = NULL;
	node->next = NULL;
	node->entity = entity;
	entity->listnode = node;
	return node;
}

void list_node_free(list_node_t* node) {
	if (node == NULL) {
		return;
	}
	free(node);
	return;
}

void list_act(list_t* list) {
	list_node_t* current;
	if (list == NULL) {
		return;
	}
	if (list->head == NULL) {
		return;
	}
	
	/* List is not empty */
	current = list->head;
	while (current != NULL) {
		current->entity->act(current->entity);
		current = current->next;
	}
	return;
}

entity_t* list_remove(list_t* list, list_node_t* node) {
	entity_t* entity;
	assert(list != NULL);
	assert(list->head != NULL);
	assert(list->tail != NULL);
	
	/* Node to remove is the head */
	if (list->head == node) {
		list->head = node->next;
	}
	/* Node to remove is the tail */
	if (list->tail == node) {
		list->tail = node->prev;
	}

	if (node->next != NULL) {
		node->next->prev = node->prev;
	}
	if (node->prev != NULL) {
		node->prev->next = node->next;
	}

	entity = node->entity;
	list_node_free(node);
	return entity;
}

/* Returns 0 on success */
int list_add(list_t* list, entity_t* entity) {
	list_node_t* new_node;
	list_node_t* tmp;
	new_node = list_node_create(entity);
	if (new_node == NULL) {
		return 1;
	}
	/* Empty list */
	if (list->head == NULL) {
		assert(list->tail == NULL);
		list->head = new_node;
		list->tail = new_node;
		return 0;
	}

	/* Non empty list */
	tmp = list->tail;
	tmp->next = new_node;
	new_node->prev = tmp;
	list->tail = new_node;
	return 0;
}

list_t* list_create(void) {
	list_t* list;
	list = (list_t*)malloc(sizeof(list_t));
	if (list == NULL) {
		return NULL;
	}
	list->head = NULL;
	list->tail = NULL;
	return list;
}

void list_free(list_t* list) {
	list_node_t* current;
	list_node_t* tmp;
	if (list == NULL) {
		return;
	}
	if (list->head == NULL) {
		assert(list->tail == NULL);
		free(list);
		return;
	}
	
	/* List is not empty */
	current = list->head;
	while (current != NULL) {
		tmp = current;
		current = current->next;
		list_node_free(tmp);
	}
	free(list);
	return;
}

