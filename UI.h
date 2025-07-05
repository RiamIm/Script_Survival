#pragma once
#include <windows.h>
#include <stdio.h>
#include "player.h"
#include "monster.h"
#include "pair.h"

void gotoxy(int x, int y);
void draw_main_ui(void);
char* draw_create_player_name_ui(void);
void draw_battle_ui(int currentStage, player_t * player, monster_t *monster, char* messege);
void draw_inventory_ui(void);