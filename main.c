#define _CRT_SECURE_NO_WARNINGS

// main.c
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>

#include "pair.h"
#include "battle.h"
#include "battle_eval.h"
#include "player.h"
#include "monster.h"
#include "item.h"
#include "UI.h"

#define ESC 27
#define TOTAL_STAGE 12

int main(void) {
    // 랜덤 시드 및 콘솔 크기 설정
    srand((unsigned int)time(NULL));
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT windowSize = { 0, 0, 176, 30 };
    SetConsoleWindowInfo(hOut, TRUE, &windowSize);
    COORD bufferSize = { 177, 300 };
    SetConsoleScreenBufferSize(hOut, bufferSize);

    while (1) {
        // 1) 메인 메뉴 그리기
        draw_main_ui();
        char menuKey = _getch();

        if (menuKey == '1') {
            int currentStage = 1;
            player_t player;
            monster_t monster;

            char* inputName = draw_create_player_name_ui();
            strncpy(player.name, inputName, sizeof(player.name) - 1);
            player.name[sizeof(player.name) - 1] = '\0';
            free(inputName);

            init_player(&player, player.name);
            init_monster(&monster, currentStage);

            while (1) {
                system("cls");
                draw_battle_ui(currentStage, &player, &monster, "NULL");

                char key = _getch();
                if (key == 'q' || key == ESC) {
                    break;
                }

                if (key == 'i' || key == 'I') {
                    // 인벤토리 열기
                    draw_inventory_ui();
                    _getch();
					draw_battle_ui(currentStage, &player, &monster, "NULL");
                    continue;
				}

                if (key == 'f') {
                    battle_result_t result = process_battle_turn(&player, &monster);
                    if (result == BATTLE_RESULT_PLAYER_WIN) {
                        draw_battle_ui(currentStage, &player, &monster, "Player wins!");
                        currentStage++;
                        init_monster(&monster, currentStage);
                    }
                    else if (result == BATTLE_RESULT_MONSTER_WIN) {
                        draw_battle_ui(currentStage, &player, &monster, "Monster wins!");
                    }
                    else if (result == EVASION_PLAYER_MONSTER) {
                        draw_battle_ui(currentStage, &player, &monster, "Player and monster evaded!");
                    }
                    else if (result == EVASION_PLAYER) {
                        draw_battle_ui(currentStage, &player, &monster, "Player evaded the attack!");
                    }
                    else if (result == EVASION_MONSTER) {
                        draw_battle_ui(currentStage, &player, &monster, "Monster evaded the attack!");
                    }
                    else {
                        draw_battle_ui(currentStage, &player, &monster, "Battle ongoing...");
                    }
                    Sleep(1000);
                }
            }

        }
        // 임시
        else if (menuKey == '2') {
            system("cls");
            gotoxy(5, 5);
            printf("[ 옵션 ]\n");
            gotoxy(5, 7);
            printf("1) 사운드 ON/OFF  2) 화면 크기  3) 뒤로가기 (Press any key)");
            _getch();

        }
        else if (menuKey == '3' || menuKey == ESC) {
            break;
        }
    }

    return 0;
}
