// UI_static.c
#define _CRT_SECURE_NO_WARNINGS
#include "UI_static.h"

void UI_static_main_box(void) 
{
	for (int y = 1; y < HEIGHT; y++) {
		utils_gotoxy(0, y);        printf("|");
		utils_gotoxy(WIDTH, y);    printf("|");
	}

	for (int x = 0; x <= WIDTH; x++) {
		utils_gotoxy(x, 0);        printf("=");
		utils_gotoxy(x, HEIGHT - 1);   printf("=");
	}
}

void UI_static_title(void)
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

	utils_set_color(COLOR_TITLE);
	for (int i = 0; i < art_lines; i++) {
		int len = (int)strlen(title_art[i]);
		int tx = (WIDTH - len) / 2;
		utils_gotoxy(tx, start_y + i);
		printf("%s", title_art[i]);
	}

	const char* slogan = ">>> 생존을 위한 운빨 전쟁이 시작된다 <<<";
	utils_gotoxy((WIDTH - (int)strlen(slogan)) / 2, start_y + art_lines + 1);
	utils_set_color(COLOR_SLOGAN);
	printf("%s", slogan);
}

void UI_static_battle_box(void)
{
	for (int x = 1; x < WIDTH - 1; x++) {
		utils_gotoxy(x, 18); putchar('=');
		utils_gotoxy(x, 20); putchar('=');
	}

	for (int y = 19; y < 27; y++) {
		if (y == 20) continue; // 20행은 비워둠
		utils_gotoxy(37, y); printf("|");
		utils_gotoxy(111, y); printf("|");
	}

	utils_gotoxy(14, 19); printf("플레이어 행동");
	utils_gotoxy(124, 19); printf("플레이어 스텟");
}

void UI_static_inventory_box(void)
{
	for (int i = 1; i < WIDTH - 1; i++) {
		utils_gotoxy(i, 4); putchar('=');
	}

	for (int y = 1; y < 4; y++) {
		utils_gotoxy(9, y); putchar('|');
		utils_gotoxy(53, y); putchar('|');
		utils_gotoxy(97, y); putchar('|');
		utils_gotoxy(141, y); putchar('|');
	}

	for (int i = 1; i < WIDTH - 1; i++) {
		utils_gotoxy(i, 18); putchar('=');
		utils_gotoxy(i, 20); putchar('=');
	}

	for (int y = 5; y < 27; y++) {
		if (y == 18 || y == 20) continue;
		utils_gotoxy(75, y); putchar('|');
	}

	for (int y = 19; y < 27; y++) {
		if (y == 20) continue;
		utils_gotoxy(37, y); putchar('|');
		utils_gotoxy(111, y); putchar('|');
	}

	utils_gotoxy(12, 19); printf("착용 중인 무기");
	utils_gotoxy(49, 19); printf("착용 중인 방어구");
	utils_gotoxy(91, 19); printf("메세지");
	utils_gotoxy(123, 19); printf(" 플레이어 스텟");
}

void UI_static_shop_box(void)
{
	for (int i = 1; i < WIDTH - 1; i++) {
		utils_gotoxy(i, 4); putchar('=');
	}

	for (int y = 1; y < 4; y++) {
		utils_gotoxy(9, y); putchar('|');
		utils_gotoxy(53, y); putchar('|');
		utils_gotoxy(97, y); putchar('|');
	}

	for (int i = 1; i < WIDTH - 1; i++) {
		utils_gotoxy(i, 18); putchar('=');
	}

	for (int x = 1; x < 9; x++) {
		utils_gotoxy(x, 9); putchar('=');
		utils_gotoxy(x, 13); putchar('=');
	}

	

	for (int y = 5; y < 18; y++) {
		utils_gotoxy(79, y); putchar('|');
		utils_gotoxy(9, y); putchar('|');
	}	

	for (int y = 19; y < 27; y++) {
		utils_gotoxy(49, y); putchar('|');
		utils_gotoxy(101, y); putchar('|');
	}

	utils_gotoxy(3, 2); printf("◁---");
	utils_gotoxy(38, 2); printf("무기ㆍ방어구");
	utils_gotoxy(111, 2); printf("소비아이템");

	utils_gotoxy(4, 7); printf("숲");
	utils_gotoxy(3, 11); printf("사막");
	utils_gotoxy(3, 15); printf("설원");

	utils_gotoxy(23, 25); printf("구매");
	utils_gotoxy(74, 25); printf("판매");
	utils_gotoxy(124, 25); printf("코인");
}