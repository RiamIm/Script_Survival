// UI_static.h
#pragma once
#include "inout.h"
#include "utils.h"

#include "item.h"
#include "player.h"
#include "monster.h"

#include "UI_info.h"

void UI_static_main_box(int color);
void UI_static_title(void);
void UI_static_setting_menu(void);
void UI_static_select_game_mode(void);

void UI_static_battle_box(void);

void UI_static_inventory_box(void);
void UI_static_shop_box(void);