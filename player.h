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

	item_t inventory[INVENTORY_SIZE]; // �÷��̾��� ������ �κ��丮
	int inventory_count; // ���� �κ��丮�� �ִ� ������ ����

	item_t* equipped_weapon; // ������ ����
	item_t* equipped_armor; // ������ ��

} player_t;

void init_player(player_t* player, char* name);

/* 
�� ������ �����͸� ������ 3 6 9 �� �޼��ϸ� ���� ���� �Լ� ����
region: 0 = forest, 1 = swamp, 2 = desert
level: player->forest_mastery, player->swamp_mastery, player->desert_mastery 
*/
void increase_mastery(player_t* player, int region, int level);
