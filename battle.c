// battle.c
#include "battle.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

bool execute_attack(int attacker_attack, float defender_evasion_rate, float defender_defence_rate, int* out_defender_hp)
{
    if ((float)(rand() % 100) < defender_evasion_rate)
        return true;

    float damage = (float)attacker_attack * (1.0f - (defender_defence_rate / 100.0f));
    int final_damage = (int)ceilf(damage);
    *out_defender_hp -= final_damage;
    return false;
}

battle_result_t process_battle_turn(player_t* const player, monster_t* const monster)
{
	bool monster_evasion;
	bool player_evasion;

    if (player->speed >= monster->speed) {
        monster_evasion = execute_attack(player->attack, monster->evasion_rate, monster->defence_rate, &monster->current_hp);
        if (monster->current_hp <= 0) return BATTLE_RESULT_PLAYER_WIN;

        player_evasion = execute_attack(monster->attack, player->evasion_rate, player->defence_rate, &player->current_hp);
        if (player->current_hp <= 0) return BATTLE_RESULT_MONSTER_WIN;
    }
    else {
        player_evasion = execute_attack(monster->attack, player->evasion_rate, player->defence_rate, &player->current_hp);
        if (player->current_hp <= 0) return BATTLE_RESULT_MONSTER_WIN;

        monster_evasion = execute_attack(player->attack, monster->evasion_rate, monster->defence_rate, &monster->current_hp);
        if (monster->current_hp <= 0) return BATTLE_RESULT_PLAYER_WIN;
    }

    if(player_evasion && monster_evasion) {
        return EVASION_PLAYER_MONSTER;
    }
    else if(player_evasion) {
        return EVASION_PLAYER;
    }
    else if(monster_evasion) {
        return EVASION_MONSTER;
	}

    return BATTLE_RESULT_ONGOING;
}

void debugging_print_status(const player_t* player, const monster_t* monster, int turn)
{
    printf("===== Turn %d =====\n", turn);
    printf("[Player] HP: %d / %d | ATK: %d | SPD: %d | EVA: %.1f%% | DEF: %.1f%%\n",
        player->current_hp, player->max_hp, player->attack, player->speed,
        player->evasion_rate, player->defence_rate);

    printf("[Monster] HP: %d / %d | ATK: %d | SPD: %d | EVA: %.1f%% | DEF: %.1f%%\n",
        monster->current_hp, monster->max_hp, monster->attack, monster->speed,
        monster->evasion_rate, monster->defence_rate);
    printf("\n");
}