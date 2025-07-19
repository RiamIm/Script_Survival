// UI_dynamic.h
#pragma once

#include "inout.h"
#include "utils.h"

#include "item.h"
#include "inventory.h"
#include "store.h"

#include "UI_info.h"
#include "UI_static.h"
#include "UI_cleaner.h"

typedef struct { int x; int y; const char* text; } menu_list;

// 정적 함수
static void s_print_stat_bonus(equipment_t* current_equipment_list, player_t* player, int index, int y, int type);

static void s_print_inventory_item_page(
	equipment_t* current_equipment_list, pair_t* inventory, region_t current_region,
	player_t* player, focus_level_t focus_level, int selected_item_index, int page, int type
);

static void s_print_store_item_page(
	equipment_t* current_equipment_list, pair_t* inventory, region_t current_region,
	player_t* player, focus_level_t focus_level, int selected_item_index,
	store_state_t buy_sell_state, int page, int type
);

static void s_print_sub_menu_box(const menu_list menus[], focus_level_t focus_level, region_t current_region);
static void s_confirm_player_name_selection(int title_state);

// 사용자 이름 입력 관련 함수
void  UI_dynamic_player_name_input(void);
bool  UI_dynamic_confirm_player_name(const char* name);
char* UI_dynamic_create_player_name(void);

void  UI_dynamic_title_selection(int ui_title_state);
void  UI_dynamic_setting_menu(setting_state_t selected, int* global_volume);
void  UI_dynamic_select_game_mode(game_mode_state_t selected);
void  UI_dynamic_player_action_selection(int ui_battle_state);

void UI_dynamic_action_order(player_t* player, monster_t* monster);

// 동적 ui 생성 함수
void  UI_dynamic_monster_info(monster_t* monster);
void  UI_dynamic_player_info(player_t* player);

void UI_dynamic_inventory_info(
	player_t* player, pair_t* weapon_inventory, pair_t* armor_inventory,
	inventory_state_t current_inventory_state, region_t current_region, focus_level_t focus_level,
	int selected_item_index, int weapon_page, int armor_page
);

void UI_dynamic_store_info(
	player_t* player, pair_t* weapon_inventory, pair_t* armor_inventory,
	store_state_t current_store_state, region_t current_region, focus_level_t focus_level,
	store_buy_sell_state_t buy_sell_successful, int selected_item_index, store_state_t buy_sell_state, int weapon_page, int armor_page
);

void  UI_dynamic_current_weapon_info(player_t* player);
void  UI_dynamic_current_armor_info(player_t* player);