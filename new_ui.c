#include "new_ui.h"

void clean_all_display(void)
{
	for (int y = 1; y < HEIGHT; y++) {
		for (int x = 1; x < WIDTH - 1; x++) {
			gotoxy(x, y);	putchar(' ');
		}
	}		
}

void static_draw_main_box(void) 
{
	for (int y = 1; y < HEIGHT; y++) {
		gotoxy(0, y);        printf("|");
		gotoxy(WIDTH, y);    printf("|");
	}

	for (int x = 0; x <= WIDTH; x++) {
		gotoxy(x, 0);        printf("=");
		gotoxy(x, HEIGHT);   printf("=");
	}
}

void static_draw_title(void)
{
	const char* title_art[] = {
		" _____  _____ ______  _____ ______  _____   _____  _   _ ______  _   _  _____  _   _   ___   _     ",
		"/  ___|/  __ \\| ___ \\|_   _|| ___ \\|_   _| /  ___|| | | || ___ \\| | | ||_   _|| | | | / _ \\ | |    ",
		"\\ `--. | /  \\/| |_/ /  | |  | |_/ /  | |   \\ `--. | | | || |_/ /| | | |  | |  | | | |/ /_\\ \\| |    ",
		" `--. \\| |    |    /   | |  |  __/   | |    `--. \\| | | ||    / | | | |  | |  | | | ||  _  || |    ",
		"/\\__/ /| \\__/\\| |\\ \\  _| |_ | |      | |   /\\__/ /| |_| || |\\ \\ \\ \\_/ / _| |_ \\ \\_/ /| | | || |____",
		"\\____/  \\____/\\_| \\_| \\___/ \\_|      \\_/   \\____/  \\___/ \\_| \\_| \\___/  \\___/  \\___/ \\_| |_/\\_____/"
	};

	const int art_lines = sizeof(title_art) / sizeof(title_art[0]);
	const int start_y = 2;

	set_color(COLOR_TITLE);
	for (int i = 0; i < art_lines; i++) {
		int len = (int)strlen(title_art[i]);
		int tx = (WIDTH - len) / 2;
		gotoxy(tx, start_y + i);
		printf("%s", title_art[i]);
	}

	const char* slogan = ">>> 생존을 위한 운빨 전쟁이 시작된다 <<<";
	gotoxy((WIDTH - (int)strlen(slogan)) / 2, start_y + art_lines + 1);
	set_color(COLOR_SLOGAN);
	printf("%s", slogan);
}

void static_draw_battle_box(void)
{
	for (int x = 1; x < WIDTH - 1; x++) {
		gotoxy(x, 18); putchar('=');
		gotoxy(x, 20); putchar('=');
	}

	for (int y = 19; y < 27; y++) {
		if (y == 20) continue; // 20행은 비워둠
		gotoxy(40, y); printf("|");
		gotoxy(111, y); printf("|");
	}

	gotoxy(14, 19); printf("플레이어 행동");
	gotoxy(124, 19); printf("플레이어 스텟");
}

void static_draw_inventory_box(void)
{
	for (int i = 0; i < WIDTH - 1; i++) {
		gotoxy(i, 4); putchar('=');
	}

	for (int y = 1; y < 4; y++) {
		gotoxy(46, y); putchar('|');
		gotoxy(103, y); putchar('|');
	}

	for (int y = 5; y < 27; y++) {
		gotoxy(75, y); putchar('|');
	}

	for (int i = 1; i < WIDTH - 1; i++) {
		gotoxy(i, 18); putchar('=');
		gotoxy(i, 20); putchar('=');
	}

	for (int y = 19; y < 27; y++) {
		if (y == 20) continue;
		gotoxy(38, y); putchar('|');
		gotoxy(111, y); putchar('|');
	}
}