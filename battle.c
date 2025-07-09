// battle.c
#define _CRT_SECURE_NO_WARNINGS
#include "battle.h"

bool helper_execute_attack(int attacker_attack, double defender_evasion_rate, double defender_defence_rate, int* out_defender_hp)
{
    if ((double)(rand() % 100) < defender_evasion_rate)
        return true;

    double damage = (double)attacker_attack * (1.0f - (defender_defence_rate / 100.0f));
    int final_damage = (int)ceil(damage);
    *out_defender_hp -= final_damage;
    return false;
}

battle_result_t battle_process(player_t* const player, monster_t* const monster)
{
	bool monster_evasion;
	bool player_evasion;

    if (player->speed >= monster->speed) {
        monster_evasion = helper_execute_attack(player->attack, monster->evasion_rate, monster->defence_rate, &monster->current_hp);
        log_deal_damage(player, monster, player->attack, player->speed >= monster->speed);
        if (monster->current_hp <= 0) return BATTLE_RESULT_PLAYER_WIN;
        Sleep(1000);

        player_evasion = helper_execute_attack(monster->attack, player->evasion_rate, player->defence_rate, &player->current_hp);
        log_take_damage(player, monster, monster->attack, player->speed < monster->speed);
        if (player->current_hp <= 0) return BATTLE_RESULT_MONSTER_WIN;
        Sleep(1000);
    }
    else {
        player_evasion = helper_execute_attack(monster->attack, player->evasion_rate, player->defence_rate, &player->current_hp);
		log_take_damage(player, monster, monster->attack, player->speed < monster->speed);
        if (player->current_hp <= 0) return BATTLE_RESULT_MONSTER_WIN;
        Sleep(1000);

        monster_evasion = helper_execute_attack(player->attack, monster->evasion_rate, monster->defence_rate, &monster->current_hp);
		log_deal_damage(player, monster, player->attack, player->speed >= monster->speed);
        if (monster->current_hp <= 0) return BATTLE_RESULT_PLAYER_WIN;
        Sleep(1000);
    }

    UI_cleaner_battle_log();

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