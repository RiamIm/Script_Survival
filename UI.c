#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include "UI.h"  // 헤더만 추가하면 됨
#include "monster.h"


void gotoxy(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void DrawUI(int currentStage, player_t *player, monster_t *monster, char* messege) {
    //system("cls");

    int width = 151;

    for (int i = 0; i <= width; i++) {
        gotoxy(i, 0); putchar('=');
        gotoxy(max(1, i), 18); putchar('=');
        gotoxy(max(1, i), 20); putchar('=');
        gotoxy(i, 27); putchar('=');
    }

    for (int y = 1; y < 27; y++) {
        gotoxy(0, y); putchar('|');
        gotoxy(151, y); putchar('|');
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
#pragma once

