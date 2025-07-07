#include "UI_cleaner.h"

void clean_all_display(void)
{
	for (int y = 1; y < HEIGHT; y++) {
		for (int x = 1; x < WIDTH - 1; x++) {
			gotoxy(x, y);	putchar(' ');
		}
	}
}

void clean_monster_display(void)
{
	int monster_display_max_y = 18;
	int moster_display_max_x = 150;
	for (int y = 1; y < monster_display_max_y; y++) {
		for (int x = 1; x < moster_display_max_x; x++) {
			gotoxy(x, y); putchar(' ');
		}
	}
}

void clean_player_stats_display(void) {
	int start_x = 112;
	int start_y = 21;

	for (int y = start_y; y < HEIGHT; y++) {
		for (int x = start_x; x < WIDTH - 1; x++) {
			gotoxy(x, y); putchar(' ');
		}
	}
}