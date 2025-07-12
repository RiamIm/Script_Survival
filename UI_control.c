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

void UI_control_inventory(int* ui_main_state, int* ui_inventory_state, int* focus_level, int* selected_item_index, int menu_key, int* weapon_page, int* armor_page)
{
	// 지금 포커스가 탑에 있을 때, 뒤로가기키랑 소비아이템 쪽에서 키가 안먹는 버그 발생

	int* page = NULL; // 페이지 변수

	if (*ui_inventory_state == INVENTORY_STATE_WEAPON)
	{
		page = weapon_page;
	}
	else if (*ui_inventory_state == INVENTORY_STATE_ARMOR)
	{
		page = armor_page;
	}

	if (page == NULL) return;

	if (*focus_level == INVENTORY_FOCUS_LEVEL_TOP) // 포커스가 상단 카테고리에 있을 때
	{
		if (menu_key == ENTER) {
			if (*ui_inventory_state == INVENTORY_STATE_BACK) {
				*ui_main_state = UI_STATE_BATTLE; // 전투 상태로 돌아가기
			}
			else if (*ui_inventory_state == INVENTORY_STATE_WEAPON || *ui_inventory_state == INVENTORY_STATE_ARMOR) {
				*focus_level = INVENTORY_FOCUS_LEVEL_ITEM_LIST; // 포커스를 서브 (지역 선택)으로 이동	
				*selected_item_index = *page * 6;
			}
			else if (*ui_inventory_state == INVENTORY_STATE_HEAL_ITEM) {
				*focus_level = INVENTORY_FOCUS_LEVEL_ITEM_LIST; // 포커스를 아이템 리스트로 이동
				*selected_item_index = 0; // 아이템 선택은 첫 번째부터
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
	}
	else if (*focus_level == INVENTORY_FOCUS_LEVEL_ITEM_LIST) // 포커스가 아이템 리스트에 있을 때
	{
		int page_start = (*page) * ITEMS_PER_PAGE;
		int page_end = page_start + ITEMS_PER_PAGE - 1;
		if (page_end >= s_total_items) page_end = s_total_items - 1;
		if (menu_key == ENTER) {
			// 아이템 사용/장착 로직 (예: use_weapon(*selected_item_index, player);)
		}
		else if (menu_key == ESC) {
			*focus_level = INVENTORY_FOCUS_LEVEL_TOP; // 포커스를 상단 카테고리로 이동
		}
		else if (menu_key == UP) {
			if (*selected_item_index > 0) {
				// 같은 페이지 안에서 위로 한 칸
				(*selected_item_index)--;

				// 만약 이전 페이지로 넘어가야 하면
				if (*selected_item_index < page_start) {
					(*page)--;
					// 이전 페이지의 마지막 칸으로
					*selected_item_index = (*page) * ITEMS_PER_PAGE + (ITEMS_PER_PAGE - 1);
					// 실제 아이템 수보다 크면 끝으로 클램프
					if (*selected_item_index >= s_total_items)
						*selected_item_index = s_total_items - 1;
				}
			}
			// else: 맨 처음 아이템이므로 무시
		}
		else if (menu_key == DOWN) {
			if (*selected_item_index < s_total_items - 1) {
				// 같은 페이지 안에서 아래로 한 칸
				(*selected_item_index)++;

				// 만약 다음 페이지로 넘어가야 하면
				if (*selected_item_index > page_end) {
					(*page)++;
					// 다음 페이지의 첫 칸으로
					*selected_item_index = (*page) * ITEMS_PER_PAGE;
				}
			}
			// else: 맨 마지막 아이템이므로 무시
		}
		// 왼쪽 끝 오른쪽 끝 이동 했을 때 이동 못하게 예외처리
		else if (menu_key == LEFT) {
			// 같은 row 내에서의 상대적 위치 계산 (0,1,2)
			int first_col_top = (*page) * ITEMS_PER_PAGE;
			int second_col_top = first_col_top + 3;
			if (*selected_item_index < second_col_top) {
				// --- 첫 번째 열에 있을 때 ---
				if (*page > 0) {
					int row = *selected_item_index - first_col_top;
					*page -= 1;
					// 이전 페이지의 두 번째 열 same row
					*selected_item_index = (*page) * 6 + 3 + row;
				}
				// else page==0 이면 무시
			}
			else {
				// --- 두 번째 열에 있을 때 ---
				// 같은 페이지 첫 번째 열로
				*selected_item_index -= 3;
			}
		}
		else if (menu_key == RIGHT) {
			int first_col_top = (*page) * 6;
			int second_col_top = first_col_top + 3;
			if (*selected_item_index >= second_col_top) {
				// --- 두 번째 열에 있을 때 ---
				if (*page < 3) {
					int row = *selected_item_index - second_col_top;
					*page += 1;
					// 다음 페이지의 첫 번째 열 same row
					*selected_item_index = (*page) * 6 + row;
				}
				// else 마지막 페이지면 무시
			}
			else {
				// --- 첫 번째 열에 있을 때 ---
				// 같은 페이지 두 번째 열로
				*selected_item_index += 3;
			}
		}
	}
}

void UI_control_store(int menu_key)
{
	// TODO
}