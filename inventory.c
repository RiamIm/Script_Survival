// inventory.c
#define _CRT_SECURE_NO_WARNINGS
#include "inventory.h"
#include "item.h"

// extern으로 선언된 전역 변수 정의
pair_t weapon_inventory[RARITY_COUNT][ITEM_COUNT];
pair_t armor_inventory[RARITY_COUNT][ITEM_COUNT];
int heal_item_inventory[HEAL_ITEM_COUNT];

// ==================== 상태 변수 캡슐화 ====================
static inventory_state_t      s_inventory_state;
static focus_level_t          s_focus_level;
static equipment_rarity_t     s_rarity;
static int                    s_selected_item_index;
static int				      s_weapon_page;
static int                    s_armor_page;

void inventory_init(void) {
	for (int i = 0; i < RARITY_COUNT; i++) {
		for (int j = 0; j < ITEM_COUNT; j++) {
			weapon_inventory[i][j].is_was_having = FALSE;
			weapon_inventory[i][j].count = 0;
			armor_inventory[i][j].is_was_having = FALSE;
			armor_inventory[i][j].count = 0;
		}
	}

	for (int i = 0; i < HEAL_ITEM_COUNT; i++) {
		heal_item_inventory[i] = 0;
	}

	weapon_inventory[RARITY_NORMAL][0].is_was_having = TRUE; // 기본 무기 획득
	weapon_inventory[RARITY_NORMAL][0].count = 1; // 기본 무기 1개 획득

	armor_inventory[RARITY_NORMAL][0].is_was_having = TRUE; // 기본 방어구 획득
	armor_inventory[RARITY_NORMAL][0].count = 1; // 기본 방어구 1개 획득

	s_inventory_state = INVENTORY_STATE_BACK;
	s_focus_level = FOCUS_LEVEL_TOP;
	s_rarity = RARITY_NORMAL;
	s_selected_item_index = 0;
	s_weapon_page = 0;
	s_armor_page = 0;
}

void inventory_unlock_all_items(void)
{
	// 모든 무기/방어구 1개씩 지급, is_was_having을 true로 설정
	for (int r = 0; r < RARITY_COUNT; r++) {
		int item_max = rarity_item_counts[r];
		for (int i = 0; i < item_max; i++) {
			weapon_inventory[r][i].count = 1;
			weapon_inventory[r][i].is_was_having = true;
			armor_inventory[r][i].count = 1;
			armor_inventory[r][i].is_was_having = true;
		}
	}
	// 모든 소비 아이템은 0개로 초기화
	for (int i = 0; i < HEAL_ITEM_COUNT; i++) {
		heal_item_inventory[i] = 0;
	}
}

// --- Getter ---
inventory_state_t  get_inventory_state() { return s_inventory_state; }
focus_level_t      get_inventory_focus_level() { return s_focus_level; }
equipment_rarity_t get_inventory_rarity_type() { return s_rarity; }
int                get_inventory_selected_index() { return s_selected_item_index; }
int                get_inventory_weapon_page() { return s_weapon_page; }
int                get_inventory_armor_page() { return s_armor_page; }

// --- Setter ---
void set_inventory_state(inventory_state_t new_state) { s_inventory_state = new_state; }
void set_inventory_focus_level(focus_level_t new_focus) { s_focus_level = new_focus; }
void set_inventory_rarity_type(equipment_rarity_t new_rarity) { s_rarity = new_rarity; }
void set_inventory_selected_index(int new_index) { s_selected_item_index = new_index; }
void set_inventory_weapon_page(int new_page) { s_weapon_page = new_page; }
void set_inventory_armor_page(int new_page) { s_armor_page = new_page; }


// type 0: 무기, 1: 방어구, 3: 소비 아이템
void get_item(equipment_rarity_t rarity, int item_index, int type)
{
	int item_count = rarity_item_counts[rarity]; // rarity에 따라 아이템 개수 조정

	if (type == 0) { // 무기
		if (item_index >= 0 && item_index < item_count) {
			weapon_inventory[rarity][item_index].is_was_having = TRUE;
			weapon_inventory[rarity][item_index].count++;
		}
	} 
	else if (type == 1) { // 방어구
		if (item_index >= 0 && item_index < item_count) {
			armor_inventory[rarity][item_index].is_was_having = TRUE;
			armor_inventory[rarity][item_index].count++;
		}
	}
	else { // 소비 아이템
		if (item_index >= 0 && item_index < HEAL_ITEM_COUNT) {
			heal_item_inventory[item_index]++;
		}
	}
}

//void inventory_get_all_items_for_test()
//{
//	for (int i = 0; i < INVENTORY_SIZE; i++) {
//		// 모든 무기를 1개씩 획득
//		weapon_inventory[i].is_was_having = TRUE;
//		weapon_inventory[i].count = 1;
//
//		// 모든 방어구를 1개씩 획득
//		armor_inventory[i].is_was_having = TRUE;
//		armor_inventory[i].count = 1;
//	}
//}

//void inventory_get_all_heal_items_for_test()
//{
//	for (int i = 0; i < HEAL_ITEM_COUNT; i++) {
//		heal_item_inventory[i] = 1;
//	}
//}

void sell_item(equipment_rarity_t rarity, int item_index, int type)
{
	int item_count = rarity_item_counts[rarity]; // rarity에 따라 아이템 개수 조정

	if (type == 0) { // 무기
		if (item_index >= 0 && item_index < item_count) {
			if (weapon_inventory[rarity][item_index].count > 0) {
				weapon_inventory[rarity][item_index].count--;
			}
		}
	}
	else if (type == 1) { // 방어구
		if (item_index >= 0 && item_index < item_count) {
			if (armor_inventory[rarity][item_index].count > 0) {
				armor_inventory[rarity][item_index].count--;
			}
		}
	}
	else { // 소비 아이템
		if (item_index >= 0 && item_index < HEAL_ITEM_COUNT) {
			if (heal_item_inventory[item_index] > 0) {
				heal_item_inventory[item_index]--;
			}
		}
	}
}