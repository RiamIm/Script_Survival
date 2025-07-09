// UI_control.c 
#define _CRT_SECURE_NO_WARNINGS
#include "UI_control.h"

void UI_control_init(int* current_ui_state, int* title_state, int* setting_state, int* battle_state, int* inventory_state, int* store_state)
{
	*current_ui_state = UI_STATE_TITLE;
	*title_state = TITLE_STATE_START;
	*setting_state = 0;
	*battle_state = BATTLE_STATE_ATTACK;
	*inventory_state = INVENTORY_STATE_WEAPON;
	*store_state = 0;
}

void UI_control_title(int* ui_main_state, int* ui_title_state, int menu_key)
{
	if (menu_key == ENTER) {
		if (*ui_title_state == TITLE_STATE_START) {
			*ui_main_state = UI_STATE_CREATE_PLAYER_NAME;
		}
		else if (*ui_title_state == TITLE_STATE_OPTIONS) {
			*ui_main_state = UI_STATE_SETTING;
		}
		else if (*ui_title_state == TITLE_STATE_EXIT) {
			utils_gotoxy(0, 28);
			exit(0);
		}
	}

	else if (menu_key == UP) { 
		if (*ui_title_state == TITLE_STATE_START) *ui_title_state = TITLE_STATE_EXIT;    
		else if (*ui_title_state == TITLE_STATE_OPTIONS) *ui_title_state = TITLE_STATE_START;
		else if (*ui_title_state == TITLE_STATE_EXIT) *ui_title_state = TITLE_STATE_OPTIONS;
	}
	else if (menu_key == DOWN) { 
		if (*ui_title_state == TITLE_STATE_START) *ui_title_state = TITLE_STATE_OPTIONS;
		else if (*ui_title_state == TITLE_STATE_OPTIONS) *ui_title_state = TITLE_STATE_EXIT;
		else if (*ui_title_state == TITLE_STATE_EXIT) *ui_title_state = TITLE_STATE_START;  
	}
}

void UI_control_setting(int* ui_setting_state, int menu_key)
{
	// TODO
}

battle_action_t UI_control_battle(int* ui_battle_state, int menu_key)
{
	if (menu_key == ENTER) {
		if (*ui_battle_state == BATTLE_STATE_ATTACK) {
			return BATTLE_ACTION_ATTACK;
		}
		else if (*ui_battle_state == BATTLE_STATE_EXTORTION) {
			return BATTLE_ACTION_EXTORTION;
		}
		else if (*ui_battle_state == BATTLE_STATE_INVENTORY) {
			return BATTLE_ACTION_INVENTORY;
		}
	}

	else if (menu_key == UP) { 
		if (*ui_battle_state == BATTLE_STATE_ATTACK) *ui_battle_state = BATTLE_STATE_INVENTORY; 
		else if (*ui_battle_state == BATTLE_STATE_EXTORTION) *ui_battle_state = BATTLE_STATE_ATTACK;
		else if (*ui_battle_state == BATTLE_STATE_INVENTORY) *ui_battle_state = BATTLE_STATE_EXTORTION;
	}
	else if (menu_key == DOWN) { 
		if (*ui_battle_state == BATTLE_STATE_ATTACK) *ui_battle_state = BATTLE_STATE_EXTORTION;
		else if (*ui_battle_state == BATTLE_STATE_EXTORTION) *ui_battle_state = BATTLE_STATE_INVENTORY;
		else if (*ui_battle_state == BATTLE_STATE_INVENTORY) *ui_battle_state = BATTLE_STATE_ATTACK; 
	}

	return BATTLE_ACTION_NONE;
}

void UI_control_inventory(int menu_key)
{
	// TODO
}

void UI_control_store(int menu_key)
{
	// TODO
}