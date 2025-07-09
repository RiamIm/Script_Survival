// battle.h
#pragma once
#include "inout.h"
#include "utils.h"

#include "player.h"
#include "monster.h"
#include "log.h"

#include "UI_info.h"
#include "UI_cleaner.h"

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

// 헬퍼 함수
bool helper_execute_attack(int attacker_attack, double defender_evasion_rate, double defender_defence_rate, int* out_defender_hp);

// 전투 함수
battle_result_t battle_process(player_t* const player, monster_t* const monster);