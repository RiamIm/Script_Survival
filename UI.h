#pragma once
#include <windows.h>
#include <stdio.h>
#include "item.h"
#include "player.h"
#include "monster.h"
#include "pair.h"
#include "utils.h"
#include "UI_controller.h"

void draw_title_ui(int state);
char* draw_create_player_name_ui(void);
void draw_battle_ui(int currentStage, player_t * player, monster_t *monster, int title_stage);
void draw_inventory_ui(player_t* player);