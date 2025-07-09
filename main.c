// main.c
#define _CRT_SECURE_NO_WARNINGS

#pragma once
#include "inout.h"
#include "utils.h"

#include "battle.h"
#include "battle_eval.h"
#include "player.h"
#include "monster.h"
#include "item.h"
#include "inventory.h"

#include "UI_info.h"
#include "UI_control.h"
#include "UI_static.h"
#include "UI_dynamic.h"
#include "UI_cleaner.h"

static int ui_main_state;
static int ui_title_state;
static int ui_setting_state;
static int ui_battle_state;
static int ui_inventory_state;
static int ui_store_state;

int main(void)  
{
    // 커서 지우기
	utils_hide_cursor();

    // 랜덤 시드 및 콘솔 크기 설정
    srand((unsigned int)time(NULL));
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT windowSize = { 0, 0, 176, 30 };
    SetConsoleWindowInfo(hOut, TRUE, &windowSize);
    COORD bufferSize = { 177, 300 };
    SetConsoleScreenBufferSize(hOut, bufferSize);

    UI_control_init(&ui_main_state, &ui_title_state, &ui_setting_state, &ui_battle_state, &ui_inventory_state, &ui_store_state); 
    UI_static_main_box(); 

    bool is_change_ui_main = true; // 처음에는 새로 생성해야 함 (true)
    int currentStage = 1;
    player_t player;
    monster_t monster;
    char* input_name = NULL;

    while (true)
    {
        switch (ui_main_state)
        {
        case UI_STATE_TITLE:
        {
            if (is_change_ui_main) {
                UI_cleaner_all_display();
                UI_static_title();
                is_change_ui_main = false;
            }
            UI_dynamic_title_selection(ui_title_state);

            int key = _getch();
            if (key == EXTENDED_KEY) key = _getch();

            UI_control_title(&ui_main_state, &ui_title_state, key);
            if (ui_main_state != UI_STATE_TITLE) {
                is_change_ui_main = true;
            }
            break;
        }
        case UI_STATE_SETTING:
        {
            UI_cleaner_all_display();
            utils_gotoxy(5, 5);
            printf("[ 옵션 ]\n");
            utils_gotoxy(5, 7);
            printf("1) 사운드 ON/OFF  2) 화면 크기  3) 뒤로가기 (Press any key)");

            _getch(); // 아무 키나 누르면 타이틀로 돌아감 (임시)
            ui_main_state = UI_STATE_TITLE;
            is_change_ui_main = true;
            break;
        }

        case UI_STATE_CREATE_PLAYER_NAME:
        {
            // 이 상태는 키 입력을 기다리지 않고 바로 실행
            UI_cleaner_all_display();
            input_name = UI_dynamic_create_player_name();

            strncpy(player.name, input_name, sizeof(player.name) - 1);
            player.name[sizeof(player.name) - 1] = '\0';
            free(input_name);

            ui_main_state = UI_STATE_BATTLE;
            break;
        }
        } // switch end

        if (ui_main_state == UI_STATE_BATTLE) {
            break;
        }
    }

    // 초기화
    is_change_ui_main = true;
    player_init(&player, player.name);
    monster_init(&monster, currentStage);
    inventory_init();

    while (1)
    {
        if (ui_main_state == UI_STATE_BATTLE)
        {
            if (is_change_ui_main) {
                is_change_ui_main = false;
                UI_cleaner_all_display();
                UI_static_battle_box();
            }
            UI_dynamic_monster_info(&monster);
            UI_dynamic_player_info(&player);
            UI_dynamic_battle_selection(ui_battle_state);

            int key = _getch();
            if (key == EXTENDED_KEY) key = _getch();

            battle_action_t action = UI_control_battle(&ui_battle_state, key);
            UI_dynamic_battle_selection(ui_battle_state);

            switch (action)
            {
            case BATTLE_ACTION_ATTACK:
                battle_process(&player, &monster);
                break;
            case BATTLE_ACTION_INVENTORY:
                is_change_ui_main = true;
                ui_main_state = UI_STATE_INVENTORY;
                break;
            case BATTLE_ACTION_EXTORTION:
                break;
            case BATTLE_ACTION_NONE: 
            default:
                break;
            }
        }

        if (ui_main_state == UI_STATE_INVENTORY)
        {
            if (is_change_ui_main) {
                is_change_ui_main = false;
                UI_cleaner_all_display();
                UI_static_inventory_box();
            }

            UI_dynamic_inventory_info(&player);
            _getch();

            is_change_ui_main = true;
            ui_main_state = UI_STATE_BATTLE;
        }
    }

    return 0;
}