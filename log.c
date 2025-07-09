// log.c
#define _CRT_SECURE_NO_WARNINGS

// log.c
#define _CRT_SECURE_NO_WARNINGS
#include "log.h"

void log_deal_damage(player_t* player, monster_t* monster, int attack, bool is_player_attack) {
	utils_gotoxy(42, (is_player_attack) ? 21 : 22);
	printf("%s가 %s에게 %d의 피해를 주었습니다!", player->name, monster->name, attack);
}

void log_take_damage(player_t* player, monster_t* monster, int attack, bool is_monster_attack) {
	utils_gotoxy(42, (is_monster_attack) ? 21 : 22);
	printf("%s가 %s에게 %d의 피해를 받았습니다!", player->name, monster->name, attack);
}

void log_evade(player_t* player, monster_t* monster)
{
	// TODO
}