// battle.h
#pragma once

#include <stdbool.h>
#include "pair.h"
#include "player.h"
#include "monster.h"

typedef enum battle_result {
    BATTLE_RESULT_ONGOING,
    BATTLE_RESULT_PLAYER_WIN,
    BATTLE_RESULT_MONSTER_WIN,
    BATTLE_RESULT_DRAW,
    EVASION_PLAYER_MONSTER,
	EVASION_MONSTER,
    EVASION_PLAYER,
    EVASION_NONE
} battle_result_t;

battle_result_t process_battle_turn(player_t* const player, monster_t* const monster);
void debugging_print_status(const player_t* player, const monster_t* monster, int turn);