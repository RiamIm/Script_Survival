// log.h 
#pragma once
#include "inout.h"
#include "utils.h"
#include "player.h"
#include "monster.h"

#include "UI_info.h"
#include "UI_cleaner.h"

#define LOG_MAX_LINES 5
#define LOG_BUFFER_SIZE 55
#define LOG_START_X   42
#define LOG_START_Y   21

void log_buffer_clear(void);
void log_player_focus(player_t* player);
void log_player_attack(player_t* player, monster_t* monster, int damage, int break_damage);
void log_monster_attack(player_t* player, monster_t* monster, int damage);
void log_evaded(const char* defender_name, const char* attacker_name);