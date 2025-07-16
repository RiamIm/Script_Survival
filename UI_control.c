// UI_control.c 
#define _CRT_SECURE_NO_WARNINGS
#include "UI_control.h"

void UI_control_init(int* ui_main_state, int* ui_title_state, int* ui_setting_state, int* ui_battle_state, int* ui_inventory_state, int* ui_store_state)
{
	*ui_main_state = UI_STATE_TITLE;
	*ui_title_state = TITLE_STATE_START;
	*ui_setting_state = 0;
	*ui_battle_state = BATTLE_STATE_ATTACK;
	*ui_inventory_state = STORE_STATE_WEAPON;
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

void UI_control_inventory(int* ui_main_state, int* ui_inventory_state, int* ui_inventory_sub_title_state, int* inventory_focus_level, int* selected_item_index, int menu_key, int* weapon_page, int* armor_page)
{
	int* page = NULL; // 페이지 변수

	if (*ui_inventory_state == INVENTORY_STATE_WEAPON)
	{
		page = weapon_page;
	}
	else if (*ui_inventory_state == INVENTORY_STATE_ARMOR)
	{
		page = armor_page;
	}

	if (*inventory_focus_level == FOCUS_LEVEL_TOP) // 포커스가 상단 카테고리에 있을 때
	{
		if (menu_key == ENTER) {
			if (*ui_inventory_state == INVENTORY_STATE_BACK) {
				*ui_main_state = UI_STATE_BATTLE; // 전투 상태로 돌아가기
			}
			else if (*ui_inventory_state == INVENTORY_STATE_WEAPON || *ui_inventory_state == INVENTORY_STATE_ARMOR) {
				*inventory_focus_level = FOCUS_LEVEL_SUB; // 포커스를 서브 메뉴로 이동
			}
			else if (*ui_inventory_state == INVENTORY_STATE_HEAL_ITEM) {
				*inventory_focus_level = FOCUS_LEVEL_ITEM_LIST; // 포커스를 아이템 리스트로 이동
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
	else if (*inventory_focus_level == FOCUS_LEVEL_SUB) // 포커스가 서브 메뉴에 있을 때
	{
		if (menu_key == ENTER) {
			*selected_item_index = *page * 6;
			*inventory_focus_level = FOCUS_LEVEL_ITEM_LIST; // 포커스를 아이템 리스트로 이동
		}
		else if (menu_key == ESC) {
			*inventory_focus_level = FOCUS_LEVEL_TOP; // 포커스를 상단 카테고리로 이동
		}
		else if (menu_key == UP) {
			switch (*ui_inventory_sub_title_state) {
			case INVENTORY_SUB_TITLE_FOREST: *ui_inventory_sub_title_state = INVENTORY_SUB_TITLE_SNOW; break;
			case INVENTORY_SUB_TITLE_DESERT: *ui_inventory_sub_title_state = INVENTORY_SUB_TITLE_FOREST; break;
			case INVENTORY_SUB_TITLE_SNOW: *ui_inventory_sub_title_state = INVENTORY_SUB_TITLE_DESERT; break;
			}
		}
		else if (menu_key == DOWN) {
			switch (*ui_inventory_sub_title_state) {
			case INVENTORY_SUB_TITLE_FOREST: *ui_inventory_sub_title_state = INVENTORY_SUB_TITLE_DESERT; break;
			case INVENTORY_SUB_TITLE_DESERT: *ui_inventory_sub_title_state = INVENTORY_SUB_TITLE_SNOW; break;
			case INVENTORY_SUB_TITLE_SNOW: *ui_inventory_sub_title_state = INVENTORY_SUB_TITLE_FOREST; break;
			}
		}
	}
	else if (*inventory_focus_level == FOCUS_LEVEL_ITEM_LIST)
	{
		if (*ui_inventory_state == INVENTORY_STATE_WEAPON || *ui_inventory_state == INVENTORY_STATE_ARMOR) {
			int total_items = (*ui_inventory_sub_title_state * 24) + 24; // 각 지역당 24개 아이템
			int page_start = (*page) * ITEMS_PER_PAGE;
			int page_end = page_start + ITEMS_PER_PAGE - 1;
			if (page_end >= total_items) page_end = total_items - 1;
			if (menu_key == ENTER) {
				// TODO: use weapon/armor item
				//new_equipment_index = 
			}
			else if (menu_key == ESC) {
				*inventory_focus_level = FOCUS_LEVEL_SUB;
			}
			else if (menu_key == UP) {
				if (*selected_item_index > 0) {
					(*selected_item_index)--;
					if (*selected_item_index < page_start) {
						(*page)--;
						*selected_item_index = (*page) * ITEMS_PER_PAGE + (ITEMS_PER_PAGE - 1);
						if (*selected_item_index >= total_items)
							*selected_item_index = total_items - 1;
					}
				}
			}
			else if (menu_key == DOWN) {
				if (*selected_item_index < total_items - 1) {
					(*selected_item_index)++;
					if (*selected_item_index > page_end) {
						(*page)++;
						*selected_item_index = (*page) * ITEMS_PER_PAGE;
						if (*selected_item_index >= total_items)
							*selected_item_index = total_items - 1;
					}
				}
			}
			else if (menu_key == LEFT) {
				int first_col_top = (*page) * ITEMS_PER_PAGE;
				int second_col_top = first_col_top + 3;
				if (*selected_item_index < second_col_top) {
					if (*page > 0) {
						int row = *selected_item_index - first_col_top;
						*page -= 1;
						*selected_item_index = (*page) * 6 + 3 + row;
					}
				}
				else {
					*selected_item_index -= 3;
				}
			}
			else if (menu_key == RIGHT) {
				int first_col_top = (*page) * 6;
				int second_col_top = first_col_top + 3;
				if (*selected_item_index >= second_col_top) {
					if (*page < 3) {
						int row = *selected_item_index - second_col_top;
						*page += 1;
						*selected_item_index = (*page) * 6 + row;
					}
				}
				else {
					*selected_item_index += 3;
				}
			}
		}
		else if (*ui_inventory_state == INVENTORY_STATE_HEAL_ITEM) {
			int item_count = HEAL_ITEM_COUNT;
			if (menu_key == ENTER) {
				// TODO: use heal item
			}
			else if (menu_key == ESC) {
				*inventory_focus_level = FOCUS_LEVEL_TOP;
			}
			else if (menu_key == UP) {
				if (*selected_item_index % 3 > 0)
					(*selected_item_index)--;
			}
			else if (menu_key == DOWN) {
				if ((*selected_item_index % 3) < 2 && *selected_item_index + 1 < item_count)
					(*selected_item_index)++;
			}
			else if (menu_key == LEFT) {
				if (*selected_item_index >= 3)
					*selected_item_index -= 3;
			}
			else if (menu_key == RIGHT) {
				if (*selected_item_index + 3 < item_count)
					*selected_item_index += 3;
			}
		}
	}
}

void UI_control_store(int* ui_main_state, int* ui_store_state, int* ui_store_sub_title_state, int* store_focus_level, int* selected_item_index, int menu_key, int* weapon_page, int* armor_page, int* store_buy_sell_state)
{
	int* page = NULL; // 페이지 변수

	if (*ui_store_state == STORE_STATE_WEAPON)
	{
		page = weapon_page;
	}
	else if (*ui_store_state == STORE_STATE_ARMOR)
	{
		page = armor_page;
	}

	if (*store_focus_level == FOCUS_LEVEL_TOP) // 포커스가 상단 카테고리에 있을 때
	{
		if (menu_key == ENTER) {
			if (*ui_store_state == STORE_STATE_BACK) {
				*ui_main_state = UI_STATE_BATTLE; // 전투 상태로 돌아가기
			}
			else if (*ui_store_state == STORE_STATE_WEAPON || *ui_store_state == STORE_STATE_ARMOR) {
				*store_focus_level = FOCUS_LEVEL_SUB; // 포커스를 서브 메뉴로 이동
			}
			else if (*ui_store_state == STORE_STATE_HEAL_ITEM) {
				*store_focus_level = FOCUS_LEVEL_ITEM_LIST; // 포커스를 아이템 리스트로 이동
				*selected_item_index = 0; // 아이템 선택은 첫 번째부터
			}
		}
		else if (menu_key == LEFT) {
			switch (*ui_store_state) {
			case STORE_STATE_BACK: *ui_store_state = STORE_STATE_HEAL_ITEM; break;
			case STORE_STATE_WEAPON: *ui_store_state = STORE_STATE_BACK; break;
			case STORE_STATE_ARMOR: *ui_store_state = STORE_STATE_WEAPON; break;
			case STORE_STATE_HEAL_ITEM: *ui_store_state = STORE_STATE_ARMOR; break;
			}
		}
		else if (menu_key == RIGHT) {
			switch (*ui_store_state) {
			case STORE_STATE_BACK: *ui_store_state = STORE_STATE_WEAPON; break;
			case STORE_STATE_WEAPON: *ui_store_state = STORE_STATE_ARMOR; break;
			case STORE_STATE_ARMOR: *ui_store_state = STORE_STATE_HEAL_ITEM; break;
			case STORE_STATE_HEAL_ITEM: *ui_store_state = STORE_STATE_BACK; break;
			}
		}
	}
	else if (*store_focus_level == FOCUS_LEVEL_SUB) // 포커스가 서브 메뉴에 있을 때
	{
		if (menu_key == ENTER) {
			*selected_item_index = *page * 6;
			*store_focus_level = FOCUS_LEVEL_ITEM_LIST; // 포커스를 아이템 리스트로 이동
		}
		else if (menu_key == ESC) {
			*store_focus_level = FOCUS_LEVEL_TOP; // 포커스를 상단 카테고리로 이동
		}
		else if (menu_key == UP) {
			switch (*ui_store_sub_title_state) {
			case STORE_SUB_TITLE_FOREST: *ui_store_sub_title_state = STORE_SUB_TITLE_SNOW; break;
			case STORE_SUB_TITLE_DESERT: *ui_store_sub_title_state = STORE_SUB_TITLE_FOREST; break;
			case STORE_SUB_TITLE_SNOW: *ui_store_sub_title_state = STORE_SUB_TITLE_DESERT; break;
			}
		}
		else if (menu_key == DOWN) {
			switch (*ui_store_sub_title_state) {
			case STORE_SUB_TITLE_FOREST: *ui_store_sub_title_state = INVENTORY_SUB_TITLE_DESERT; break;
			case INVENTORY_SUB_TITLE_DESERT: *ui_store_sub_title_state = INVENTORY_SUB_TITLE_SNOW; break;
			case INVENTORY_SUB_TITLE_SNOW: *ui_store_sub_title_state = STORE_SUB_TITLE_FOREST; break;
			}
		}
	}
	else if (*store_focus_level == FOCUS_LEVEL_ITEM_LIST)
	{
		if (*ui_store_state == STORE_STATE_WEAPON || *ui_store_state == STORE_STATE_ARMOR) {
			int total_items = (*ui_store_sub_title_state * 24) + 24; // 각 지역당 24개 아이템
			int page_start = (*page) * ITEMS_PER_PAGE;
			int page_end = page_start + ITEMS_PER_PAGE - 1;
			if (page_end >= total_items) page_end = total_items - 1;
			if (menu_key == ENTER) {
				// todo: 포커스 레벨이 buy/sell 상태로 변경
				*store_focus_level = FOCUS_LEVEL_ITEM_BUY_SELL;
			}
			else if (menu_key == ESC) {
				*store_focus_level = FOCUS_LEVEL_SUB;
			}
			else if (menu_key == UP) {
				if (*selected_item_index > 0) {
					(*selected_item_index)--;
					if (*selected_item_index < page_start) {
						(*page)--;
						*selected_item_index = (*page) * ITEMS_PER_PAGE + (ITEMS_PER_PAGE - 1);
						if (*selected_item_index >= total_items)
							*selected_item_index = total_items - 1;
					}
				}
			}
			else if (menu_key == DOWN) {
				if (*selected_item_index < total_items - 1) {
					(*selected_item_index)++;
					if (*selected_item_index > page_end) {
						(*page)++;
						*selected_item_index = (*page) * ITEMS_PER_PAGE;
						if (*selected_item_index >= total_items)
							*selected_item_index = total_items - 1;
					}
				}
			}
			else if (menu_key == LEFT) {
				int first_col_top = (*page) * ITEMS_PER_PAGE;
				int second_col_top = first_col_top + 3;
				if (*selected_item_index < second_col_top) {
					if (*page > 0) {
						int row = *selected_item_index - first_col_top;
						*page -= 1;
						*selected_item_index = (*page) * 6 + 3 + row;
					}
				}
				else {
					*selected_item_index -= 3;
				}
			}
			else if (menu_key == RIGHT) {
				int first_col_top = (*page) * 6;
				int second_col_top = first_col_top + 3;
				if (*selected_item_index >= second_col_top) {
					if (*page < 3) {
						int row = *selected_item_index - second_col_top;
						*page += 1;
						*selected_item_index = (*page) * 6 + row;
					}
				}
				else {
					*selected_item_index += 3;
				}
			}
		}
		else if (*ui_store_state == STORE_STATE_HEAL_ITEM) {
			int item_count = HEAL_ITEM_COUNT;
			if (menu_key == ENTER) {
				// TODO: use heal item
			}
			else if (menu_key == ESC) {
				*store_focus_level = FOCUS_LEVEL_TOP;
			}
			else if (menu_key == UP) {
				if (*selected_item_index % 3 > 0)
					(*selected_item_index)--;
			}
			else if (menu_key == DOWN) {
				if ((*selected_item_index % 3) < 2 && *selected_item_index + 1 < item_count)
					(*selected_item_index)++;
			}
			else if (menu_key == LEFT) {
				if (*selected_item_index >= 3)
					*selected_item_index -= 3;
			}
			else if (menu_key == RIGHT) {
				if (*selected_item_index + 3 < item_count)
					*selected_item_index += 3;
			}
		}
	}
	else if (*store_focus_level == FOCUS_LEVEL_ITEM_BUY_SELL) {
		if (menu_key == ESC) {
			*store_focus_level = FOCUS_LEVEL_ITEM_LIST; // 아이템 리스트로 돌아가기
		}
		else if (menu_key == RIGHT) {
			switch (*store_buy_sell_state) {
			case STORE_STATE_BUY: *store_buy_sell_state = STORE_STATE_SELL; break;
			case STORE_STATE_SELL: *store_buy_sell_state = STORE_STATE_BUY; break;
			}
		}
		else if (menu_key == LEFT) {
			switch (*store_buy_sell_state) {
			case STORE_STATE_BUY: *store_buy_sell_state = STORE_STATE_SELL; break;
			case STORE_STATE_SELL: *store_buy_sell_state = STORE_STATE_BUY; break;
			}
		}
	}
}