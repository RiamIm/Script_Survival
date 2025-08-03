#define _CRT_SECURE_NO_WARNINGS
#include "battle.h"
#include "log.h"
#include "utils.h"
#include "UI_static.h"
#include "UI_dynamic.h"

#include "game_manager.h"

#include <windows.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

static double s_monster_tmep_evasion_rate;

static bool s_check_evasion(double defender_evasion_rate)
{
    return (((double)genrand_int32() / UPPER_MASK) < defender_evasion_rate);
}

static void s_apply_damage(int attacker_attack, double damage_increase, double attacker_def_penetration, double defender_defence_rate, int* out_defender_hp, int* out_final_damage)
{
	double effective_defence = defender_defence_rate * (1.0 - attacker_def_penetration);

    if (effective_defence < 0.0) {
        effective_defence = 0.0; // 방어율이 음수로 내려가지 않도록 보정
	}
    else if (effective_defence > 1.0) {
        effective_defence = 1.0; // 방어율이 100%를 넘지 않도록 보정
	}

    double damage = (double)attacker_attack * (1.0 - effective_defence) * damage_increase;
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
            attack_power *= 0.2;
            skill_break_damge *= 3;
        }
        else if (player->choice_hero == HERO_COUNTER && is_use_skill) {
            attack_power *= 2.0;
        }

        bool is_critical = ((double)genrand_int32() / UPPER_MASK) < player->crit_chance;

        if (is_critical) {
            attack_power *= player->crit_damage;
        }
        attack_power *= player->damage_increase;

        int final_damage = 0;
        s_apply_damage((int)attack_power, player->damage_increase, player->defence_penetration, monster->defence_rate, &monster->current_hp, &final_damage);

        int break_extra_damage_dealt = 0;
        if (monster->is_groggy) {
            break_extra_damage_dealt = player->break_extra_damage;
            monster->current_hp -= break_extra_damage_dealt;
        }

        if (player->choice_hero == HERO_COUNTER && is_use_skill) {
            log_player_counter_success(player, monster, final_damage, skill_break_damge, is_critical);
        }
        else {
            log_player_attack(player, monster, final_damage, skill_break_damge, is_critical, break_extra_damage_dealt);
        }

        PlaySound(TEXT("BGM/SoundEffect/player_attack.wav"), NULL, SND_ASYNC);

        UI_dynamic_monster_flash_effect(monster);

        if (!is_use_skill && player->choice_hero == HERO_BERSERKER) {
            int heal_point = (int)(final_damage * player->life_steal);
            player->current_hp += heal_point;
            if (player->current_hp > player->max_hp) {
                heal_point -= (player->current_hp - player->max_hp);
                player->current_hp = player->max_hp;
            }
            log_life_steal(player, heal_point);
        }

        monster->current_toughness -= skill_break_damge;
        if (monster->current_toughness <= 0) {
            monster->current_toughness = 0;
            if (monster->is_groggy == false) {
                monster->is_groggy = true;
                s_monster_tmep_evasion_rate = monster->evasion_rate;
                monster->evasion_rate = 0.0;
                monster->stun_turns = player->stun_duration;
                log_monster_groggy(monster->name);
            }
        }
    }
}

battle_result_t player_turn_process(player_t* player, monster_t* monster, player_action_t action)
{
    log_buffer_clear();

    if (action == PLAYER_ACTION_SKILL) {
        log_skill_used(player, (int)(player->self_damage * player->current_hp));
        if (player->choice_hero == HERO_BREAKER) {
            s_battle_logic(player, monster, true);
        }
        else if (player->choice_hero == HERO_COUNTER) {
            player->is_counter = true;
            log_player_counter_ready(player);
        }
        else if (player->choice_hero == HERO_BERSERKER) {
            player->current_hp -= (int)(player->self_damage * player->current_hp);
            if (player->current_hp < 1) {
                player->current_hp = 1;
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

battle_result_t monster_turn_process(monster_t* monster, player_t* player)
{
    if (monster->stun_turns > 0) {
        log_monster_stunned(monster->name);
        monster->stun_turns--;

        if (monster->stun_turns == 0) {
            monster->is_groggy = false;
            monster->evasion_rate = s_monster_tmep_evasion_rate;
            s_monster_tmep_evasion_rate = 0.0;
            monster->current_toughness = monster->max_toughness;
            log_monster_recovers(monster->name);
        }
        Sleep(1000);
        return BATTLE_RESULT_ONGOING;
    }

    bool player_evaded = s_check_evasion(player->evasion_rate);

    if (player->choice_hero == HERO_COUNTER && player->is_counter) {
        player_evaded = false;
    }

    if (player_evaded) {
        log_evaded(player->name, monster->name);
    }
    else {
        int final_damage = 0;
        int monster_damage = monster->attack;

		double damage_multiplier = 1.0;
        bool used_reduction = false;
        if (player->damage_reduction_mode && (double)player->current_hp / player->max_hp <= 0.30) {
            used_reduction = true;
			damage_multiplier = 0.7; // 피해 감소 모드 활성화
        }

        s_apply_damage(monster_damage, damage_multiplier, player->defence_rate, 0.0, &player->current_hp, &final_damage);
        log_monster_attack(player, monster, final_damage);

        if (used_reduction) {
            log_damage_reduction_effect_used();
        }

        PlaySound(TEXT("BGM/SoundEffect/monster_attack.wav"), NULL, SND_ASYNC);


        for (int i = 0; i < 2; i++) {
            UI_static_main_box(COLOR_LIGHTRED);
            Sleep(50);
            UI_static_main_box(COLOR_WHITE);
            Sleep(50);
        }
    }

    Sleep(1000);
    if (player->current_hp <= 0) {
        if (player->set_effect_id == SET_EFFECT_NONE && player->dead_count == 1) {
			log_dead_effect_used();
			player->dead_count = 0; // 이 상태에서 죽으면 1회 사망 횟수 사라짐
            player->current_hp = player->max_hp; // 플레이어가 죽었을 때 최대체력으로 초기화
            player->attack += 500;
            return BATTLE_RESULT_ONGOING;
        } 
        else {
            return BATTLE_RESULT_MONSTER_WIN;
        }
    }

    if (player->choice_hero == HERO_COUNTER && player->is_counter) {
        s_battle_logic(player, monster, player->is_counter);
        player->is_counter = false;
        if (monster->current_hp <= 0) return BATTLE_RESULT_PLAYER_WIN;
    }

    return BATTLE_RESULT_ONGOING;
}
