#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "player.h"

// 초기모델, 나중에 세이브 추가하면 변경 해야 함
void init_player(player_t* player, char* name)
{
	strcpy(player->name, name);
	player->attack = 20;
	player->max_hp = 500;
	player->current_hp = player->max_hp;
	player->speed = 100;

	player->evasion_rate = 0.1;
	player->defence_rate = 0.1;

	player->coin = 0;

	player->forest_mastery = 0;
	player->swamp_mastery = 0;
	player->desert_mastery = 0;

	player->inventory_count = 0;
	for (int i = 0; i < INVENTORY_SIZE; i++) {
		player->inventory[i].type = ITEM_TYPE_NULL; // 초기화
		player->inventory[i].quantity = 0;
	}

	player->equipped_weapon = NULL;
	player->equipped_armor = NULL;
}

/*
각 지역의 마스터리 레벨이 3 6 9 씩 달성하면 스텟 증가 함수 구현
region: 0 = forest, 1 = swamp, 2 = desert
level: player->forest_mastery, player->swamp_mastery, player->desert_mastery
*/
void increase_mastery(player_t* player, int region, int level)
{
	switch (region)
	{
	case 0:
		if (level == 3) {
			player->max_hp += 50;
		} else if (level == 6) {
			player->max_hp += 100;
		} else if (level == 9) {
			player->max_hp += 150;
		}
		break;
	case 1:
		if (level == 3) {
			player->evasion_rate += 0.05f;
			player->attack += 5;
		}
		else if (level == 6) {
			player->evasion_rate += 0.1f;
			player->attack += 10;
		} else if (level == 9) {
			player->evasion_rate += 0.15f;
			player->attack += 15;
		}
		break;
	case 2:
		if (level == 3) {
			player->defence_rate += 0.05f;
			player->speed += 5;
		} else if (level == 6) {
			player->defence_rate += 0.1f;
			player->speed += 10;
		} else if (level == 9) {
			player->defence_rate += 0.15f;
			player->speed += 15;
		}
	}
}