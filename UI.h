#pragma once
#include <windows.h>
#include <stdio.h>
#include "player.h"
#include "monster.h"
#include "pair.h"

typedef enum color {
	COLOR_BLACK,
	COLOR_BLUE,
	COLOR_GREEN,
	COLOR_CYAN,
	COLOR_RED,
	COLOR_MAGENTA,
	COLOR_BROWN,
	COLOR_LIGHTGRAY,
	COLOR_DARKGRAY,
	COLOR_LIGHTBLUE,
	COLOR_LIGHTGREEN,
	COLOR_LIGHTCYAN,
	COLOR_LIGHTRED,
	COLOR_LIGHTMAGENTA,
	COLOR_YELLOW,
	COLOR_WHITE,
};

#define COLOR_BORDER COLOR_LIGHTGRAY
#define COLOR_TITLE COLOR_YELLOW
#define COLOR_MENU COLOR_LIGHTCYAN
#define COLOR_DECOR COLOR_LIGHTBLUE
#define COLOR_SLOGAN COLOR_YELLOW
#define COLOR_STAR COLOR_DARKGRAY
#define COLOR_DEFAULT COLOR_LIGHTGRAY

void gotoxy(int x, int y);
void set_color(int color);
void draw_background_stars(int count);
void draw_main_ui(void);
char* draw_create_player_name_ui(void);
void draw_battle_ui(int currentStage, player_t * player, monster_t *monster, char* messege);
void draw_inventory_ui(void);