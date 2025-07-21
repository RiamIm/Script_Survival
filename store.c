// store.c 
#define _CRT_SECURE_NO_WARNINGS
#include "store.h"

// ==================== 상태 변수 캡슐화 ====================
static store_state_t          s_store_state;
static focus_level_t          s_focus_level;
static equipment_rarity_t     s_rarity;
static store_state_t          s_buy_sell_state;
static store_buy_sell_state_t s_store_buy_sell_state;
static int                    s_selected_item_index;
static int                    s_weapon_page;
static int                    s_armor_page;

// --- Getter ---
store_state_t           get_store_state() { return s_store_state; }
focus_level_t           get_store_focus_level() { return s_focus_level; }
equipment_rarity_t      get_store_rarity_type() { return s_rarity; }
store_state_t           get_store_buy_sell_state() { return s_buy_sell_state; }
store_buy_sell_state_t  get_store_buy_sell_successful_state() { return s_store_buy_sell_state; }
int                     get_store_selected_index() { return s_selected_item_index; }
int                     get_store_weapon_page() { return s_weapon_page; }
int                     get_store_armor_page() { return s_armor_page; }

// --- Setter ---
void set_store_state(store_state_t new_state) { s_store_state = new_state; }
void set_store_focus_level(focus_level_t new_focus) { s_focus_level = new_focus; }
void set_store_rarity_type(equipment_rarity_t new_rarity) { s_rarity = new_rarity; }
void set_store_buy_sell_state(store_state_t new_state) { s_buy_sell_state = new_state; }
void set_store_buy_sell_successful_state(store_buy_sell_state_t new_state) { s_store_buy_sell_state = new_state; }
void set_store_selected_index(int new_index) { s_selected_item_index = new_index; }
void set_store_weapon_page(int new_page) { s_weapon_page = new_page; }
void set_store_armor_page(int new_page) { s_armor_page = new_page; }

// --- 상점 상태 초기화 함수 ---
void store_init() {
    s_store_state = STORE_STATE_WEAPON;
    s_focus_level = FOCUS_LEVEL_TOP;
    s_rarity = RARITY_NORMAL;
    s_buy_sell_state = STORE_STATE_BUY;
	s_store_buy_sell_state = STORE_BUY_SELL_NONE;
    s_selected_item_index = 0;
    s_weapon_page = 0;
    s_armor_page = 0;
}