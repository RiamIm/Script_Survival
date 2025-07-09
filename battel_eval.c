// battel_eval.c
#define _CRT_SECURE_NO_WARNINGS
#include "battle_eval.h"

#define STAT_LOW_RATIO         0.8f
#define STAT_VERY_LOW_RATIO    0.66f

static double helper_safe_ratio(double a, double b) {
    return (b == 0.0) ? 999.0 : a / b;
}

static double helper_score_entity(int attack, int hp, double def, double eva, int speed) {
    (void)speed;

    double def_factor = (100.0f + def) / 100.0f;
    double eva_factor = (100.0f + eva) / 100.0f;

    return (double)attack * hp * def_factor * eva_factor;
}

typedef struct {
    const char* comment;
    double weight;
} EvalComment;

void battle_eval_analyze_state(const player_t* player, const monster_t* monster, battle_eval_result_t* result) {
    double hp_ratio = helper_safe_ratio(player->current_hp, monster->current_hp);
    double atk_ratio = helper_safe_ratio(player->attack, monster->attack);
    double def_ratio = helper_safe_ratio(player->defence_rate, monster->defence_rate);
    double eva_ratio = helper_safe_ratio(player->evasion_rate, monster->evasion_rate);

    EvalComment candidates[4];
    int count = 0;

    if (hp_ratio <= STAT_VERY_LOW_RATIO)
        candidates[count++] = (EvalComment){ "플레이어의 체력이 상대적으로 매우 낮습니다.", (1.0f / hp_ratio) };
    else if (hp_ratio <= STAT_LOW_RATIO)
        candidates[count++] = (EvalComment){ "플레이어의 체력이 상대적으로 낮습니다.", (1.0f / hp_ratio) };

    if (atk_ratio <= STAT_VERY_LOW_RATIO)
        candidates[count++] = (EvalComment){ "플레이어의 공격력이 상대적으로 매우 낮습니다.", (1.0f / atk_ratio) };
    else if (atk_ratio <= STAT_LOW_RATIO)
        candidates[count++] = (EvalComment){ "플레이어의 공격력이 상대적으로 낮습니다.", (1.0f / atk_ratio) };

    if (def_ratio <= STAT_VERY_LOW_RATIO)
        candidates[count++] = (EvalComment){ "플레이어의 방어력이 상대적으로 매우 낮습니다.", (1.0f / def_ratio) };
    else if (def_ratio <= STAT_LOW_RATIO)
        candidates[count++] = (EvalComment){ "플레이어의 방어력이 상대적으로 낮습니다.", (1.0f / def_ratio) };

    if (eva_ratio <= STAT_VERY_LOW_RATIO)
        candidates[count++] = (EvalComment){ "플레이어의 회피율이 상대적으로 매우 낮습니다.", (1.0f / eva_ratio) };
    else if (eva_ratio <= STAT_LOW_RATIO)
        candidates[count++] = (EvalComment){ "플레이어의 회피율이 상대적으로 낮습니다.", (1.0f / eva_ratio) };

    if (count > 0) {
        EvalComment* worst = &candidates[0];
        for (int i = 1; i < count; ++i) {
            if (candidates[i].weight > worst->weight)
                worst = &candidates[i];
        }
        result->comments[result->comment_count++] = worst->comment;
    }
}

battle_eval_result_t battle_eval_evaluate(const player_t* player, const monster_t* monster) {
    battle_eval_result_t result = { 0 };
    double p_score = helper_score_entity(player->attack, player->current_hp, player->defence_rate, player->evasion_rate, player->speed);
    double m_score = helper_score_entity(monster->attack, monster->current_hp, monster->defence_rate, monster->evasion_rate, monster->speed);

    double ratio = helper_safe_ratio(p_score, m_score);

    if (ratio >= 2.0f)
        result.overall = BATTLE_EVAL_PLAYER_DOMINANT;
    else if (ratio >= 1.5f)
        result.overall = BATTLE_EVAL_PLAYER_ADVANTAGE;
    else if (ratio >= 0.75f)
        result.overall = BATTLE_EVAL_EVEN;
    else if (ratio >= 0.5f)
        result.overall = BATTLE_EVAL_MONSTER_ADVANTAGE;
    else
        result.overall = BATTLE_EVAL_MONSTER_DOMINANT;

    battle_eval_analyze_state(player, monster, &result);
    return result;
}