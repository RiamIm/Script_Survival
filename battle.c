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

static void s_apply_damage(int attacker_attack, double damage_increase, double defender_defence_rate, int* out_defender_hp, int* out_final_damage)
{
    double damage = (double)attacker_attack * (1.0 - defender_defence_rate) * damage_increase;
    *out_final_damage = (int)ceil(damage);
    *out_defender_hp -= *out_final_damage;
}

static void s_battle_logic(player_t* player, monster_t* monster, bool is_use_skill)
{
    bool monster_evaded = s_check_evasion(monster->evasion_rate);

    if (monster_evaded) {
        log_evaded(monster->name, player->name);
    }
    else {
        double attack_power = (double)player->attack;
		int skill_break_damge = player->break_damage;

        if (player->choice_hero == HERO_BREAKER && is_use_skill) {
			attack_power *= 0.2; // 브레이커의 스킬 사용 시, 공격력 감소
			skill_break_damge *= 3; // 브레이커의 스킬 사용 시, 격파 피해 3배
        }
        else if (player->choice_hero == HERO_COUNTER && is_use_skill) {
            attack_power *= 2.0; // 카운터의 스킬 사용 시, 공격력 2배
        }

        // --- 1. 일반 공격 피해 계산 (치명타, 피해 증가 포함) ---  
        bool is_critical = ((double)rand() / RAND_MAX) < player->crit_chance;

        if (is_critical) {
            attack_power *= player->crit_damage_modifier;
        }
        attack_power *= player->damage_increase;

        int final_damage = 0;
        s_apply_damage((int)attack_power, player->damage_increase, monster->defence_rate, &monster->current_hp, &final_damage);

        // --- 2. 격파 추가 피해 계산 (몬스터가 기절 상태일 때만) ---
        int break_extra_damage_dealt = 0;
        if (monster->is_groggy) {
            break_extra_damage_dealt = player->break_extra_damage;
            monster->current_hp -= break_extra_damage_dealt; // 방어 무시 피해로 직접 차감
        }

        // --- 3. 강인도 및 그로기 처리 ---
        monster->current_toughness -= skill_break_damge;
        if (monster->current_toughness <= 0) {
            monster->current_toughness = 0;
            if (monster->is_groggy == false) {
                monster->is_groggy = true;
                monster->stun_turns = player->stun_duration;
                log_monster_groggy(monster->name);
            }
        }

        // --- 4. 로그 기록 ---
        if (player->choice_hero == HERO_COUNTER && is_use_skill) {
			log_player_counter_success(player, monster, final_damage, skill_break_damge, is_critical);
        }
        else {
            log_player_attack(player, monster, final_damage, skill_break_damge, is_critical, break_extra_damage_dealt);
        }
        UI_dynamic_monster_flash_effect(monster);

        if (!is_use_skill && player->choice_hero == HERO_BERSERKER) {
            int heal_point = (int)(final_damage * player->life_steal);
            player->current_hp += heal_point; // 생명력 흡수
            if (player->current_hp > player->max_hp) {
				heal_point -= (player->current_hp - player->max_hp); 
                player->current_hp = player->max_hp; // 체력이 최대치를 넘지 않도록 처리
            }
            log_life_steal(player, heal_point); // 생명력 흡수 로그 기록
        }  
    }
}

// 플레이어의 턴만 처리하는 함수
battle_result_t player_turn_process(player_t* player, monster_t* monster, player_action_t action)
{
    log_buffer_clear();

    if (action == PLAYER_ACTION_SKILL) {
		log_skill_used(player, (int)(player->self_damage * player->current_hp));
        if (player->choice_hero == HERO_BREAKER) {
            s_battle_logic(player, monster, true);
        }
        else if (player->choice_hero == HERO_COUNTER) {
            player->is_counter = true; // 카운터의 스킬은 카운터 상태로 전환
            log_player_counter_ready(player);
        }
        else if (player->choice_hero == HERO_BERSERKER) {
			player->current_hp -= (int)(player->self_damage * player->current_hp);
            // 예외처리
            if (player->current_hp < 1) {
                player->current_hp = 1; // 체력이 0 이하로 떨어지지 않도록 처리
			}
			player_damage_increase(player);
			s_battle_logic(player, monster, true);
        }
    }
    else if (action == PLAYER_ACTION_ATTACK) {
		s_battle_logic(player, monster, false);
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

    if (player->choice_hero == HERO_COUNTER && player->is_counter) {
        // 플레이어 회피 불가
		player_evaded = false;
    }

    if (player_evaded) {
        log_evaded(player->name, monster->name);
    }
    else {
        int final_damage = 0;
        int monster_damage = monster->attack;

        s_apply_damage(monster_damage, 1.0 ,player->defence_rate, &player->current_hp, &final_damage);
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

    // 반격
    if (player->choice_hero == HERO_COUNTER && player->is_counter) {
        s_battle_logic(player, monster, player->is_counter);
        player->is_counter = false;
        if (monster->current_hp <= 0) return BATTLE_RESULT_PLAYER_WIN;
	}

    return BATTLE_RESULT_ONGOING;
}