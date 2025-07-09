// log.h
#pragma once
#include "inout.h"
#include "utils.h"

#include "player.h"
#include "monster.h"

#include "UI_info.h"

void log_deal_damage(player_t* player, monster_t* monster, int attack, bool is_player_attack);
void log_take_damage(player_t* player, monster_t* monster, int attack, bool is_monster_attack);
void log_evade(player_t* player, monster_t* monster);