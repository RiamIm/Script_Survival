// inventory.h
#pragma once
#include "inout.h"
#include "utils.h"

#include "player.h"

#include "UI_info.h"

#define INVENTORY_SIZE 72
#define HEAL_ITEM_COUNT 6

typedef struct pair {
	int is_was_having;
	int count;
} pair_t;

extern pair_t weapon_inventory[INVENTORY_SIZE];
extern pair_t armor_inventory[INVENTORY_SIZE];
extern int heal_item_inventory[HEAL_ITEM_COUNT];

void inventory_init(void);
void get_item(int item_index, int type);
void sell_item(int item_index, int type);
void inventory_get_all_items_for_test();

// --- Getter ---
inventory_state_t get_inventory_state();
focus_level_t     get_inventory_focus_level();
region_t          get_inventory_region();
int               get_inventory_selected_index();
int               get_inventory_weapon_page();
int               get_inventory_armor_page();

// --- Setter ---
void set_inventory_state(inventory_state_t new_state);
void set_inventory_focus_level(focus_level_t new_focus);
void set_inventory_region(region_t new_region);
void set_inventory_selected_index(int new_index);
void set_inventory_weapon_page(int new_page);
void set_inventory_armor_page(int new_page);