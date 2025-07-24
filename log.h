// log.h 
#pragma once
#include "inout.h"
#include "utils.h"
#include "player.h"
#include "monster.h"

#include "UI_info.h"
#include "UI_cleaner.h"

#define LOG_MAX_LINES 5
#define LOG_BUFFER_SIZE 60
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