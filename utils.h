// utils.h
#pragma once
#include <Windows.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "pair.h"

#define ESC 27
#define TOTAL_STAGE 12

#define COLOR_BORDER COLOR_LIGHTGRAY
#define COLOR_TITLE COLOR_YELLOW
#define COLOR_SELECT_MENU COLOR_WHITE
#define COLOR_DECOR COLOR_LIGHTBLUE
#define COLOR_SLOGAN COLOR_YELLOW
#define COLOR_DEFAULT COLOR_DARKGRAY
#define COLOR_DEFAULT_TEXT COLOR_WHITE
#define COLOR_STAT_UP COLOR_LIGHTGREEN
#define COLOR_STAT_DOWN COLOR_LIGHTRED
#define COLOR_CURRENT_LOG COLOR_WHITE
#define COLOR_HISTORY_LOG COLOR_DARKGRAY

#define TRUE 1
#define FALSE 0

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
	COLOR_WHITE
} color_t;

void utils_gotoxy(int x, int y);
void utils_hide_cursor(void);
void utils_set_color(int new_color);
//void utils_force_disable_ime(void);
//void utils_force_english_input(void);