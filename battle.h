// battle.h
#pragma once
#include "utils.h"

#include "player.h"
#include "monster.h"

#include "UI_info.h"

typedef enum battle_result {
    BATTLE_RESULT_ONGOING,
    BATTLE_RESULT_PLAYER_WIN,
    BATTLE_RESULT_MONSTER_WIN
} battle_result_t;

// 각 턴을 처리하는 함수들을 분리하여 선언
battle_result_t player_turn_process(player_t* player, monster_t* monster, player_action_t action);
battle_result_t monster_turn_process(monster_t* monster, player_t* player);