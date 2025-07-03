// battel_eval.c
#include "battle_eval.h"
#include <stdio.h>
#include <math.h>

#define STAT_LOW_RATIO         0.8f
#define STAT_VERY_LOW_RATIO    0.66f

static float safe_ratio(float a, float b) {
    return (b == 0.0f) ? 999.0f : a / b;
}

static float score_entity(int attack, int hp, float def, float eva, int speed) {
    (void)speed;

    float def_factor = (100.0f + def) / 100.0f;
    float eva_factor = (100.0f + eva) / 100.0f;

    return (float)attack * (float)hp * def_factor * eva_factor;
}

typedef struct {
    const char* comment;
    float weight;
} EvalComment;

static void analyze_stat_comments(const player_t* player, const monster_t* monster, battle_eval_result_t* result) {
    float hp_ratio = safe_ratio(player->current_hp, monster->current_hp);
    float atk_ratio = safe_ratio(player->attack, monster->attack);
    float def_ratio = safe_ratio(player->defence_rate, monster->defence_rate);
    float eva_ratio = safe_ratio(player->evasion_rate, monster->evasion_rate);

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

battle_eval_result_t evaluate_battle(const player_t* player, const monster_t* monster) {
    battle_eval_result_t result = { 0 };
    float p_score = score_entity(player->attack, player->current_hp, player->defence_rate, player->evasion_rate, player->speed);
    float m_score = score_entity(monster->attack, monster->current_hp, monster->defence_rate, monster->evasion_rate, monster->speed);

    float ratio = safe_ratio(p_score, m_score);

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

    analyze_stat_comments(player, monster, &result);
    return result;
}

void debugging_print_battle_evaluation(const battle_eval_result_t* result) {
    static const char* grade_texts[] = {
        "플레이어 압도",
        "플레이어 유리",
        "접전",
        "몬스터 유리",
        "몬스터 압도"
    };

    printf("[전투 평가 결과] : %s\n", grade_texts[result->overall]);

    for (size_t i = 0; i < result->comment_count; ++i) {
        printf("- %s\n", result->comments[i]);
    }
    printf("\n");
}