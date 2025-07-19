// store.h 
#pragma once
#include "UI_info.h"

// --- Getter ---
store_state_t			get_store_state();
focus_level_t			get_store_focus_level();
region_t				get_store_region();
store_state_t			get_store_buy_sell_state();
store_buy_sell_state_t	get_store_buy_sell_successful_state();
int						get_store_selected_index();
int						get_store_weapon_page();
int						get_store_armor_page();

// --- Setter ---
void set_store_state(store_state_t new_state);
void set_store_focus_level(focus_level_t new_focus);
void set_store_region(region_t new_region);
void set_store_buy_sell_state(store_state_t new_state);
void set_store_buy_sell_successful_state(store_buy_sell_state_t new_state);
void set_store_selected_index(int new_index);
void set_store_weapon_page(int new_page);
void set_store_armor_page(int new_page);

void store_init();