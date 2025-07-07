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
#include "inventory.h"
#include "item.h"
#include "new_ui.h"
#include "UI_controller.h"

#define ESC 27
#define TOTAL_STAGE 12

int main(void) 
{
	hide_cursor();

    // 랜덤 시드 및 콘솔 크기 설정
    srand((unsigned int)time(NULL));
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT windowSize = { 0, 0, 176, 30 };
    SetConsoleWindowInfo(hOut, TRUE, &windowSize);
    COORD bufferSize = { 177, 300 };
    SetConsoleScreenBufferSize(hOut, bufferSize);

    // 현재 UI 상태
	int current_UI_state = UI_STATE_TITLE;
    // 현재 배틀 상태
	int current_battle_state = BATTLE_STATE_ATTACK;

    static_draw_main_box();

    while (1) {
        static_draw_title();
        // 1) 메인 메뉴 그리기
		title_control(0);
        int key = _getch();

        if (current_UI_state == UI_STATE_TITLE) 
            current_UI_state = title_control(key); 

        if (current_UI_state == UI_STATE_BATTLE) 
        {
            int currentStage = 1;
            player_t player;
            monster_t monster;

            char* inputName = draw_create_player_name_ui();

            strncpy(player.name, inputName, sizeof(player.name) - 1);
            player.name[sizeof(player.name) - 1] = '\0';

            init_player(&player, player.name);
            init_monster(&monster, currentStage);
            init_inventory();

            free(inputName);

            clean_all_display();
            static_draw_battle_box();
            while (1) {
                // 처음 한번 출력
                //battle_control(currentStage, &player, &monster, 0);
                int key = _getch();
                if (current_UI_state == UI_STATE_BATTLE)
					current_battle_state = battle_control(currentStage, &player, &monster, key);

                if (current_battle_state == BATTLE_STATE_ATTACK) {
                    battle_result_t result = process_battle_turn(&player, &monster);
                    if (result == BATTLE_RESULT_PLAYER_WIN) {
                        battle_control(currentStage, &player, &monster, 0);
                        currentStage++;
                        init_monster(&monster, currentStage);
                    }
                    else if (result == BATTLE_RESULT_MONSTER_WIN) {
                        battle_control(currentStage, &player, &monster, 0);
                    }
                    else if (result == EVASION_PLAYER_MONSTER) {
                        battle_control(currentStage, &player, &monster, 0);
                    }
                    else if (result == EVASION_PLAYER) {
                        battle_control(currentStage, &player, &monster, 0);
                    }
                    else if (result == EVASION_MONSTER) {
                        battle_control(currentStage, &player, &monster, 0);
                    }
                    else {
                        battle_control(currentStage, &player, &monster, 0);
                    }
                }
                else if (current_battle_state == BATTLE_STATE_INVENTORY) {
                    // 인벤토리 열기
                    draw_inventory_ui(&player);
                    _getch();
					// draw_battle_ui(currentStage, &player, &monster, "NULL");
                    continue;
				}
                else if (current_battle_state == BATTLE_STATE_EXTORTION) {
					// TODO: 강탈 기능 구현
                }
            }

        }// 임시
        else if (current_UI_state == UI_STATE_SETTING) {
            gotoxy(5, 5);
            printf("[ 옵션 ]\n");
            gotoxy(5, 7);
            printf("1) 사운드 ON/OFF  2) 화면 크기  3) 뒤로가기 (Press any key)");
            _getch();
			current_UI_state = UI_STATE_TITLE; // 옵션 설정 후 타이틀로 돌아가기
        }
    }
    return 0;
}
