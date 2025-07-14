// UI_cleaner.c
#define _CRT_SECURE_NO_WARNINGS
#include "UI_cleaner.h"

void UI_cleaner_all_display(void)
{
	for (int y = 1; y < HEIGHT - 1; y++) {
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

	for (int y = start_y; y < HEIGHT - 1; y++) {
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
	for (int y = 5; y < 18; y++) {
		for (int x = 10; x < 75; x++) {
			utils_gotoxy(x, y);  putchar(' ');
		}
	}
}

void UI_cleaner_inventory_item_description(void)
{
	for (int y = 5; y < 18; y++) {
		for (int x = 76; x < WIDTH - 1; x++) {
			utils_gotoxy(x, y);  putchar(' ');
		}
	}
}

void UI_cleaner_sub_menu(void)
{
	for (int y = 5; y < 18; y++) {
		for (int x = 1; x < 10; x++) {
			utils_gotoxy(x, y); putchar(' ');
		}
	}
}

void UI_cleaner_player_info(void)
{
	for (int y = 22; y < HEIGHT - 1; y++) {
		for (int x = 120; x < WIDTH - 1; x++) {
			utils_gotoxy(x, y); putchar(' ');
		}
	}
}

void UI_cleaner_current_weapon_box(void)
{
	for (int x = 1; x < 37; ++x)
	{
		utils_gotoxy(x, 23); putchar(' ');
	}
}

void UI_cleaner_current_armor_box(void)
{
	for (int x = 38; x < 75; ++x)
	{
		utils_gotoxy(x, 23); putchar(' ');
	}
}