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
		player->max_hp = 450;
		player->attack = 100;
		player->speed = 550;
		player->crit_chance = 0.05;
		player->crit_damage = 1.5;

		player->break_damage = 20;
		player->break_extra_damage = 300;
		player->stun_duration = 2;

		player->self_damage = 0.0;
		player->life_steal = 0.0;
	}
	else if (choice_hero == HERO_COUNTER)
	{
		player->max_hp = 550;
		player->attack = 100;
		player->speed = 475;
		player->crit_chance = 0.05;
		player->crit_damage = 1.5;

		player->break_damage = 10;
		player->break_extra_damage = 0;
		player->stun_duration = 1;

		player->self_damage = 0.0;
		player->life_steal = 0.0;
	}
	else if (choice_hero == HERO_BERSERKER)
	{
		player->max_hp = 500;
		player->attack = 1000;
		player->speed = 500;
		player->crit_chance = 0.05;
		player->crit_damage = 1.5;

		player->break_damage = 15;
		player->break_extra_damage = 0;
		player->stun_duration = 1;

		player->self_damage = 0.3;
		player->life_steal = 0.1;
	}
	
	// --- 공통 초기화 --- 
	player->evasion_rate = 0.05;
	player->current_hp = player->max_hp;
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
	player->evasion_to_defence = 1.0;
	player->defence_from_evasion = 0.0;
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
		double hp_ratio = (double)player->current_hp / player->max_hp;
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