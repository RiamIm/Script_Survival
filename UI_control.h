// UI_control.h
#pragma once
#include "inout.h"
#include "utils.h"

#include "item.h"
#include "store.h"
#include "inventory.h"

#include "player.h"

#include "UI_info.h"
#include "game_manager.h"

void UI_control_init(UI_state_t* ui_main_state, title_state_t* ui_title_state, player_action_t* ui_battle_state);
void UI_control_title(UI_state_t* ui_main_state, title_state_t* ui_title_state, int menu_key);
void UI_control_setting(UI_state_t* ui_main_state, setting_state_t* ui_setting_state, bool is_come_esc_menu, int* global_volume, int key);

void UI_control_game_mode(UI_state_t* ui_main_state, game_mode_state_t* ui_mode_state,
	int* game_mode, int key, bool is_infinite_unlocked
);

void UI_control_select_new_or_load_game(UI_state_t* ui_main_state, new_or_load_game_t* new_or_load_game, int key);

void UI_control_save_load_menu(UI_state_t* ui_main_state, save_load_num_t* ui_save_load_num, int key, game_context_t* context);

void UI_control_hero_select(UI_state_t* ui_main_state, hero_t* choice_hero, int key);
void UI_control_generate_upgrade_choices(player_t* player, upgrade_type_t out_choices[]);

void UI_control_handle_upgrade_selection(UI_state_t* ui_main_state, player_t* player,
	const upgrade_type_t choices[], int* selection, int key);

player_action_t UI_control_player_action(player_action_t* ui_battle_state, int menu_key);

void UI_control_esc_menu(UI_state_t* ui_main_state, esc_menu_state_t* ui_esc_menu_state, int menu_key, int gamemode);

// 무기 장착 여부 반환 (0 변경 없음, 1 무기 변경, 2 방어구 변경)
int  UI_control_inventory(UI_state_t* ui_main_state, player_t* player, int menu_key);
void UI_control_store(UI_state_t* ui_main_state, player_t* player, int menu_key);