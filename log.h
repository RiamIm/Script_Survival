// log.h
#pragma once
#include "inout.h"
#include "utils.h"

#include "player.h"
#include "monster.h"

#include "UI_info.h"

#define LOG_MAX_LINES 6
#define LOG_BUFFER_SIZE 55

// 로그창 시작 좌표 (UI_info.h 에 정의된 매크로가 있으면 그걸로 교체하세요)
#define LOG_START_X   42
#define LOG_START_Y   21

static char* s_log_buffer[LOG_MAX_LINES]; // 로그 버퍼

static void s_print_typing_anim(const char* str, int delay_ms);
static void s_log_print_buffer(void);
static void s_log_push_line(void);

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