// player.c
#define _CRT_SECURE_NO_WARNINGS
#include "player.h"

// 초기모델, 나중에 세이브 추가하면 변경 해야 함
void player_init(player_t* player, char* name, hero_t choice_hero)
{
	player->choice_hero = choice_hero;
	strcpy(player->name, name);

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
		player->attack = 100;
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