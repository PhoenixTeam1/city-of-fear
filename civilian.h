#ifndef CIVILIAN_H
#define CIVILIAN_H

#include "entity.h"

int civ_init(entity_t* self);
int civ_act(entity_t* self);
int civ_react(entity_t* self);
int civ_die(entity_t* self);

#endif
