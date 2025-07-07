#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#include <time.h>

#include "inventory.h"
#include "UI.h"
#include "utils.h"

#define WIDTH 151
#define HEIGHT 27

static void print_colored_stat(const char* label, int value, int x, int y) 
{
    gotoxy(x, y);
    printf("%s ", label);

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (value > 0)
        SetConsoleTextAttribute(hConsole, COLOR_RED);
    else if (value < 0)
        SetConsoleTextAttribute(hConsole, COLOR_LIGHTBLUE);

    printf("%+d", value);

    SetConsoleTextAttribute(hConsole, COLOR_DEFAULT_TEXT);
}

void draw_title_ui(int title_state)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	system("cls");

    set_color(COLOR_BORDER);
    for (int y = 1; y < HEIGHT; y++) {
        gotoxy(0, y);        putchar('|');
        gotoxy(WIDTH, y);    putchar('|');
    }

    for (int x = 0; x <= WIDTH; x++) {
        gotoxy(x, 0);        putchar('=');
        gotoxy(x, HEIGHT);   putchar('=');
    }

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

    const char* menu[] = {
        "1. 시작",
        "2. 옵션",
        "3. 종료"
    };
    set_color(COLOR_DEFAULT);
    for (int i = 0; i < 3; i++) {
        int mx = (WIDTH - (int)strlen(menu[i])) / 2;
        gotoxy(mx, 15 + i * 2);
        
        if (title_state== i) {
            set_color(COLOR_MENU);
            printf("%s", menu[i]);
            set_color(COLOR_DEFAULT);
        }
        else
        {
            printf("%s", menu[i]);
        }      
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
    (void)_getch(); // ← 여기서 IME 입력 종료됨

    return name;
}

void draw_battle_ui(int currentStage, player_t* player, monster_t* monster, int title_state)
{
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
        }
        else {
            gotoxy(start_point + i, 2); printf("□");
        }
    }

    for (int i = 0; i < 13; i++)
    {
        gotoxy(22, 4 + i);
        printf("%s", monster->image[i]);
    }

    const char* menu[] = {
        "1. Attack",
        "2. Item",
        "3. extortion"
    };

    set_color(COLOR_DEFAULT);

    int x = 2;
    int y = 22;
    for (int i = 0; i < 3; i++) {    
        gotoxy(x, y);

        if (title_state == i) {
            set_color(COLOR_MENU);
            printf("%s", menu[i]);
            set_color(COLOR_DEFAULT);
        }
        else
        {
            printf("%s", menu[i]);
        }
        y++;
    }

	set_color(COLOR_DEFAULT_TEXT);
    gotoxy(14, 19); printf("플레이어 행동");

    gotoxy(60, 19);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 7);
    printf(" 지역 : ");
    SetConsoleTextAttribute(hConsole, 10);
    printf("숲");
    SetConsoleTextAttribute(hConsole, 7);
    printf("\t\t몬스터 : ");
    SetConsoleTextAttribute(hConsole, 12);
    printf("%s", monster->name);
    SetConsoleTextAttribute(hConsole, 7);

    gotoxy(123, 19); printf(" 플레이어 스텟");

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



void print_colored_float_stat(const char* label, float value, int x, int y) {
    gotoxy(x, y);
    printf("%s: ", label);

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (value > 0.0f)
        SetConsoleTextAttribute(hConsole, COLOR_RED);
    else if (value < 0.0f)
        SetConsoleTextAttribute(hConsole, COLOR_LIGHTBLUE);

    printf("%+.1f%", value * 100);
    //printf("%+.2f%%", value * 100); // 부호 + 소수점 2자리 + 퍼센트

    SetConsoleTextAttribute(hConsole, COLOR_DEFAULT_TEXT);
}


void draw_inventory_ui(player_t* player)
{
    system("cls");

    int width = 150;

    for (int i = 0; i <= width; i++) {
        gotoxy(i, 4); putchar('=');
    }


    for (int y = 1; y < 4; y++) {
        gotoxy(46, y); putchar('|');
        gotoxy(103, y); putchar('|');
    }


    for (int y = 5; y < 27; y++) {
        gotoxy(75, y); putchar('|');
    }

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
        gotoxy(38, y); putchar('|');
        gotoxy(111, y); putchar('|');
    }

    // 1~4번 아이템 설명
    // 디버깅용 gotoxy(2, 6 + i * 2);
    // 무기 인벤퇼
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        
        int x = (i < 5) ? 2 : 38;
        int y = (i < 5) ? 6 + i * 2 : 6 + (i - 5) * 2;

        gotoxy(x, y);
        printf("* ");
      
        int color = (weapon_inventory[i].count == 0) ? COLOR_LIGHTGRAY : COLOR_WHITE;


        

    }


    // 아이템에 대한 설명

    gotoxy(21, 2); printf("무기");
    gotoxy(72, 2); printf("방어구");
    gotoxy(122, 2); printf("소비 아이템");

    gotoxy(12, 19); printf("착용 중인 무기");
    gotoxy(2, 23); printf("1. 검");

    gotoxy(48, 19); printf("착용 중인 방어구");
    gotoxy(40, 23); printf("2. 갑바");

    gotoxy(90, 19); printf("메세지");

    gotoxy(77, 21); printf("무기가 장착되었습니다.");
    gotoxy(77, 22); printf("무기를 변경하시겠습니까? [Y/N]");


    gotoxy(123, 19); printf(" 플레이어 스텟");
    gotoxy(113, 21);  printf("Player: %s", player->name);
    gotoxy(113, 22);  printf("HP   : %d / %d", player->current_hp, player->max_hp);
    gotoxy(113, 23);  printf("ATK  : %d", player->attack);
    gotoxy(113, 24);  printf("SPD  : %d", player->speed);
    gotoxy(113, 25);  printf("EVA  : %.2f%%", player->evasion_rate * 100);
    gotoxy(113, 26);  printf("DEF  : %.2f%%", player->defence_rate * 100);

}

#pragma once

