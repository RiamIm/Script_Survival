#include <stdio.h>
#include <string.h>
#include "player.h"

// �ʱ��, ���߿� ���̺� �߰��ϸ� ���� �ؾ� ��
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
}