// inventory.c
#define _CRT_SECURE_NO_WARNINGS
#include "inventory.h"

void inventory_init() 
{
	for (int i = 0; i < INVENTORY_SIZE; i++) {
		if (i == 0) {
			weapon_inventory[i].is_was_having = TRUE; // 기본 무기 1개 보유
			weapon_inventory[i].count = 1;
			armor_inventory[i].is_was_having = TRUE; // 기본 방어구 1개 보유
			armor_inventory[i].count = 1;
		}
		else {
			weapon_inventory[i].is_was_having = FALSE;
			weapon_inventory[i].count = 0;
			armor_inventory[i].is_was_having = FALSE;
			armor_inventory[i].count = 0;
			heal_item_inventory[i] = 0;
		}
	}

	//디버그용
	weapon_inventory[4].is_was_having = TRUE;
}