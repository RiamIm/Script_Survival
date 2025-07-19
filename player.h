// player.h
#pragma once
#include "inout.h"
#include "utils.h"

#include "item.h"
#include "inventory.h"

typedef struct player {
	char name[125];
	int attack;
	int max_hp;
	int current_hp;
	int speed;
	int break_damage;

	bool is_focused;
	double action_value; 

	double evasion_rate;
	double defence_rate;

	int coin;

	int forest_mastery;
	int snow_mastery;
	int desert_mastery;

	int weapon_index;
	int armor_index;

} player_t;

void player_init(player_t* player, char* name);

/* 
각 지역의 마스터리 레벨이 3 6 9 씩 달성하면 스텟 증가 함수 구현
region: 0 = forest, 1 = snow, 2 = desert
level: player->forest_mastery, player->snow_mastery, player->desert_mastery 
*/
void player_increase_mastery(player_t* player, int region, int level);