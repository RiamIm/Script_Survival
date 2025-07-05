#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include "UI.h"  // 헤더만 추가하면 됨
#include "monster.h"

#define WIDTH 151
#define HEIGHT 27


void gotoxy(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void draw_main_ui(void) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    system("cls");

    for (int y = 1; y < HEIGHT; y++) {
        gotoxy(0, y);        putchar('|');
        gotoxy(WIDTH, y);    putchar('|');
    }

    for (int x = 0; x <= WIDTH; x++) {
        gotoxy(x, 0);        putchar('=');
        gotoxy(x, HEIGHT);   putchar('=');
    }
    

    const char* title_art[] = {
        " ____                                       __                  ____                                                           ___      ",
        "/\\  _`\\                     __             /\\ \\__              /\\  _`\\                               __                       /\\_ \\     ",
        "\\ \\,\\L\\_\\     ___    _ __  /\\_\\    _____   \\ \\ ,_\\             \\ \\,\\L\\_\\    __  __   _ __   __  __  /\\_\\    __  __     __     \\//\\ \\    ",
        " \\/_\\__ \\    /'___\\ /\\`'__\\\\/\\ \\  /\\ '__`\\  \\ \\ \\/              \\/_\\__ \\   /\\ \\ /\\ \\\\ /\\`'__\\/\\ \\/\\ \\\\ \\/\\ \\ /\\ \\  /'__`\\     \\ \\ \\   ",
        "   /\\ \\L\\ \\ /\\ \\__/ \\ \\ \\/  \\ \\ \\ \\ \\L\\ \\  \\ \\ \\_               /\\ \\L\\ \\ \\ \\ \\_\\ \\\\ \\ \\/ \\ \\ \\_/ | \\ \\ \\ \\ \\ \\_/ |/\\ \\L\\.\\_    \\_\\ \\_ ",
        "   \\ `\\____\\\\ \\____\\ \\ \\_\\   \\ \\_\\ \\ \\ ,__/   \\ \\__\\              \\ `\\____\\ \\ \\____/ \\ \\_\\  \\ \\___/   \\ \\_\\ \\ \\___/ \\ \\__/.__\\   /\\____\\",
        "    \\/_____/ \\/____/  \\/_/    \\/_/  \\ \\ \\/     \\/__/               \\/_____/  \\/___/   \\/_/   \\/__/     \\/_/  \\/__/   \\/__/\\/__/   \\/____/",
        "                                     \\ \\_\\                                                                                              ",
        "                                      \\/_/                                                                                              "
    };
    const int art_lines = sizeof(title_art) / sizeof(title_art[0]);
    const int start_y = 2;

    // 3) ASCII 아트 그리기 (가로 중앙 정렬)
    for (int i = 0; i < art_lines; i++) {
        int len = (int)strlen(title_art[i]);
        int tx = (WIDTH - len) / 2;
        gotoxy(tx, start_y + i);
        printf("%s", title_art[i]);
    }


    
    // 메뉴 옵션 출력
    const char* menu[] = {
        "1. 게임 시작하기",
        "2. 옵션",
        "3. 종료"
    };
    for (int i = 0; i < 3; i++) {
        int mx = (WIDTH - (int)strlen(menu[i])) / 2;
        gotoxy(mx, 15 + i * 2);
        printf("%s", menu[i]);
    }

    // 커서를 메뉴 아래로 이동
    gotoxy(0, HEIGHT + 2);
}


char* draw_create_player_name_ui(void) {
    system("cls");
    gotoxy(0, 0);
    printf("플레이어 이름을 입력하세요 (최대 24자, 공백 허용): ");

    // 윈도우즈 MSVC에서는 stdin 버퍼 비우기
    fflush(stdin);

    // 입력 버퍼 할당 (24자 + 널)
    char* name = malloc(25);
    if (!name) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    if (fgets(name, 25, stdin)) {
        // 윈도우즈의 "\r\n" 혹은 유닉스의 "\n" 제거
        name[strcspn(name, "\r\n")] = '\0';
    }
    else {
        name[0] = '\0';
    }
    return name;
}

void draw_battle_ui(int currentStage, player_t *player, monster_t *monster, char* messege) {
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
	gotoxy(113, 25);  printf("EVA  : %.1f%%", player->evasion_rate * 100);
	gotoxy(113, 26);  printf("DEF  : %.1f%%", player->defence_rate * 100);

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

void draw_inventory_ui(void) {
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
    gotoxy(2, 6);  printf("1. 설명");
    gotoxy(2, 8);  printf("2. 설명");
    gotoxy(2, 10);  printf("3. 설명");
    gotoxy(2, 12);  printf("4. 설명");

    gotoxy(103, 6);  printf("아이템 대한 설명");

    // 아이템에 대한 설명
    gotoxy(80, 8);  printf("아이템 대한 설명");
    gotoxy(80, 9);  printf("아이템 대한 설명");
    gotoxy(80, 10);  printf("아이템 대한 설명");
    gotoxy(80, 11);  printf("아이템 대한 설명");


    gotoxy(14, 2); printf("무기ㆍ방어구");
    gotoxy(70, 2); printf("소비 아이템");
    gotoxy(128, 2); printf("전리품");

}
#pragma once

