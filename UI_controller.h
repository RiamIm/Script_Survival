#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <conio.h>
#include <stdbool.h>
#include "utils.h"
#include "new_ui.h"

typedef enum UI_state {
	UI_STATE_TITLE,
	UI_STATE_SETTING,
	UI_STATE_BATTLE,
	UI_STATE_INVENTORY,
	UI_STATE_STORE
} UI_state_t;

typedef enum title_state {
	TITLE_STATE_START,
	TITLE_STATE_OPTIONS,
	TITLE_STATE_EXIT
} title_state_t;

typedef enum setting_state {
	
} setting_state_t;

typedef enum battle_state {
	BATTLE_STATE_ATTACK,
	BATTLE_STATE_INVENTORY,
	BATTLE_STATE_EXTORTION
} battle_state_t;

typedef enum inventory_state {
	INVENTORY_STATE_WEAPON,
	INVENTORY_STATE_ARMOR,
	INVENTORY_STATE_HEAL_ITEM
} inventory_state_t;

typedef enum store_state {

} store_state_t;

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ENTER 13

#define WIDTH 151
#define HEIGHT 27

// draw title_ui도 함께 실행

static bool s_confirm_player_name(const char* name);
char* draw_create_player_name_ui(void);
UI_state_t title_control(int dir);
UI_state_t setting_control(menu_key);
battle_state_t battle_control(int current_stage, player_t* player, monster_t* monster, int key);
UI_state_t inventory_control(menu_key);
UI_state_t store_control(menu_key);