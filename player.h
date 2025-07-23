// player.h
#pragma once
#include "inout.h"
#include "utils.h"
#include "item.h"
#include "inventory.h"

typedef enum hero {
	HERO_BREAKER = 0,
	HERO_COUNTER,
	HERO_BERSERKER
} hero_t;

typedef struct player {
	char name[125];
	int attack;
	int max_hp;
	int current_hp;
	int speed;
	int break_damage;

	bool is_focused;
	double action_value;

	double self_damage;
	double life_steal;
	
	double evasion_rate;
	double defence_rate;
	
	int coin;

	int weapon_index;
	int armor_index;

	equipment_rarity_t weapon_rarity;
	equipment_rarity_t armor_rarity;

} player_t;

void player_init(player_t* player, char* name, hero_t choice_hero);