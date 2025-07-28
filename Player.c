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
		player->max_hp = 11000;
		player->attack = 1050;
		player->current_hp = player->max_hp;
		player->speed = 800;

		player->evasion_rate = 0.2;
		player->crit_chance = 0.3;
		player->crit_damage_modifier = 1.5;

		player->break_damage = 30;

		player->self_damage = 0.0;
		player->life_steal = 0.0;

		player->break_extra_damage = 8000;    
		player->stun_duration = 2;        
	}
	else if (choice_hero == HERO_COUNTER)
	{
		player->max_hp = 8500;
		player->attack = 1500;
		player->current_hp = player->max_hp;
		player->speed = 475;

		player->evasion_rate = 0.15;
		player->crit_chance = 0.3;
		player->crit_damage_modifier = 2.0;

		player->break_damage = 10;

		player->self_damage = 0.0;
		player->life_steal = 0.0;

		player->break_extra_damage = 0;
		player->stun_duration = 1;
	}
	else if (choice_hero == HERO_BERSERKER)
	{
		player->max_hp = 13500;
		player->attack = 2000;
		player->current_hp = player->max_hp;
		player->speed = 500;

		player->evasion_rate = 0.1;
		player->crit_chance = 0.2;
		player->crit_damage_modifier = 1.8;

		player->break_damage = 15;

		player->self_damage = 0.3;
		player->life_steal = 0.4;

		player->break_extra_damage = 0;
		player->stun_duration = 1;
	}

	player->is_counter = false;

	player->auto_heal = 0.3;

	player->coin = 1000000;

	player->weapon_index = -1;
	player->armor_index = -1;

	player->damage_increase = 1.0;

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