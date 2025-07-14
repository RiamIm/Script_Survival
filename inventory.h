// inventory.h
#pragma once
#include "inout.h"
#include "utils.h"

#include "player.h"

#define INVENTORY_SIZE 72
#define HEAL_ITEM_COUNT 6

typedef struct pair {
	int is_was_having;
	int count;
} pair_t;

extern pair_t weapon_inventory[INVENTORY_SIZE];
extern pair_t armor_inventory[INVENTORY_SIZE];
extern int heal_item_inventory[HEAL_ITEM_COUNT];

void inventory_init(void);
void get_item(int item_index, int type);