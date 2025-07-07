#pragma once
#include <stdio.h>
#include <Windows.h>

#include "player.h"
#include "monster.h"
#include "utils.h"

void print_attack_log(player_t* player, monster_t* monster, int attack, bool is_player_attack);
void print_damage_taken_log(player_t* player, monster_t* monster, int attack, bool is_monster_attack);
void print_evasion_log(player_t* player, monster_t* monster);

void log_clear(void);