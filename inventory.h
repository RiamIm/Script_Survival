// inventory.h
#pragma once
#include "inout.h"
#include "utils.h"
#include "player.h"

#define INVENTORY_SIZE 23

typedef struct pair {
	int is_was_having;
	int count;
} pair_t;

pair_t weapon_inventory[INVENTORY_SIZE];
pair_t armor_inventory[INVENTORY_SIZE];	
int heal_item_inventory[INVENTORY_SIZE];

void inventory_init();