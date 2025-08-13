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

	int    _attack;
	int    _max_hp;
	int    _current_hp;
	int    _speed;
	int    _break_damage;
	double _evasion_rate;
	double _defense_rate;
	double _crit_chance;          // 치명타 확률
	double _crit_damage;          // 치명타 피해 배율 (기본 1.5 = 150%)
	int    _break_extra_damage;   // 격파 시 추가 피해량
	int    _stun_duration;        // 스턴 시간 (기본 1턴)

	// 기본 베이스 스탯 계산 (장비 스탯 제외)
	int    _base_attack;
	int	   _base_max_hp;
	int    _base_speed;
	double _base_evasion_rate;
	double _base_defense_rate;
	double _base_crit_chance;
	double _base_crit_damage;
	int    _base_break_damage;
	int    _base_break_extra_damage;
	int    _base_stun_duration;

	double action_value;

	double self_damage;
	double life_steal;
	double damage_increase;      // 가하는 피해 증가 (기본 0.0 = 0%)

	bool is_counter;

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
	double evasion_to_defense;
	double defense_from_evasion;

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

// getter
int    get_player_attack(player_t* player);
int    get_player_hp(player_t* player);
int    get_player_max_hp(player_t* player);
int	   get_player_speed(player_t* player);
double get_player_evasion_rate(player_t* player);
double get_player_defense_rate(player_t* player);
double get_player_crit_chance(player_t* player);
double get_player_crit_damage(player_t* player);
int    get_player_break_damage(player_t* player);
int    get_player_break_extra_damage(player_t* player);
int    get_player_stun_duration(player_t* player);

// Setters (type: 0 = 영구 적용, 1 = 일시 적용)
void set_player_attack(player_t* player, int value, int type);
void set_player_hp(player_t* player, int value); // 현재 HP는 type 인자 제외
void set_player_max_hp(player_t* player, int value, int type);
void set_player_speed(player_t* player, int value, int type);
void set_player_evasion_rate(player_t* player, double value, int type);
void set_player_defense_rate(player_t* player, double value, int type);
void set_player_crit_chance(player_t* player, double value, int type);
void set_player_crit_damage(player_t* player, double value, int type);
void set_player_break_damage(player_t* player, int value, int type);
void set_player_break_extra_damage(player_t* player, int value, int type);
void set_player_stun_duration(player_t* player, int value, int type);

void player_save_legacy_data(player_t* player);

bool player_load_legacy_data(player_t* player);