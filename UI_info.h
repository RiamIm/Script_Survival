// UI_info.h
#pragma once

typedef enum {
	UI_STATE_TITLE,
	UI_STATE_CREATE_PLAYER_NAME,
	UI_STATE_SETTING,
	UI_STATE_BATTLE,
	UI_STATE_INVENTORY,
	UI_STATE_STORE
} UI_state_t;

typedef enum {
	TITLE_STATE_START,
	TITLE_STATE_OPTIONS,
	TITLE_STATE_EXIT
} title_state_t;

typedef enum {

} setting_state_t;

// ====================

typedef enum {
	BATTLE_STATE_ATTACK,
	BATTLE_STATE_EXTORTION,
	BATTLE_STATE_INVENTORY
} battle_state_t;

typedef enum {
	BATTLE_ACTION_NONE,   
	BATTLE_ACTION_ATTACK,
	BATTLE_ACTION_EXTORTION,
	BATTLE_ACTION_INVENTORY
} battle_action_t;

// ====================

typedef enum {
	FOCUS_LEVEL_TOP,
	FOCUS_LEVEL_SUB,
	FOCUS_LEVEL_ITEM_LIST,
	FOCUS_LEVEL_ITEM_BUY_SELL
} focus_level_t;

// ====================

typedef enum {
	INVENTORY_STATE_BACK,
	INVENTORY_STATE_WEAPON,
	INVENTORY_STATE_ARMOR,
	INVENTORY_STATE_HEAL_ITEM,
	INVENTORY_STATE_OPTIONS
} inventory_state_t;

typedef enum {
	INVENTORY_SUB_TITLE_FOREST,
	INVENTORY_SUB_TITLE_DESERT,
	INVENTORY_SUB_TITLE_SNOW
} inventory_sub_title_t;

// ====================

typedef enum {
	STORE_STATE_BACK,
	STORE_STATE_WEAPON,
	STORE_STATE_ARMOR,
	STORE_STATE_HEAL_ITEM,
	STORE_STATE_BUY,
	STORE_STATE_SELL
} store_state_t;

typedef enum {
	STORE_SUB_TITLE_FOREST,
	STORE_SUB_TITLE_DESERT,
	STORE_SUB_TITLE_SNOW
} store_sub_title_t;

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ENTER 13

#define WIDTH 151
#define HEIGHT 28

#define EXTENDED_KEY 224

#define ITEMS_PER_PAGE 6