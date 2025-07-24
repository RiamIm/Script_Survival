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
		player->attack = 100;
		player->max_hp = 350;
		player->current_hp = player->max_hp;
		player->speed = 500;
		player->break_damage = 30;

		player->self_damage = 0.0;
		player->life_steal = 0.0;

		player->break_extra_damage = 30;      // 기본 격파 시 추가 피해량 0
		player->stun_duration = 1;           // 기본 스턴 시간 1턴
	}
	else if (choice_hero == HERO_COUNTER)
	{
		player->attack = 100;
		player->max_hp = 500;
		player->current_hp = player->max_hp;
		player->speed = 500;
		player->break_damage = 20;

		player->self_damage = 0.0;
		player->life_steal = 0.0;

		player->break_extra_damage = 0;      // 기본 격파 시 추가 피해량 0
		player->stun_duration = 1;           // 기본 스턴 시간 1턴
	}
	else if (choice_hero == HERO_BERSERKER)
	{
		player->attack = 100;
		player->max_hp = 800;
		player->current_hp = player->max_hp;
		player->speed = 250;
		player->break_damage = 10;

		player->self_damage = 0.3;
		player->life_steal = 0.1;

		player->break_extra_damage = 0;      // 기본 격파 시 추가 피해량 0
		player->stun_duration = 1;           // 기본 스턴 시간 1턴
	}

	player->is_counter = false;

	player->evasion_rate = 0.0;
	player->defence_rate = 0.0;

	// ============ 추가된 스탯 기본값 설정 ============
	player->crit_chance = 0.05;          // 기본 치명타 확률 5%
	player->crit_damage_modifier = 1.5;  // 기본 치명타 피해 150%
	player->damage_increase = 1.0;       // 기본 가하는 피해 증가 0%
	// =============================================

	player->auto_heal = 0.3;

	player->coin = 1000000;

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