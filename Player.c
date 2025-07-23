// player.c
#define _CRT_SECURE_NO_WARNINGS
#include "player.h"

// 초기모델, 나중에 세이브 추가하면 변경 해야 함
void player_init(player_t* player, char* name, hero_t choice_hero)
{
	strcpy(player->name, name);

	if (choice_hero == HERO_BREAKER)
	{
		player->attack = 100;
		player->max_hp = 350;
		player->current_hp = player->max_hp;
		player->speed = 500;
		player->break_damage = 30;

		player->is_focused = false;

		player->self_damage = 0.0;
		player->life_steal = 0.0;
	}
	else if (choice_hero == HERO_COUNTER)
	{
		player->attack = 100;
		player->max_hp = 500;
		player->current_hp = player->max_hp;
		player->speed = 500;
		player->break_damage = 20;

		player->is_focused = false;

		player->self_damage = 0.0;
		player->life_steal = 0.0;
	}
	else if (choice_hero == HERO_BERSERKER)
	{
		player->attack = 100;
		player->max_hp = 800;
		player->current_hp = player->max_hp;
		player->speed = 250;
		player->break_damage = 10;

		player->is_focused = false;

		player->self_damage = 30.0;
		player->life_steal = 10.0;
	}

	player->evasion_rate = 0.0;
	player->defence_rate = 0.0;

	player->coin = 1000000;

	player->weapon_index = -1;
	player->armor_index = -1;

	player->weapon_rarity = RARITY_NORMAL;
	player->armor_rarity = RARITY_NORMAL;

	use_weapon(RARITY_NORMAL, 0, player);
	use_armor(RARITY_NORMAL, 0, player);
}