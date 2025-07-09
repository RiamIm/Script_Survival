// UI_control.c 
#define _CRT_SECURE_NO_WARNINGS
#include "UI_control.h"

void UI_control_init(int* ui_main_state, int* ui_title_state, int* ui_setting_state, int* ui_battle_state, int* ui_inventory_state, int* ui_store_state)
{
	*ui_main_state = UI_STATE_TITLE;
	*ui_title_state = TITLE_STATE_START;
	*ui_setting_state = 0;
	*ui_battle_state = BATTLE_STATE_ATTACK;
	*ui_inventory_state = INVENTORY_STATE_WEAPON;
	*ui_store_state = 0;
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

void UI_control_inventory(int* ui_main_state, int* ui_inventory_state, int menu_key)
{
	if (menu_key == ENTER) {
		if (*ui_inventory_state == INVENTORY_STATE_BACK) {
			*ui_main_state = UI_STATE_BATTLE; 
		}
		// else if (*ui_inventory_state == INVENTORY_STATE_OPTIONS) { /* 옵션 선택 시 로직 */ }
	}
	else if (menu_key == LEFT) {
		switch (*ui_inventory_state) {
		case INVENTORY_STATE_BACK: *ui_inventory_state = INVENTORY_STATE_OPTIONS; break; // [수정] 옵션으로 순환
		case INVENTORY_STATE_WEAPON: *ui_inventory_state = INVENTORY_STATE_BACK; break;
		case INVENTORY_STATE_ARMOR: *ui_inventory_state = INVENTORY_STATE_WEAPON; break;
		case INVENTORY_STATE_HEAL_ITEM: *ui_inventory_state = INVENTORY_STATE_ARMOR; break;
		case INVENTORY_STATE_OPTIONS: *ui_inventory_state = INVENTORY_STATE_HEAL_ITEM; break; // [추가] 옵션 -> 소비템
		}
	}
	else if (menu_key == RIGHT) { 
		switch (*ui_inventory_state) {
		case INVENTORY_STATE_BACK: *ui_inventory_state = INVENTORY_STATE_WEAPON; break;
		case INVENTORY_STATE_WEAPON: *ui_inventory_state = INVENTORY_STATE_ARMOR; break;
		case INVENTORY_STATE_ARMOR: *ui_inventory_state = INVENTORY_STATE_HEAL_ITEM; break;
		case INVENTORY_STATE_HEAL_ITEM: *ui_inventory_state = INVENTORY_STATE_OPTIONS; break; // [수정] 소비템 -> 옵션
		case INVENTORY_STATE_OPTIONS: *ui_inventory_state = INVENTORY_STATE_BACK; break; // [추가] 옵션 -> 뒤로가기 순환
		}
	}
}

void UI_control_store(int menu_key)
{
	// TODO
}