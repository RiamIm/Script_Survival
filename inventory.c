#include <stdio.h>
#include "inventory.h"

void init_inventory() 
{
	for (int i = 0; i < INVENTORY_SIZE; i++) {
		weapon_inventory[i].is_was_having = 0;
		weapon_inventory[i].count = 0;
		armor_inventory[i].is_was_having = 0;
		armor_inventory[i].count = 0;
		heal_item_inventory[i] = 0;
	}
}