// log.h 
#pragma once
#include "inout.h"
#include "utils.h"
#include "player.h"
#include "monster.h"

#include "UI_info.h"
#include "UI_cleaner.h"

#define LOG_MAX_LINES 6
#define LOG_BUFFER_SIZE 70
#define LOG_START_X   42
#define LOG_START_Y   21

void log_buffer_clear(void);

void log_player_counter_ready(player_t* player);
void log_player_counter_success(player_t* player, monster_t* monster, int final_damage, int break_damage, bool is_critical);
void log_player_attack(player_t* player, monster_t* monster, int damage, int break_damage, bool is_critical, int break_extra_damage_dealt);
void log_monster_attack(player_t* player, monster_t* monster, int damage);
void log_evaded(const char* defender_name, const char* attacker_name);

void log_skill_used(player_t* player, int self_damage);
void log_life_steal(player_t* player, int heal_point);

void log_monster_groggy(const char* monster_name);
void log_monster_stunned(const char* monster_name);
void log_monster_recovers(const char* monster_name);

void log_auto_heal(player_t* player, int heal_point);

void log_drop_item(player_t* player, equipment_t item[][ITEM_COUNT], int rarity, int index);

void log_drop_coin(player_t* player, int coin);

void log_goto_store(void);

void log_dead_effect_used(void);

void log_damage_reduction_effect_used(void);

void log_field_effect_on(void);
void log_field_effect_off(void);

void log_field_effect_swap_action_value(void);
void log_field_effect_player_damage(player_t* player, int damage);
void log_field_effect_monster_damage(monster_t* monster, int damage);
void log_filed_effect_swap_attack(void);
void log_field_effect_zero_evasion(void);
void log_field_effect_attack_increase(void);
void log_field_effect_heal(void);
void log_field_effect_blood_for_power(const char* name, int sum_attack);
void log_field_effect_power_attack(const char* name);

void log_prologue(void);
void log_chapter_2(void);
void log_chapter_3(void);
void log_chapter_4(void);

void log_monster_use_skill(monster_t* monster, int type);

void log_fianl_monster_use_skill(monster_t* monster);

void log_roar_damage(player_t* player, int damage);

void log_final_monster_after_skill(monster_t* monster);

void log_select_rest(void);

void log_select_store(void);

void log_infinite_mode_start(void);

void log_field_effect_off(void);

void log_run(void);

void log_legacy(void);
