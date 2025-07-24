// UI_info.h
#pragma once

// ==================== 아이템 관련 상수 정의 ====================
#define NORMAL_ITEM_COUNT 24
#define RARE_ITEM_COUNT 18
#define EPIC_ITEM_COUNT 12
#define UNIQUE_ITEM_COUNT 6

#define ITEMS_PER_PAGE 6
#define ITEMS_PER_ROW 3
// =============================================================

typedef enum UI_state {
	UI_STATE_TITLE = 0,
	UI_STATE_SELECT_HERO,
	UI_STATE_CREATE_PLAYER_NAME,
	UI_STATE_SELECT_GAME_MODE,
	UI_STATE_SETTING,
	UI_STATE_BATTLE,
	UI_STATE_INVENTORY,
	UI_STATE_STORE,
	UI_STATE_INFINITE_UPGRADE
} UI_state_t;

typedef enum title_state {
	TITLE_STATE_START = 0,
	TITLE_STATE_OPTIONS,
	TITLE_STATE_EXIT
} title_state_t;

typedef enum setting_state {
	SETTING_STATE_SOUND_ON = 0,
	SETTING_STATE_SOUND_OFF,
	SETTING_STATE_VOLUME,
	SETTING_STATE_BACK,
	SETTING_STATE_MAX
} setting_state_t;

typedef enum game_mode_state {
	MODE_STATE_NORMAL = 0,
	MODE_STATE_INFINITY,
	MODE_STATE_MAX
} game_mode_state_t;

typedef enum player_action_state {
	PLAYER_ACTION_ATTACK = 0,
	PLAYER_ACTION_FOCUS,
	PLAYER_ACTION_INVENTORY,
	PLAYER_ACTION_NONE
} player_action_t;

typedef enum focus_level {
	FOCUS_LEVEL_TOP = 0,
	FOCUS_LEVEL_SUB,
	FOCUS_LEVEL_ITEM_LIST,
	FOCUS_LEVEL_ITEM_BUY_SELL
} focus_level_t;

typedef enum inventory_state {
	INVENTORY_STATE_BACK = 0,
	INVENTORY_STATE_WEAPON,
	INVENTORY_STATE_ARMOR,
	INVENTORY_STATE_HEAL_ITEM,
	INVENTORY_STATE_OPTIONS
} inventory_state_t;

typedef enum store_state {
	STORE_STATE_BACK = 0,
	STORE_STATE_WEAPON,
	STORE_STATE_ARMOR,
	STORE_STATE_HEAL_ITEM,
	STORE_STATE_BUY,
	STORE_STATE_SELL
} store_state_t;

typedef enum store_buy_sell_state {
	STORE_BUY_SUCCESS = 0,
	STORE_BUY_FAIL,
	STORE_SELL_SUCCESS,
	STORE_SELL_FAIL,
	STORE_BUY_SELL_NONE
} store_buy_sell_state_t;

typedef enum {
	UPGRADE_NONE = -1,
	UPGRADE_HP,
	UPGRADE_ATK,
	UPGRADE_SPD,
	UPGRADE_BREAK,
	UPGRADE_STUN,
	UPGRADE_CRIT_CHANCE,
	UPGRADE_CRIT_DMG,
	UPGRADE_LIFESTEAL,
	UPGRADE_MAX
} upgrade_type_t;

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ENTER 13

#define WIDTH 151
#define HEIGHT 28

#define ESC 27
#define EXTENDED_KEY 224

#define GAME_MODE_NORMAL   0
#define GAME_MODE_INFINITY 1

extern int game_mode;