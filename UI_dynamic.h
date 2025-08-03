// UI_dynamic.h
#pragma once

#include "inout.h"
#include "utils.h"

#include "item.h"
#include "store.h"
#include "inventory.h"

#include "UI_info.h"
#include "UI_static.h"
#include "UI_cleaner.h"

#include "save_load.h"

typedef struct { int x; int y; const char* text; } menu_list;

// 사용자 이름 입력 관련 함수
void  UI_dynamic_player_name_input(void);
bool  UI_dynamic_confirm_player_name(const char* name);
char* UI_dynamic_create_player_name(void);

void  UI_dynamic_title_selection(int ui_title_state);
void  UI_dynamic_setting_menu(setting_state_t selected, int* global_volume);
void  UI_dynamin_esc_menu(esc_menu_state_t* selected, int gamemode);
void  UI_dynamic_save_load_menu(save_load_num_t* selected);

void  UI_dynamic_select_game_mode(game_mode_state_t selected, bool is_infinite_unlocked);
void  UI_dynamic_select_new_or_load_game(new_or_load_game_t* selected);
void  UI_dynamic_hero_select(hero_t selected_hero);

void  UI_dynamic_infinite_upgrade(player_t* player, const upgrade_type_t choices[], int selection);

void  UI_dynamic_player_action_selection(int ui_battle_state);
    
void  UI_dynamic_action_order(player_t* player, monster_t* monster);

// 동적 ui 생성 함수
void  UI_dynamic_monster_info(monster_t* monster, int current_stage);
void  UI_dynamic_player_info(player_t* player);

void  UI_dynamic_select_heal_or_store(heal_or_store_t* selected, player_t* player);

void  UI_dynamic_monster_flash_effect(monster_t* monster);

void  UI_dynamic_inventory_info(player_t* player);

void  UI_dynamic_store_info(player_t* player);

void  UI_dynamic_current_weapon_info(player_t* player);
void  UI_dynamic_current_armor_info(player_t* player);