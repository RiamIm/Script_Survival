// log.h
#pragma once
#include "inout.h"
#include "utils.h"

#include "player.h"
#include "monster.h"

#include "UI_info.h"

#define LOG_MAX_LINES 5
#define LOG_BUFFER_SIZE 55

char* log_buffer[LOG_MAX_LINES]; // 로그 버퍼

void s_log_push_line(void);

void log_buffer_clear(void);

void log_player_turn(player_t* player);
void log_player_attack(player_t* player, monster_t* monster, int attack);
void log_player_skill(player_t* player, monster_t* monster, int attack);
void log_player_evade(player_t* player, monster_t* monster);

void log_monster_turn(monster_t* monster);
void log_monster_attack(player_t* player, monster_t* monster, int attack);
void log_monster_skill(player_t* player, monster_t* monster, int attack);
void log_monster_evade(player_t* player, monster_t* monster);

void log_deal_damage(player_t* player, monster_t* monster, int attack, bool is_player_attack);
void log_take_damage(player_t* player, monster_t* monster, int attack, bool is_monster_attack);

void log_evade(player_t* player, monster_t* monster);