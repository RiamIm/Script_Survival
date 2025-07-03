// battel_eval.h
#pragma once
#include "battle.h"

typedef enum {
    BATTLE_EVAL_PLAYER_DOMINANT,
    BATTLE_EVAL_PLAYER_ADVANTAGE,
    BATTLE_EVAL_EVEN,
    BATTLE_EVAL_MONSTER_ADVANTAGE,
    BATTLE_EVAL_MONSTER_DOMINANT
} battle_eval_grade_t;

typedef struct {
    battle_eval_grade_t overall;
    const char* comments[10];
    int comment_count;
} battle_eval_result_t;

battle_eval_result_t evaluate_battle(const player_t* player, const monster_t* monster);
void debugging_print_battle_evaluation(const battle_eval_result_t* result);