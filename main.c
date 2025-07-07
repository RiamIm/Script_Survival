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
#include "UI_cleaner.h"
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

    int currentStage = 1;
    player_t player;
    monster_t monster;
    char* input_name = NULL;

    static_draw_main_box();

    while (true) {
        static_draw_title();
        // 1) 메인 메뉴 그리기
        title_control(0);
        int key = _getch();

        if (current_UI_state == UI_STATE_TITLE)
            current_UI_state = title_control(key);

        if (current_UI_state == UI_STATE_CREATE_PLAYER_NAME)
        {
            input_name = draw_create_player_name_ui();

            strncpy(player.name, input_name, sizeof(player.name) - 1);
            player.name[sizeof(player.name) - 1] = '\0';
            free(input_name);

			current_UI_state = UI_STATE_BATTLE; // 플레이어 이름 설정 후 배틀 상태로 전환
            break;
        }// 임시
        else if (current_UI_state == UI_STATE_SETTING) {
			clean_all_display();
            gotoxy(5, 5);
            printf("[ 옵션 ]\n");
            gotoxy(5, 7);
            printf("1) 사운드 ON/OFF  2) 화면 크기  3) 뒤로가기 (Press any key)");
            _getch();
            clean_all_display();
            current_UI_state = UI_STATE_TITLE; // 옵션 설정 후 타이틀로 돌아가기
        }
    }

    // 초기화
    init_player(&player, player.name);
    init_monster(&monster, currentStage);
    init_inventory();

    while (1)
    {
        if (current_UI_state == UI_STATE_BATTLE)
        {
            clean_all_display();
            static_draw_battle_box();
            while (true)
            {
				draw_monster_info_box(&monster);
				draw_player_info_box(&player);
                // 일단 출력
                battle_control(0);
                int key = _getch();

                current_battle_state = battle_control(key);


                if (current_battle_state == BATTLE_STATE_ATTACK)
                {
                    // 배틀 시스템 구현
					battle_result_t result = process_battle_turn(&player, &monster);
				}
                else if (current_battle_state == BATTLE_STATE_INVENTORY)
                {
                    current_UI_state = UI_STATE_INVENTORY; // 인벤토리 상태로 전환
                    break; // 배틀 상태에서 벗어나 인벤토리로 이동
                }
                else if (current_battle_state == BATTLE_STATE_EXTORTION)
                {
                    // extortion logic here
				}
            }

        }

        if (current_UI_state == UI_STATE_INVENTORY)
        {
			clean_all_display();
            static_draw_inventory_box();
            while (true)
            {
                // todo
            }
        }
    }

    return 0;
}
