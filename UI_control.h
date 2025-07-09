// UI_control.h
#pragma once
#include "inout.h"
#include "utils.h"

#include "UI_info.h"

void UI_control_init(int* current_ui_state, int* title_state, int* setting_state, int* battle_state, int* inventory_state, int* store_state);

void			UI_control_title(int* ui_main_state, int* ui_title_state, int menu_key);
void			UI_control_setting(int* ui_setting_state, int menu_key);
void			UI_control_inventory(int* ui_main_state, int* ui_inventory_state, int menu_key);
void			UI_control_store(int menu_key);
battle_action_t UI_control_battle(int* ui_battle_state, int menu_key);