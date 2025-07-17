// UI_dynamic.h
#pragma once

#include "inout.h"
#include "utils.h"

#include "item.h"
#include "inventory.h"

#include "UI_info.h"
#include "UI_static.h"
#include "UI_cleaner.h"

typedef struct { int x; int y; const char* text; } menu_list;

// 정적 함수
static void s_print_stat_bonus(equipment_t* current_equipment_list, player_t* player, int index, int y, int type);
static void s_print_inventory_item_page(equipment_t* current_equipment_list, pair_t* weapon_inventory, pair_t* armor_inventory, int ui_inventory_sub_title_State, player_t* player, int focus_level, int selected_item_index, int page, int type);
static void s_print_store_item_page(equipment_t* current_equipment_list, pair_t* weapon_inventory, pair_t* armor_inventory, int ui_store_sub_title_state, player_t* player, int store_focus_level, int selected_item_index, int store_buy_sell_state, int page, int type);
static void s_print_sub_menu_box(menu_list menu[], int focus_level, int ui_sub_title_state);
static void s_confirm_player_name_selection(int title_state);

// 사용자 이름 입력 관련 함수
void  UI_dynamic_player_name_input(void);
bool  UI_dynamic_confirm_player_name(const char* name);
char* UI_dynamic_create_player_name(void);

void  UI_dynamic_title_selection(int ui_title_state);
void  UI_dynamic_battle_selection(int ui_battle_state);

// 동적 ui 생성 함수
void  UI_dynamic_monster_info(monster_t* monster);
void  UI_dynamic_player_info(player_t* player);

void  UI_dynamic_inventory_info(player_t* player, pair_t* weapon_inventory, pair_t* armor_inventory, int ui_inventory_state, int ui_inventory_sub_title_State, int focus_level, int selected_item_index, int weapon_page, int armor_page);
void  UI_dynamic_current_weapon_info(player_t* player);
void  UI_dynamic_current_armor_info(player_t* player);

void UI_dynamic_store_info(player_t* player, pair_t* weapon_inventory, pair_t* armor_inventory, int ui_store_state, int ui_sotre_sub_title_state, int focus_level, int selected_item_index, int store_buy_sell_state, int weapon_page, int armor_page);
