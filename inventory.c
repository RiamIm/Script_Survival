// inventory.c
#define _CRT_SECURE_NO_WARNINGS
#include "inventory.h"

// externÀ¸·Î ¼±¾ðµÈ Àü¿ª º¯¼ö Á¤ÀÇ
pair_t weapon_inventory[INVENTORY_SIZE];
pair_t armor_inventory[INVENTORY_SIZE];
int heal_item_inventory[HEAL_ITEM_COUNT]; 

void inventory_init() {
    for (int i = 0; i < INVENTORY_SIZE; i++) {
		weapon_inventory[i].is_was_having = FALSE;
		weapon_inventory[i].count = 0;
		armor_inventory[i].is_was_having = FALSE;
		armor_inventory[i].count = 0;
    }

    for (int i = 0; i < HEAL_ITEM_COUNT; i++) {
        heal_item_inventory[i] = 0;
    }

    // ±âº» ¾ÆÀÌÅÛ Áö±Þ
    weapon_inventory[0].is_was_having = TRUE; // ½£-ÀÏ¹Ý-´Ü°Ë
	weapon_inventory[0].count = 1;
    armor_inventory[0].is_was_having = TRUE;  // ½£-ÀÏ¹Ý-°¡Á×°©¿Ê
	armor_inventory[0].count = 1;

	// --- Å×½ºÆ®¿ë ¾ÆÀÌÅÛ È¹µæ ---
    weapon_inventory[1].is_was_having = 1; // ½£-Èñ±Í-Ã¤Âï°Ë
    weapon_inventory[24].is_was_having = 1; // »ç¸·-ÀÏ¹Ý-½Ã¹ÌÅÍ
    armor_inventory[25].is_was_having = 1; // »ç¸·-Èñ±Í-ÅÂ¾ç¹®¾çÈä°©
}