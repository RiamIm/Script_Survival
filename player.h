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
	hero_t choice_hero;
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
	
	// ============ 요청하신 스탯 추가 ============
	double crit_chance;          // 치명타 확률
	double crit_damage_modifier; // 치명타 피해 배율 (기본 1.5 = 150%)
	double damage_increase;      // 가하는 피해 증가 (기본 0.0 = 0%)
	int break_extra_damage;      // 격파 시 추가 피해량
	int stun_duration;           // 스턴 시간 (기본 1턴)
	// ==========================================

	double auto_heal;

	int coin;

	int weapon_index;
	int armor_index;

	equipment_rarity_t weapon_rarity;
	equipment_rarity_t armor_rarity;

} player_t;

void player_init(player_t* player, char* name, hero_t choice_hero);