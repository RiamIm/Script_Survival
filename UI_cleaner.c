// UI_cleaner.c
#define _CRT_SECURE_NO_WARNINGS
#include "UI_cleaner.h"

void UI_cleaner_all_display(void)
{
	for (int y = 1; y < HEIGHT; y++) {
		for (int x = 1; x < WIDTH - 1; x++) {
			utils_gotoxy(x, y);	putchar(' ');
		}
	}
}

void UI_cleaner_monster_display(void)
{
	int monster_display_max_y = 18;
	int moster_display_max_x = 150;
	for (int y = 1; y < monster_display_max_y; y++) {
		for (int x = 1; x < moster_display_max_x; x++) {
			utils_gotoxy(x, y); putchar(' ');
		}
	}
}

void UI_cleaner_player_stats_display(void) {
	int start_x = 112;
	int start_y = 21;

	for (int y = start_y; y < HEIGHT; y++) {
		for (int x = start_x; x < WIDTH - 1; x++) {
			utils_gotoxy(x, y); putchar(' ');
		}
	}
}

void UI_cleaner_battle_log(void)
{
	int x = 42;
	int y = 21;

	for (int i = 0; i < 6; i++)
	{
		utils_gotoxy(x, y + i);
		for (int j = 0; j < 60; ++j) {
			printf(" ");
		}
	}
}

void UI_cleaner_inventory_item_list(void)
{
	for (int y = 6; y < 12; y++) {
		utils_gotoxy(1, y);
		for (int x = 0; x < 70; x++) {
			putchar(' ');
		}
	}
}

void UI_cleaner_inventory_item_description(void)
{
	for (int y = 6; y < 12; y++) {
		utils_gotoxy(80, y);
		for (int x = 0; x < 68; x++) {
			putchar(' ');
		}
	}
}