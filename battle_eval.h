// battel_eval.h
#pragma once
#include "inout.h"
#include "utils.h"

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

// 헬퍼 함수
static double helper_score_entity(int attack, int hp, double def, double eva, int speed);
static double helper_safe_ratio(double a, double b);

// 전투 평가 함수
void                 battle_eval_analyze_state(const player_t* player, const monster_t* monster, battle_eval_result_t* result);
battle_eval_result_t battle_eval_evaluate(const player_t* player, const monster_t* monster);