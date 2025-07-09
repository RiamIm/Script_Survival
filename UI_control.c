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

void UI_control_inventory(int* ui_main_state, int* ui_inventory_state, int* focus_level, int* selected_item_index, int menu_key)
{
	if (*focus_level == 0) // 포커스가 상단 카테고리에 있을 때
	{
		if (menu_key == ENTER) {
			if (*ui_inventory_state == INVENTORY_STATE_BACK) {
				*ui_main_state = UI_STATE_BATTLE; // 전투 상태로 돌아가기
			}
		}
		else if (menu_key == LEFT) {
			switch (*ui_inventory_state) {
			case INVENTORY_STATE_BACK: *ui_inventory_state = INVENTORY_STATE_OPTIONS; break;
			case INVENTORY_STATE_WEAPON: *ui_inventory_state = INVENTORY_STATE_BACK; break;
			case INVENTORY_STATE_ARMOR: *ui_inventory_state = INVENTORY_STATE_WEAPON; break;
			case INVENTORY_STATE_HEAL_ITEM: *ui_inventory_state = INVENTORY_STATE_ARMOR; break;
			case INVENTORY_STATE_OPTIONS: *ui_inventory_state = INVENTORY_STATE_HEAL_ITEM; break;
			}
		}
		else if (menu_key == RIGHT) {
			switch (*ui_inventory_state) {
			case INVENTORY_STATE_BACK: *ui_inventory_state = INVENTORY_STATE_WEAPON; break;
			case INVENTORY_STATE_WEAPON: *ui_inventory_state = INVENTORY_STATE_ARMOR; break;
			case INVENTORY_STATE_ARMOR: *ui_inventory_state = INVENTORY_STATE_HEAL_ITEM; break;
			case INVENTORY_STATE_HEAL_ITEM: *ui_inventory_state = INVENTORY_STATE_OPTIONS; break;
			case INVENTORY_STATE_OPTIONS: *ui_inventory_state = INVENTORY_STATE_BACK; break;
			}
		}
		else if (menu_key == DOWN) {
			// 아이템 목록이 있는 카테고리에서만 아래로 이동 가능
			if (*ui_inventory_state == INVENTORY_STATE_WEAPON || *ui_inventory_state == INVENTORY_STATE_ARMOR || *ui_inventory_state == INVENTORY_STATE_HEAL_ITEM) {
				*focus_level = 1; // 포커스를 아이템 리스트로 변경
				*selected_item_index = 0; // 아이템 선택은 첫 번째부터
			}
		}
	}
	else if (*focus_level == 1) // 포커스가 아이템 리스트에 있을 때
	{
		if (menu_key == ENTER) {
			// 아이템 사용/장착 로직 (예: use_weapon(*selected_item_index, player);)
		}
		else if (menu_key == UP) {
			if (*selected_item_index < 5) { // 첫 번째 줄에 있을 때
				*focus_level = 0; // 포커스를 상단 카테고리로 이동
			}
			else {
				*selected_item_index -= 5;
			}
		}
		else if (menu_key == DOWN) {
			if (*selected_item_index < 5) { // 첫 번째 줄에 있을 때
				*selected_item_index += 5;
			}
		}
		else if (menu_key == LEFT) {
			if (*selected_item_index >= 5) { // 오른쪽에 있을 때
				*selected_item_index -= 5;
			}
		}
		else if (menu_key == RIGHT) {
			if (*selected_item_index < 5) { // 왼쪽에 있을 때
				*selected_item_index += 5;
			}
		}
	}
}

void UI_control_store(int menu_key)
{
	// TODO
}