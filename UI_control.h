// UI_control.h
#pragma once
#include "inout.h"
#include "utils.h"

#include "player.h"
#include "inventory.h"
#include "item.h"
#include "store.h"

#include "UI_info.h"

void UI_control_init(UI_state_t* ui_main_state, title_state_t* ui_title_state, player_action_t* ui_battle_state);
void UI_control_title(UI_state_t* ui_main_state, title_state_t* ui_title_state, int menu_key);
void UI_control_setting(UI_state_t* ui_main_state, setting_state_t* ui_setting_state, int* global_volume, int key);

void UI_control_game_mode(UI_state_t* ui_main_state, game_mode_state_t* ui_mode_state,
	int* game_mode, int key, bool is_infinite_unlocked
);

void UI_control_hero_select(UI_state_t* ui_main_state, hero_t* choice_hero, int key);

player_action_t UI_control_player_action(player_action_t* ui_battle_state, int menu_key);

// 무기 장착 여부 반환 (0 변경 없음, 1 무기 변경, 2 방어구 변경)
int  UI_control_inventory(UI_state_t* ui_main_state, player_t* player, int menu_key);
void UI_control_store(UI_state_t* ui_main_state, player_t* player, int menu_key);