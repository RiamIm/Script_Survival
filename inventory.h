// inventory.h
#pragma once
#include "inout.h"
#include "utils.h"

#include "item.h"

typedef struct player player_t;

#include "UI_info.h"

typedef struct pair {
	int is_was_having;
	int count;
} pair_t;


extern pair_t weapon_inventory[RARITY_COUNT][ITEM_COUNT];
extern pair_t armor_inventory[RARITY_COUNT][ITEM_COUNT];

extern int heal_item_inventory[];

void inventory_init(void);
void inventory_unlock_all_items(void); 
void get_item(equipment_rarity_t rarity, int item_index, int type);
void sell_item(equipment_rarity_t rarity, int item_index, int type);

//void inventory_get_all_items_for_test();
//void inventory_get_all_heal_items_for_test();

// --- Getter ---
inventory_state_t	get_inventory_state(void);
focus_level_t		get_inventory_focus_level(void);
equipment_rarity_t	get_inventory_rarity_type(void);
int					get_inventory_selected_index(void);
int					get_inventory_weapon_page(void);
int					get_inventory_armor_page(void);

// --- Setter ---
void set_inventory_state(inventory_state_t new_state);
void set_inventory_focus_level(focus_level_t new_focus);
void set_inventory_rarity_type(equipment_rarity_t new_rarity);
void set_inventory_selected_index(int new_index);
void set_inventory_weapon_page(int new_page);
void set_inventory_armor_page(int new_page);

