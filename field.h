// field.h
#pragma once
#include "utils.h"
#include "player.h"
#include "monster.h"


void field_effect_on(player_t* player, monster_t* monster, int type);
void field_effect_off(player_t* player, monster_t* monster, int type);