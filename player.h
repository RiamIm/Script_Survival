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

typedef struct legacy_data {
	int weapon_index;
	int armor_index;
	equipment_rarity_t weapon_rarity;
	equipment_rarity_t armor_rarity;
	int coin;
} legacy_data_t;

typedef struct player {
	hero_t choice_hero;
	char name[8];
	int attack;
	int max_hp;
	int current_hp;
	int speed;
	int break_damage;

	double action_value;

	double self_damage;
	double life_steal;

	bool is_counter;
	
	double evasion_rate;
	double defence_rate;
	
	double crit_chance;          // 치명타 확률
	double crit_damage;          // 치명타 피해 배율 (기본 1.5 = 150%)
	double damage_increase;      // 가하는 피해 증가 (기본 0.0 = 0%)
	int break_extra_damage;      // 격파 시 추가 피해량
	int stun_duration;           // 스턴 시간 (기본 1턴)

	double auto_heal;

	int coin;

	int weapon_index;
	int armor_index;

	equipment_rarity_t weapon_rarity;
	equipment_rarity_t armor_rarity;

	bool run;

	int set_effect_id;
	

	// 0 번
	bool dead_effect_used;
	int dead_count;

	// 1번 세트 효과
	double crit_bonus;
	double set_effect_crit_bonus;

	// 2번 세트 효과
	double defence_penetration;

	// 3번 세트 효과
	double evasion_to_defence;
	double defence_from_evasion;

	// 4번 세트 효과
	int speed_bonus;
	int set_effect_speed_bonus;	

	// 5번 세트 효과
	bool damage_reduction_mode;

} player_t;

#define RAGE_COEFF 2.0
#define RAGE_MAX_MULTIPLIER 3.0

void player_init(player_t* player, char* name, hero_t choice_hero);
void player_damage_increase(player_t* player);

void player_save_legacy_data(player_t* player);

bool player_load_legacy_data(player_t* player);
