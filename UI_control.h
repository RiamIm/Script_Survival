// UI_control.h
#pragma once
#include "inout.h"
#include "utils.h"

#include "player.h"
#include "inventory.h"
#include "item.h"

#include "UI_info.h"
void UI_control_init(
	UI_state_t* ui_main_state, title_state_t* ui_title_state, battle_state_t* ui_battle_state,
	inventory_state_t* inventory_state, focus_level_t* inventory_focus_level, region_t* inventory_region, int* inventory_selected_index, int* inventory_weapon_page, int* inventory_armor_page,
	store_state_t* store_state, focus_level_t* store_focus_level, region_t* store_region, store_state_t* store_buy_sell_state, int* store_selected_index, int* store_weapon_page, int* store_armor_page
);

void UI_control_title(UI_state_t* ui_main_state, title_state_t* ui_title_state, int menu_key);
void UI_control_setting(UI_state_t* ui_main_state, setting_state_t* ui_setting_state, int* global_volume, int key);
void UI_control_game_mode(UI_state_t* ui_main_state, game_mode_state_t* ui_mode_state, int* game_mode, int key);

// 무기 장착 여부 반환 (0 변경 없음, 1 무기 변경, 2 방어구 변경)
int UI_control_inventory(
	UI_state_t* ui_main_state, inventory_state_t* current_inventory_state, region_t* current_region, 
	focus_level_t* focus_level, int* selected_item_index, int menu_key, int* weapon_page, int* armor_page, player_t* player
);
void UI_control_store(
	UI_state_t* ui_main_state, store_state_t* current_store_state, region_t* current_region,
	focus_level_t* focus_level, int* selected_item_index, int menu_key,
	int* weapon_page, int* armor_page, store_state_t* buy_sell_state, player_t* player
);

battle_action_t UI_control_battle(battle_state_t* ui_battle_state, int menu_key);