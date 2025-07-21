// battle.c
#define _CRT_SECURE_NO_WARNINGS
#include "battle.h"
#include "log.h"
#include "utils.h"
#include "UI_static.h"

static bool s_check_evasion(double defender_evasion_rate)
{
    return (((double)rand() / RAND_MAX) < defender_evasion_rate);
}

static void s_apply_damage(int attacker_attack, double defender_defence_rate, int* out_defender_hp, int* out_final_damage)
{
    double damage = (double)attacker_attack * (1.0 - defender_defence_rate);
    *out_final_damage = (int)ceil(damage);
    *out_defender_hp -= *out_final_damage;
}

// 플레이어의 턴만 처리하는 함수
battle_result_t player_turn_process(player_t* player, monster_t* monster, player_action_t action)
{
    log_buffer_clear();

    if (action == PLAYER_ACTION_FOCUS) {
        player->is_focused = true;
        log_player_focus(player);
        Sleep(1000);
    }
    else if (action == PLAYER_ACTION_ATTACK) {
        bool monster_evaded = s_check_evasion(monster->evasion_rate);

        if (monster_evaded) {
            log_evaded(monster->name, player->name);
        }
        else {
            int final_damage = 0;
            int break_damage = player->break_damage;

            if (player->is_focused) {
                break_damage *= 3;
                player->is_focused = false; // 버프 소모
            }

            s_apply_damage(player->attack, monster->defence_rate, &monster->current_hp, &final_damage);
            log_player_attack(player, monster, final_damage, break_damage);

            monster->current_toughness -= break_damage;
            if (monster->current_toughness <= 0) {
                monster->current_toughness = 0;
                if (monster->is_groggy == false) {
                    monster->is_groggy = true;
                    monster->stun_turns = 1; // 1턴 기절
                    log_monster_groggy(monster->name);
                }
            }
        }
        Sleep(1000);
    }

    if (monster->current_hp <= 0) return BATTLE_RESULT_PLAYER_WIN;
    return BATTLE_RESULT_ONGOING;
}

// 몬스터의 턴만 처리하는 함수
battle_result_t monster_turn_process(monster_t* monster, player_t* player)
{
    if (monster->stun_turns > 0) {
        log_monster_stunned(monster->name);
        monster->stun_turns--; // 기절 턴 수 감소

        // 이번 턴에 기절이 풀렸다면 강인도 회복
        if (monster->stun_turns == 0) {
            monster->is_groggy = false;
            monster->current_toughness = monster->max_toughness;
            log_monster_recovers(monster->name);
        }
        Sleep(1000);
        return BATTLE_RESULT_ONGOING;
    }

    bool player_evaded = s_check_evasion(player->evasion_rate);

    if (player_evaded) {
        log_evaded(player->name, monster->name);
    }
    else {
        int final_damage = 0;
        int monster_damage = monster->attack;

        if (player->is_focused) {
            monster_damage = (int)(monster_damage * 0.8);
        }

        s_apply_damage(monster_damage, player->defence_rate, &player->current_hp, &final_damage);
        log_monster_attack(player, monster, final_damage);

        // 여기서 메인박스 색 빨간색으로 "짧게" 전환
        UI_static_main_box(COLOR_LIGHTRED); // 혹은 COLOR_HIT 등 정의!
        Sleep(200); // 0.12초간 빨강색 유지
        UI_static_main_box(COLOR_WHITE);  // 다시 원래색(흰색 등)으로
    }
    Sleep(1000);

    if (player->current_hp <= 0) return BATTLE_RESULT_MONSTER_WIN;
    return BATTLE_RESULT_ONGOING;
}