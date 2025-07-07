#include <stdio.h>
#include <Windows.h>

#include "log.h"

void print_attack_log(player_t* player, monster_t* monster, int attack, bool is_player_attack) {
	gotoxy(42, (is_player_attack) ? 21 : 22);
	printf("%s가 %s에게 %d의 피해를 주었습니다!", player->name, monster->name, attack);
}

void print_damage_taken_log(player_t* player, monster_t* monster, int attack, bool is_monster_attack) {
	gotoxy(42, (is_monster_attack) ? 21 : 22);
	printf("%s가 %s에게 %d의 피해를 받았습니다!", monster->name, player->name, attack);
}

void print_evasion_log(player_t* player, monster_t* monster)
{
	// TODO
}

void log_clear(void)
{
	int x = 42;
	int y = 21;

	for (int i = 0; i < 6; i++)
	{
		gotoxy(x, y + i);
		printf("                                                 ");
	}
}