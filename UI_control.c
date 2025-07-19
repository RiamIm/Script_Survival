// UI_control.c
#define _CRT_SECURE_NO_WARNINGS
#include "UI_control.h"

void UI_control_init(UI_state_t* ui_main_state, title_state_t* ui_title_state, player_action_t* player_action_state)
{
	*ui_main_state = UI_STATE_TITLE;
	*ui_title_state = TITLE_STATE_START;
	*player_action_state = PLAYER_ACTION_ATTACK;
	// inventory, store 초기화 삭제
}

void UI_control_title(UI_state_t* ui_main_state, title_state_t* ui_title_state, int menu_key)
{
	if (menu_key == ENTER) {
		if (*ui_title_state == TITLE_STATE_START) *ui_main_state = UI_STATE_SELECT_GAME_MODE;
		else if (*ui_title_state == TITLE_STATE_OPTIONS) *ui_main_state = UI_STATE_SETTING;
		else if (*ui_title_state == TITLE_STATE_EXIT) exit(0);
	}
	else if (menu_key == UP) {
		*ui_title_state = (*ui_title_state - 1 + 3) % 3;
	}
	else if (menu_key == DOWN) {
		*ui_title_state = (*ui_title_state + 1) % 3;
	}
}

void UI_control_setting(UI_state_t* ui_main_state, setting_state_t* ui_setting_state, int* global_volume, int key)
{
	switch (key) {
	case UP:
		*ui_setting_state = (*ui_setting_state - 1 + SETTING_STATE_MAX) % SETTING_STATE_MAX;
		break;

	case DOWN:
		*ui_setting_state = (*ui_setting_state + 1) % SETTING_STATE_MAX;
		break;

	case LEFT:
		if (*ui_setting_state == SETTING_STATE_VOLUME && *global_volume > 0)
			*global_volume -= 5;
		break;

	case RIGHT:
		if (*ui_setting_state == SETTING_STATE_VOLUME && *global_volume < 100)
			*global_volume += 5;
		break;

	case ENTER:
		if (*ui_setting_state == SETTING_STATE_SOUND_ON) {
			// 예: 사운드 엔진에 알림
			// sound_set_enabled(true);
		}
		else if (*ui_setting_state == SETTING_STATE_SOUND_OFF) {
			// sound_set_enabled(false);
		}
		else if (*ui_setting_state == SETTING_STATE_BACK) {
			*ui_main_state = UI_STATE_TITLE;
		}
		break;
	}
}

// 모드 선택 화면 ↑↓ + 엔터 처리
void UI_control_game_mode(UI_state_t* ui_main_state, game_mode_state_t* ui_mode_state, int* game_mode, int key)
{
	if (key == ENTER) {
		if (*ui_mode_state == MODE_STATE_NORMAL)
			*game_mode = GAME_MODE_NORMAL;
		else if (*ui_mode_state == MODE_STATE_INFINITY)
			*game_mode = GAME_MODE_INFINITY;

		*ui_main_state = UI_STATE_CREATE_PLAYER_NAME;
	}
	else if (key == UP) {
		*ui_mode_state = (*ui_mode_state - 1 + MODE_STATE_MAX) % MODE_STATE_MAX;
	}
	else if (key == DOWN) {
		*ui_mode_state = (*ui_mode_state + 1) % MODE_STATE_MAX;
	}
}

player_action_t UI_control_player_action(player_action_t* player_action_state, int menu_key)
{
	if (menu_key == ENTER) {
		return *player_action_state; // return player_action_t
	}
	else if (menu_key == UP) {
		*player_action_state = (*player_action_state - 1 + 3) % 3;
	}
	else if (menu_key == DOWN) {
		*player_action_state = (*player_action_state + 1) % 3;
	}
	return PLAYER_ACTION_NONE;
}

// 무기 장착 여부 반환 (0 변경 없음, 1 무기 변경, 2 방어구 변경)
int UI_control_inventory(UI_state_t* ui_main_state, player_t* player, int menu_key)
{
	inventory_state_t current_state = get_inventory_state();
	focus_level_t focus_level = get_inventory_focus_level();
	region_t current_region = get_inventory_region();
	int selected_index = get_inventory_selected_index();
	int weapon_page = get_inventory_weapon_page();
	int armor_page = get_inventory_armor_page();
	int* page = (current_state == INVENTORY_STATE_WEAPON) ? &weapon_page : &armor_page;

	if (focus_level == FOCUS_LEVEL_TOP)
	{
		if (menu_key == ENTER) {
			if (current_state == INVENTORY_STATE_BACK) {
				*ui_main_state = UI_STATE_BATTLE;
			}
			else if (current_state == INVENTORY_STATE_WEAPON || current_state == INVENTORY_STATE_ARMOR) {
				focus_level = FOCUS_LEVEL_SUB;
			}
			else if (current_state == INVENTORY_STATE_HEAL_ITEM) {
				focus_level = FOCUS_LEVEL_ITEM_LIST;
				selected_index = 0;
			}
		}
		else if (menu_key == LEFT) {
			current_state = (current_state - 1 + 5) % 5;
		}
		else if (menu_key == RIGHT) {
			current_state = (current_state + 1) % 5;
		}
	}
	else if (focus_level == FOCUS_LEVEL_SUB)
	{
		if (menu_key == ENTER) {
			if (page != NULL) *page = 0;
			selected_index = current_region * ITEMS_PER_REGION;
			focus_level = FOCUS_LEVEL_ITEM_LIST;
		}
		else if (menu_key == ESC) {
			focus_level = FOCUS_LEVEL_TOP;
		}
		else if (menu_key == UP) {
			current_region = (current_region - 1 + REGION_COUNT) % REGION_COUNT;
		}
		else if (menu_key == DOWN) {
			current_region = (current_region + 1) % REGION_COUNT;
		}
	}
	else if (focus_level == FOCUS_LEVEL_ITEM_LIST)
	{
		if (menu_key == ENTER) {
			if ((current_state == INVENTORY_STATE_WEAPON) && (selected_index != player->weapon_index)) {
				use_weapon(selected_index, player);
				return 1;
			}
			else if ((current_state == INVENTORY_STATE_ARMOR) && (selected_index != player->armor_index)) {
				use_armor(selected_index, player);
				return 2;
			}
			else if (current_state == INVENTORY_STATE_HEAL_ITEM) {
				if (use_heal_item(selected_index, player)) {
					return 3;
				}
				// 아이템이 없어서 사용에 실패하면 아무것도 하지 않음
			}
			return 0;
		}

		if ((current_state == INVENTORY_STATE_WEAPON || current_state == INVENTORY_STATE_ARMOR) && page != NULL) {
			int region_start_index = current_region * ITEMS_PER_REGION;
			int region_end_index = region_start_index + ITEMS_PER_REGION - 1;

			if (menu_key == ESC) {
				focus_level = FOCUS_LEVEL_SUB;
			}
			else if (menu_key == UP) {
				if (selected_index > region_start_index) {
					(selected_index)--;
					*page = (selected_index - region_start_index) / ITEMS_PER_PAGE;
				}
			}
			else if (menu_key == DOWN) {
				if (selected_index < region_end_index) {
					(selected_index)++;
					*page = (selected_index - region_start_index) / ITEMS_PER_PAGE;
				}
			}
			else if (menu_key == LEFT) {
				if (selected_index - ITEMS_PER_ROW >= region_start_index) {
					selected_index -= ITEMS_PER_ROW;
					*page = (selected_index - region_start_index) / ITEMS_PER_PAGE;
				}
			}
			else if (menu_key == RIGHT) {
				if (selected_index + ITEMS_PER_ROW <= region_end_index) {
					selected_index += ITEMS_PER_ROW;
					*page = (selected_index - region_start_index) / ITEMS_PER_PAGE;
				}
			}
		}
		else if (current_state == INVENTORY_STATE_HEAL_ITEM) {
			if (menu_key == ESC) {
				focus_level = FOCUS_LEVEL_TOP;
			}
			else if (menu_key == UP) {
				if (selected_index > 0) {
					selected_index--;
				}
			}
			else if (menu_key == DOWN) {
				if (selected_index < HEAL_ITEM_COUNT - 1) {
					selected_index++;
				}
			}
			else if (menu_key == LEFT) {
				if (selected_index - ITEMS_PER_ROW >= 0) {
					selected_index -= ITEMS_PER_ROW;
				}
			}
			else if (menu_key == RIGHT) {
				if (selected_index + ITEMS_PER_ROW < HEAL_ITEM_COUNT) {
					selected_index += ITEMS_PER_ROW;
				}
			}
		}
	}

	set_inventory_state(current_state);
	set_inventory_focus_level(focus_level);
	set_inventory_region(current_region);
	set_inventory_selected_index(selected_index);
	set_inventory_weapon_page(weapon_page);
	set_inventory_armor_page(armor_page);

	return 0;
}

void UI_control_store(UI_state_t* ui_main_state, player_t* player, int menu_key)
{
	store_state_t current_state = get_store_state();
	focus_level_t focus_level = get_store_focus_level();
	region_t current_region = get_store_region();
	store_state_t buy_sell_state = get_store_buy_sell_state();
	int selected_index = get_store_selected_index();
	int weapon_page = get_store_weapon_page();
	int armor_page = get_store_armor_page();
	int* page = (current_state == STORE_STATE_WEAPON) ? &weapon_page : &armor_page;

	if (focus_level == FOCUS_LEVEL_TOP)
	{
		if (menu_key == ENTER) {
			if (current_state == STORE_STATE_BACK) *ui_main_state = UI_STATE_BATTLE;
			else if (current_state == STORE_STATE_WEAPON || current_state == STORE_STATE_ARMOR) focus_level = FOCUS_LEVEL_SUB;
			else if (current_state == STORE_STATE_HEAL_ITEM) {
				focus_level = FOCUS_LEVEL_ITEM_LIST;
				selected_index = 0;
			}
		}
		else if (menu_key == LEFT) {
			current_state = (current_state - 1 + 4) % 4;
		}
		else if (menu_key == RIGHT) {
			current_state = (current_state + 1) % 4;
		}
	}
	else if (focus_level == FOCUS_LEVEL_SUB)
	{
		if (menu_key == ENTER) {
			if (page != NULL) *page = 0;
			selected_index = current_region * ITEMS_PER_REGION;
			focus_level = FOCUS_LEVEL_ITEM_LIST;
		}
		else if (menu_key == ESC) {
			focus_level = FOCUS_LEVEL_TOP;
		}
		else if (menu_key == UP) {
			current_region = (current_region - 1 + REGION_COUNT) % REGION_COUNT;
		}
		else if (menu_key == DOWN) {
			current_region = (current_region + 1) % REGION_COUNT;
		}
	}
	else if (focus_level == FOCUS_LEVEL_ITEM_LIST)
	{
		if ((current_state == STORE_STATE_WEAPON || current_state == STORE_STATE_ARMOR) && page != NULL) {
			int region_start_index = current_region * ITEMS_PER_REGION;
			int region_end_index = region_start_index + ITEMS_PER_REGION - 1;

			if (menu_key == ENTER) {
				focus_level = FOCUS_LEVEL_ITEM_BUY_SELL;
			}
			else if (menu_key == ESC) {
				focus_level = FOCUS_LEVEL_SUB;
			}
			else if (menu_key == UP) {
				if (selected_index > region_start_index) {
					(selected_index)--;
					*page = (selected_index - region_start_index) / ITEMS_PER_PAGE;
				}
			}
			else if (menu_key == DOWN) {
				if (selected_index < region_end_index) {
					(selected_index)++;
					*page = (selected_index - region_start_index) / ITEMS_PER_PAGE;
				}
			}
			else if (menu_key == LEFT) {
				if (selected_index - ITEMS_PER_ROW >= region_start_index) {
					selected_index -= ITEMS_PER_ROW;
					*page = (selected_index - region_start_index) / ITEMS_PER_PAGE;
				}
			}
			else if (menu_key == RIGHT) {
				if (selected_index + ITEMS_PER_ROW <= region_end_index) {
					selected_index += ITEMS_PER_ROW;
					*page = (selected_index - region_start_index) / ITEMS_PER_PAGE;
				}
			}
		}
		else if (current_state == STORE_STATE_HEAL_ITEM) {
			if (menu_key == ENTER) {
				focus_level = FOCUS_LEVEL_ITEM_BUY_SELL;
			}
			else if (menu_key == ESC) {
				focus_level = FOCUS_LEVEL_TOP;
			}
			else if (menu_key == UP) { if (selected_index > 0) selected_index--; }
			else if (menu_key == DOWN) { if (selected_index < HEAL_ITEM_COUNT - 1) selected_index++; }
			else if (menu_key == LEFT) { if (selected_index - ITEMS_PER_ROW >= 0) selected_index -= ITEMS_PER_ROW; }
			else if (menu_key == RIGHT) { if (selected_index + ITEMS_PER_ROW < HEAL_ITEM_COUNT) selected_index += ITEMS_PER_ROW; }
		}
	}
	else if (focus_level == FOCUS_LEVEL_ITEM_BUY_SELL)
	{
		if (menu_key == ESC) {
			focus_level = FOCUS_LEVEL_ITEM_LIST;
		}
		else if (menu_key == LEFT || menu_key == RIGHT) {
			buy_sell_state = (buy_sell_state == STORE_STATE_BUY) ? STORE_STATE_SELL : STORE_STATE_BUY;
		}
		else if (menu_key == ENTER) {
			if (buy_sell_state == STORE_STATE_BUY) {
				if (current_state == STORE_STATE_WEAPON) {
					if (player->coin >= weapons[selected_index].buy_price) {
						player->coin -= weapons[selected_index].buy_price;
						get_item(selected_index, ITEM_TYPE_WEAPON);
					}
				}
				else if (current_state == STORE_STATE_ARMOR) {
					if (player->coin >= armors[selected_index].buy_price) {
						player->coin -= armors[selected_index].buy_price;
						get_item(selected_index, ITEM_TYPE_ARMOR);
					}
				}
				else if (current_state == STORE_STATE_HEAL_ITEM) {
					if (player->coin >= heal_items[selected_index].buy_price) {
						player->coin -= heal_items[selected_index].buy_price;
						get_item(selected_index, ITEM_TYPE_HEAL_ITEM);
					}
				}
			}
			else { // STORE_STATE_SELL
				if (current_state == STORE_STATE_WEAPON) {
					if (weapon_inventory[selected_index].count > 0) {
						player->coin += weapons[selected_index].sell_price;
						sell_item(selected_index, ITEM_TYPE_WEAPON);
					}
				}
				else if (current_state == STORE_STATE_ARMOR) {
					if (armor_inventory[selected_index].count > 0) {
						player->coin += armors[selected_index].sell_price;
						sell_item(selected_index, ITEM_TYPE_ARMOR);
					}
				}
				else if (current_state == STORE_STATE_HEAL_ITEM) {
					if (heal_item_inventory[selected_index] > 0) {
						player->coin += heal_items[selected_index].sell_price;
						sell_item(selected_index, ITEM_TYPE_HEAL_ITEM);
					}
				}
			}
			focus_level = FOCUS_LEVEL_ITEM_LIST;
		}
	}

	set_store_state(current_state);
	set_store_focus_level(focus_level);
	set_store_region(current_region);
	set_store_buy_sell_state(buy_sell_state);
	set_store_selected_index(selected_index);
	set_store_weapon_page(weapon_page);
	set_store_armor_page(armor_page);
}