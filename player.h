#pragma once

#include "item.h"

#define INVENTORY_SIZE 10

typedef struct player {
	char name[125];
	int attack;
	int max_hp;
	int current_hp;
	int speed;

	float evasion_rate;
	float defence_rate;

	int coin;

	int forest_mastery;
	int swamp_mastery;
	int desert_mastery;

	item_t inventory[INVENTORY_SIZE]; // 플레이어의 아이템 인벤토리
	int inventory_count; // 현재 인벤토리에 있는 아이템 개수

	item_t* equipped_weapon; // 장착된 무기
	item_t* equipped_armor; // 장착된 방어구

} player_t;

void init_player(player_t* player, char* name);

/* 
각 지역의 마스터리 레벨이 3 6 9 씩 달성하면 스텟 증가 함수 구현
region: 0 = forest, 1 = swamp, 2 = desert
level: player->forest_mastery, player->swamp_mastery, player->desert_mastery 
*/
void increase_mastery(player_t* player, int region, int level);
