#pragma once
#include <windows.h>
#include <stdio.h>
#include "player.h"
#include "monster.h"
#include "pair.h"

void gotoxy(int x, int y);
void DrawUI(int currentStage, player_t * player, monster_t *monster, char* messege);