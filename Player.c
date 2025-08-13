// player.c
#define _CRT_SECURE_NO_WARNINGS
#include "player.h"

#define REGACY_FILE_NAME "data/legacy.dat"

// 초기모델, 나중에 세이브 추가하면 변경 해야 함
void player_init(player_t* player, char* name, hero_t choice_hero)
{
	player->choice_hero = choice_hero;
	strncpy(player->name, name, sizeof(player->name) - 1);
	player->name[sizeof(player->name) - 1] = '\0';

	if (choice_hero == HERO_BREAKER)
	{
		player->_base_max_hp      = player->_max_hp      = 450;
		player->_base_attack      = player->_attack      = 100;
		player->_base_speed		 = player->_speed       = 550;
		player->_base_crit_chance = player->_crit_chance = 0.05;
		player->_base_crit_damage = player->_crit_damage = 1.5;

		player->_base_break_damage = player->_break_damage = 20;
		player->_base_break_extra_damage = player->_break_extra_damage = 300;
		player->_base_stun_duration      = player->_stun_duration      = 2;

		player->self_damage = 0.0;
		player->life_steal = 0.0;
	}
	else if (choice_hero == HERO_COUNTER)
	{
		player->_base_max_hp      = player->_max_hp      = 550;
		player->_base_attack      = player->_attack      = 100;
		player->_base_speed       = player->_speed       = 475;
		player->_base_crit_chance = player->_crit_chance = 0.05;
		player->_base_crit_damage = player->_crit_damage = 1.5;

		player->_base_break_damage = player->_break_damage = 10;
		player->_base_break_extra_damage = player->_break_extra_damage = 0;
		player->_base_stun_duration      = player->_stun_duration      = 1;

		player->self_damage = 0.0;
		player->life_steal = 0.0;
	}
	else if (choice_hero == HERO_BERSERKER)
	{
		player->_base_max_hp      = player->_max_hp      = 700;
		player->_base_attack      = player->_attack      = 80;
		player->_base_speed       = player->_speed       = 500;
		player->_base_crit_chance = player->_crit_chance = 0.05;
		player->_base_crit_damage = player->_crit_damage = 1.5;

		player->_base_break_damage       = player->_break_damage = 15;
		player->_base_break_extra_damage = player->_break_extra_damage = 0;
		player->_base_stun_duration      = player->_stun_duration      = 1;

		player->self_damage = 0.3;
		player->life_steal = 0.1;
	}
	
	// --- 공통 초기화 --- 
	player->_base_evasion_rate = player->_evasion_rate = 0.05;
	player->_current_hp = player->_max_hp;

	player->damage_increase = 1.0;

	// --- 나머지 초기화 코드 ---
	player->is_counter = false;
	player->auto_heal = 0.3;
	player->coin = 10000000;

	player->set_effect_id = SET_EFFECT_INVALID;
	player->dead_count = 0;
	player->dead_effect_used = false;
	player->crit_bonus = 0.25;
	player->set_effect_crit_bonus = 0.0;
	player->defence_penetration = 0.0;
	player->evasion_to_defense = 1.0;
	player->defense_from_evasion = 0.0;
	player->speed_bonus = 100;
	player->set_effect_speed_bonus = 0;
	player->damage_reduction_mode = false;

	player->weapon_index = -1;
	player->armor_index = -1;
	player->weapon_rarity = RARITY_NORMAL;
	player->armor_rarity = RARITY_NORMAL;
	use_weapon(RARITY_NORMAL, 0, player);
	use_armor(RARITY_NORMAL, 0, player);

	player->run = false;
}

void player_damage_increase(player_t* player)
{
	if (player->choice_hero == HERO_BERSERKER) {
		double hp_ratio = (double)get_player_hp(player) / get_player_max_hp(player);
		double missing_ratio = 1.0 - hp_ratio;

		double multiplier = 1.0 + pow(missing_ratio, 2.0) * RAGE_COEFF;

		if (multiplier > RAGE_MAX_MULTIPLIER) {
			multiplier = RAGE_MAX_MULTIPLIER; // 최대 3배로 제한
		}

		player->damage_increase = multiplier;
	}
	else {
		player->damage_increase = 1.0; // 브레이커와 카운터는 피해 증가 없음
	}
}

// getter
int    get_player_attack(player_t* player)             { return player->_attack; }
int    get_player_hp(player_t* player)                 { return player->_current_hp; }
int    get_player_max_hp(player_t* player)             { return player->_max_hp; }
int    get_player_speed(player_t* player)              { return player->_speed; }
double get_player_evasion_rate(player_t* player)       { return player->_evasion_rate; }
double get_player_defense_rate(player_t* player)       { return player->_defense_rate; }
double get_player_crit_chance(player_t* player)        { return player->_crit_chance; }
double get_player_crit_damage(player_t* player)        { return player->_crit_damage; }
int    get_player_break_damage(player_t* player)       { return player->_break_damage; }
int    get_player_break_extra_damage(player_t* player) { return player->_break_extra_damage; }
int    get_player_stun_duration(player_t* player)      { return player->_stun_duration; }

// Setters
// type 0: 영구적인 스탯 상승 (base 스탯과 현재 스탯 모두에 적용)
// type 1: 일시적인 스탯 상승 (현재 스탯에만 적용)
void set_player_attack(player_t* player, int value, int type)
{
	if (type == 0) player->_base_attack += value;
	player->_attack += value;
}

void set_player_hp(player_t* player, int value)
{
	player->_current_hp += value;
	if (player->_current_hp < 0) player->_current_hp = 0;
	else if (player->_current_hp > player->_max_hp) player->_current_hp = player->_max_hp;
}

void set_player_max_hp(player_t* player, int value, int type)
{
	if (type == 0) player->_base_max_hp += value;
	player->_max_hp += value;

	set_player_hp(player, value);
}

void set_player_speed(player_t* player, int value, int type)
{
	if (type == 0) player->_base_speed += value;
	player->_speed += value;
}

void set_player_evasion_rate(player_t* player, double value, int type)
{
	if (type == 0) player->_base_evasion_rate += value;
	player->_evasion_rate += value;
}

void set_player_defense_rate(player_t* player, double value, int type)
{
	if (type == 0) player->_base_defense_rate += value;
	player->_defense_rate += value;
}

void set_player_crit_chance(player_t* player, double value, int type)
{
	if (type == 0) player->_base_crit_chance += value;
	player->_crit_chance += value;
}

void set_player_crit_damage(player_t* player, double value, int type)
{
	if (type == 0) player->_base_crit_damage += value;
	player->_crit_damage += value;
}

void set_player_break_damage(player_t* player, int value, int type)
{
	if (type == 0) player->_base_break_damage += value;
	player->_break_damage += value;
}

void set_player_break_extra_damage(player_t* player, int value, int type)
{
	if (type == 0) player->_base_break_extra_damage += value;
	player->_break_extra_damage += value;
}

void set_player_stun_duration(player_t* player, int value, int type)
{
	if (type == 0) player->_base_stun_duration += value;
	player->_stun_duration += value;
}

void player_save_legacy_data(player_t* player)
{
	player->run = true;

	FILE* fp = fopen(REGACY_FILE_NAME, "wb");
	if (fp == NULL) {
		return;
	}

	legacy_data_t data = {
		.weapon_index = player->weapon_index,
		.armor_index = player->armor_index,
		.weapon_rarity = player->weapon_rarity,
		.armor_rarity = player->armor_rarity,
		.coin = player->coin
	};

	fwrite(&data, sizeof(data), 1, fp);
	fclose(fp);
}

bool player_load_legacy_data(player_t* player)
{
	FILE* fp = fopen(REGACY_FILE_NAME, "rb");
	if (fp == NULL) return false;

	legacy_data_t data;
	size_t count = fread(&data, sizeof(data), 1, fp);
	fclose(fp);

	if (count != 1) return false;

	player->coin = data.coin;

	get_item(data.weapon_rarity, data.weapon_index, 0);
	get_item(data.armor_rarity, data.armor_index, 1);

	use_weapon(data.weapon_rarity, data.weapon_index, player);
	use_armor(data.armor_rarity, data.armor_index, player);

	return true;
}