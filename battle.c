// battle.c
#define _CRT_SECURE_NO_WARNINGS
#include "battle.h"
#include "log.h"
#include "utils.h"
#include "UI_static.h"
#include "UI_dynamic.h"

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
            // --- 1. 일반 공격 피해 계산 (치명타, 피해 증가 포함) ---
            double attack_power = (double)player->attack;
            bool is_critical = ((double)rand() / RAND_MAX) < player->crit_chance;

            if (is_critical) {
                attack_power *= player->crit_damage_modifier;
            }
            attack_power *= (1.0 + player->damage_increase);

            int final_damage = 0;
            s_apply_damage((int)attack_power, monster->defence_rate, &monster->current_hp, &final_damage);

            // --- 2. 격파 추가 피해 계산 (몬스터가 기절 상태일 때만) ---
            int break_extra_damage_dealt = 0;
            if (monster->is_groggy) {
                break_extra_damage_dealt = player->break_extra_damage;
                monster->current_hp -= break_extra_damage_dealt; // 방어 무시 피해로 직접 차감
            }

            // --- 3. 로그 기록 ---
            int break_damage = player->is_focused ? player->break_damage * 3 : player->break_damage;
            log_player_attack(player, monster, final_damage, break_damage, is_critical, break_extra_damage_dealt);
            UI_dynamic_monster_flash_effect(monster);

            if (player->is_focused) player->is_focused = false;

            // --- 4. 강인도 및 그로기 처리 ---
            monster->current_toughness -= break_damage;
            if (monster->current_toughness <= 0) {
                monster->current_toughness = 0;
                if (monster->is_groggy == false) {
                    monster->is_groggy = true;
                    monster->stun_turns = player->stun_duration;
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

        // ======== 플레이어 피격 효과 (두 번 깜빡이도록 수정) ========
        for (int i = 0; i < 2; i++)
        {
            UI_static_main_box(COLOR_LIGHTRED);
            Sleep(50);
            UI_static_main_box(COLOR_WHITE);
            Sleep(50);
        }
    }
    Sleep(1000);

    if (player->current_hp <= 0) return BATTLE_RESULT_MONSTER_WIN;
    return BATTLE_RESULT_ONGOING;
}