// inventory.c
#define _CRT_SECURE_NO_WARNINGS
#include "inventory.h"

// extern으로 선언된 전역 변수 정의
pair_t weapon_inventory[INVENTORY_SIZE];
pair_t armor_inventory[INVENTORY_SIZE];
int heal_item_inventory[HEAL_ITEM_COUNT];

// ==================== 상태 변수 캡슐화 ====================
static inventory_state_t s_inventory_state;
static focus_level_t     s_focus_level;
static region_t          s_region;
static int               s_selected_item_index;
static int               s_weapon_page;
static int               s_armor_page;

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

	// 캡슐화된 상태 변수 초기화
	s_inventory_state = INVENTORY_STATE_WEAPON;
	s_focus_level = FOCUS_LEVEL_TOP;
	s_region = REGION_FOREST;
	s_selected_item_index = 0;
	s_weapon_page = 0;
	s_armor_page = 0;
}

// --- Getter ---
inventory_state_t get_inventory_state() { return s_inventory_state; }
focus_level_t     get_inventory_focus_level() { return s_focus_level; }
region_t          get_inventory_region() { return s_region; }
int               get_inventory_selected_index() { return s_selected_item_index; }
int               get_inventory_weapon_page() { return s_weapon_page; }
int               get_inventory_armor_page() { return s_armor_page; }

// --- Setter ---
void set_inventory_state(inventory_state_t new_state) { s_inventory_state = new_state; }
void set_inventory_focus_level(focus_level_t new_focus) { s_focus_level = new_focus; }
void set_inventory_region(region_t new_region) { s_region = new_region; }
void set_inventory_selected_index(int new_index) { s_selected_item_index = new_index; }
void set_inventory_weapon_page(int new_page) { s_weapon_page = new_page; }
void set_inventory_armor_page(int new_page) { s_armor_page = new_page; }


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

void inventory_get_all_heal_items_for_test()
{
	for (int i = 0; i < HEAL_ITEM_COUNT; i++) {
		heal_item_inventory[i] = 1;
	}
}

void sell_item(int item_index, int type)
{
	if (type == 0) { // 무기
		if (item_index >= 0 && item_index < INVENTORY_SIZE) {
			weapon_inventory[item_index].count--;
			if (weapon_inventory[item_index].count <= 0) {
			}
		}
	} 
	else if (type == 1) { // 방어구
		if (item_index >= 0 && item_index < INVENTORY_SIZE) {
			armor_inventory[item_index].count--;
			if (armor_inventory[item_index].count <= 0) {
			}
		}
	}
	else { // 소비 아이템
		if (item_index >= 0 && item_index < HEAL_ITEM_COUNT && heal_item_inventory[item_index] > 0) {
			heal_item_inventory[item_index]--;
		}
	}
}