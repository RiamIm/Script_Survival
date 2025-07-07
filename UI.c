#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#include <time.h>
#include "UI.h"  // 헤더만 추가하면 됨
#include "monster.h"

#define WIDTH 151
#define HEIGHT 27

void draw_background_stars(int count) 
{
    set_color(COLOR_STAR);
    const char stars[] = { '.', '*', '+', '.' };
    for (int i = 0; i < count; i++) {
        int x = rand() % (WIDTH - 2) + 1;
        int y = rand() % (HEIGHT - 2) + 1;

        if (y >= 2 && y <= 13) continue;
        if (y >= 15 && y <= 20) continue;

        gotoxy(x, y);
        putchar(stars[rand() % 4]);
    }
}

void draw_main_ui(void) 
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    system("cls");
    srand((unsigned int)time(NULL));

    set_color(COLOR_BORDER);
    for (int y = 1; y < HEIGHT; y++) {
        gotoxy(0, y);        putchar('|');
        gotoxy(WIDTH, y);    putchar('|');
    }

    for (int x = 0; x <= WIDTH; x++) {
        gotoxy(x, 0);        putchar('=');
        gotoxy(x, HEIGHT);   putchar('=');
    }

    draw_background_stars(80);

    set_color(COLOR_TITLE);
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

    const char* menu[] = {
        "1. 시작",
        "2. 옵션",
        "3. 종료"
    };
    set_color(COLOR_MENU);
    for (int i = 0; i < 3; i++) {
        int mx = (WIDTH - (int)strlen(menu[i])) / 2;
        gotoxy(mx, 15 + i * 2);
        printf("%s", menu[i]);
    }

	set_color(COLOR_DEFAULT_TEXT);
    gotoxy(0, HEIGHT + 2);
}


char* draw_create_player_name_ui(void) 
{
    system("cls");
    gotoxy(0, 0);
    printf("플레이어 이름을 입력하세요 (최대 24자, 공백 허용): ");

    fflush(stdin);

    char* name = malloc(25);
    if (!name) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    if (fgets(name, 25, stdin)) {
        name[strcspn(name, "\r\n")] = '\0';
    }
    else {
        name[0] = '\0';
    }

    printf("\n입력이 완료되었습니다. [Enter] 키를 누르면 게임이 시작됩니다...\n");
    _getch(); // ← 여기서 IME 입력 종료됨

    return name;
}

void draw_battle_ui(int currentStage, player_t *player, monster_t *monster, char* messege)
{
    //system("cls");

    for (int i = 0; i <= WIDTH; i++) {
        gotoxy(i, 0); putchar('=');
        gotoxy(max(1, i), 18); putchar('=');
        gotoxy(max(1, i), 20); putchar('=');
        gotoxy(i, 27); putchar('=');
    }

    for (int y = 1; y < 27; y++) {
        gotoxy(0, y); putchar('|');
        gotoxy(WIDTH, y); putchar('|');
    }
    for (int y = 19; y < 27; y++) {
        if (y == 20) continue;
        gotoxy(40, y); putchar('|');
        gotoxy(111, y); putchar('|');
    }

    int full_hp = monster->max_hp / 5;
	int current_hp = monster->current_hp / 5;
	int start_point = 72 - (full_hp / 2);
    for (int i = 0; i < full_hp; i++) {
        if (i < current_hp) {
            gotoxy(start_point + i, 2); printf("■");
        } else {
            gotoxy(start_point + i, 2); printf("□");
        }
    }
   
    for (int i = 0; i < 13; i++)
    {
        gotoxy(22, 4 + i);
		printf("%s", monster->image[i]);
    }
   
    gotoxy(2, 21); printf("1. Attack");
    gotoxy(2, 22); printf("2. Item");
    gotoxy(2, 23); printf("3. extortion");

    gotoxy(2, 19); printf("플레이어 행동");

    gotoxy(41, 19);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 7);
    printf(" 지역 : ");
    SetConsoleTextAttribute(hConsole, 10);
    printf("숲");
    SetConsoleTextAttribute(hConsole, 7);
    printf("\t몬스터 : ");
    SetConsoleTextAttribute(hConsole, 12);
    printf("%s", monster->name);
    SetConsoleTextAttribute(hConsole, 7);

    gotoxy(112, 19); printf(" 플레이어 스텟");

    if (messege != "NULL")
    {
        gotoxy(41, 21);
		printf("메세지: %s", messege); 
    }
    else
    {   
        for(int i = 0; i < 40; i++) {
		    gotoxy(41 + i, 21);
            printf(" ");
        }
    }

    gotoxy(113, 21);  printf("Player: %s", player->name);
    gotoxy(113, 22);  printf("HP   : %d / %d", player->current_hp, player->max_hp);
	gotoxy(113, 23);  printf("ATK  : %d", player->attack);
	gotoxy(113, 24);  printf("SPD  : %d", player->speed);
	gotoxy(113, 25);  printf("EVA  : %.2f%%", player->evasion_rate * 100);
	gotoxy(113, 26);  printf("DEF  : %.2f%%", player->defence_rate * 100);

    /*gotoxy(0, 28);
    printf("스테이지 진행: ");
    for (int i = 0; i < 12; i++) {
        if (i < currentStage)
            printf("■ㆍㆍㆍ");
        else
            printf("□ㆍㆍㆍ");
    }

    gotoxy(0, 29);
    printf("[n] 다음 스테이지  [q] 종료\n");
    gotoxy(0, 30);
    printf("Your choice: ");*/
}

void draw_inventory_ui(player_t* player)
{
    system("cls");

    int width = 150;

    for (int i = 0; i <= width; i++) {
        gotoxy(i, 4); putchar('=');
    }


    for (int y = 1; y < 4; y++) {
        gotoxy(40, y); putchar('|');
        gotoxy(111, y); putchar('|');
    }


    for (int y = 5; y < 18; y++) {
        gotoxy(75, y); putchar('|');
    }
    // 1~4번 아이템 설명
    // 디버깅용
    for (int i = 0; i <= width; i++) {
        if (i >= player->inventory_count)
        {
            break;
        }	
        gotoxy(2, 6 + i * 2);
		printf("%d. %s", i + 1, player->inventory[i].name);

        // 아이템 기본 설명
        int len = strlen(player->inventory[i].description);
        gotoxy(110 - (len / 2), 6);  printf("%s", player->inventory[i].description);

        int baseX = 80;
        int line = 8;

        // Attack
        if (player->inventory[i].attack_bonus != 0) {
            gotoxy(baseX, line++);
            printf("Attack: %d", player->inventory[i].attack_bonus);
        }

        // HP
        if (player->inventory[i].hp_bonus != 0) {
            gotoxy(baseX, line++);
            printf("HP: %d", player->inventory[i].hp_bonus);
        }

        // Speed
        if (player->inventory[i].speed_bonus != 0) {
            gotoxy(baseX, line++);
            printf("Speed: %d", player->inventory[i].speed_bonus);
        }

        // Evasion
        if (player->inventory[i].evasion_bonus != 0.0f) {
            gotoxy(baseX, line++);
            printf("Evasion: %.2f%%", player->inventory[i].evasion_bonus * 100);
        }

        // Defence
        if (player->inventory[i].defence_bonus != 0.0f) {
            gotoxy(baseX, line++);
            printf("Defence: %.2f%%", player->inventory[i].defence_bonus * 100);
        }

    }
    // 아이템에 대한 설명
    
    gotoxy(14, 2); printf("무기ㆍ방어구");
    gotoxy(70, 2); printf("소비 아이템");
    gotoxy(128, 2); printf("전리품");

}
#pragma once

