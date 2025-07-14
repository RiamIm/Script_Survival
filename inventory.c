// inventory.c
#define _CRT_SECURE_NO_WARNINGS
#include "inventory.h"

// extern으로 선언된 전역 변수 정의
pair_t weapon_inventory[INVENTORY_SIZE];
pair_t armor_inventory[INVENTORY_SIZE];
int heal_item_inventory[HEAL_ITEM_COUNT]; 

void inventory_init(void) {
    for (int i = 0; i < INVENTORY_SIZE; i++) {
		weapon_inventory[i].is_was_having = FALSE;
		weapon_inventory[i].count = 0;
		armor_inventory[i].is_was_having = FALSE;
		armor_inventory[i].count = 0;
    }

    for (int i = 0; i < HEAL_ITEM_COUNT; i++) {
        heal_item_inventory[i] = 0;
    }

    // 기본 아이템 지급
    weapon_inventory[0].is_was_having = TRUE; // 숲-일반-단검
	weapon_inventory[0].count = 1;
    armor_inventory[0].is_was_having = TRUE;  // 숲-일반-가죽갑옷
	armor_inventory[0].count = 1;
}


// type 0: 무기, 1: 방어구, 3: 소비 아이템
void get_item(int item_index, int type)
{
	if (type == 0) { // 무기
		if (item_index >= 0 && item_index < INVENTORY_SIZE) {
			weapon_inventory[item_index].is_was_having = TRUE;
			weapon_inventory[item_index].count++;
		}
	} 
	else if (type == 1) { // 방어구
		if (item_index >= 0 && item_index < INVENTORY_SIZE) {
			armor_inventory[item_index].is_was_having = TRUE;
			armor_inventory[item_index].count++;
		}
	}
	else { // 소비 아이템
		if (item_index >= 0 && item_index < HEAL_ITEM_COUNT) {
			heal_item_inventory[item_index]++;
		}
	}
}

void inventory_get_all_items_for_test()
{
	for (int i = 0; i < INVENTORY_SIZE; i++) {
		// 모든 무기를 1개씩 획득
		weapon_inventory[i].is_was_having = TRUE;
		weapon_inventory[i].count = 1;

		// 모든 방어구를 1개씩 획득
		armor_inventory[i].is_was_having = TRUE;
		armor_inventory[i].count = 1;
	}
}