// main.c 
#define _CRT_SECURE_NO_WARNINGS

#include "inout.h"
#include "utils.h"

#include "battle.h"

#include "player.h"
#include "monster.h"

#include "item.h"
#include "inventory.h"
#include "store.h"

#include "UI_info.h"
#include "UI_control.h"
#include "UI_static.h"
#include "UI_dynamic.h"
#include "UI_cleaner.h"

// --- UI 상태 전역 변수 ---
static UI_state_t        ui_main_state;
static title_state_t     ui_title_state;
static setting_state_t   ui_setting_state;
static player_action_t   player_action_state;

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
    
    const int MAX_STAGE_BEFORE_BOSS = 2;
    game_mode_state_t ui_mode_state = MODE_STATE_NORMAL;
    UI_control_init(
        &ui_main_state, &ui_title_state, &player_action_state
    );

    int global_volume = 50;           // 초기 볼륨
    int game_mode = GAME_MODE_NORMAL; // 게임 모드
    bool is_change_ui_main = true;    // 화면 UI를 메인 화면으로 변경해야 하는지 여부를 나타내는 플래그
    int currentStage = 1;             // 현재 게임의 스테이지 번호 (1단계부터 시작)
    char* input_name = NULL;          // 사용자로부터 입력받은 이름을 저장할 포인터 (메모리 할당 전 NULL로 초기화)
    hero_t choice_hero = 0;
    player_t player;
    monster_t monster;

    // --- 이름 생성까지의 루프 ---
    while (true)
    {
        switch (ui_main_state)
        {
        case UI_STATE_TITLE:
        {
            if (is_change_ui_main) {
                UI_cleaner_all_display();
                UI_static_main_box(COLOR_WHITE);
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
            if (is_change_ui_main) {
                UI_cleaner_all_display();
                UI_static_main_box(COLOR_WHITE);
                UI_static_setting_menu();
                is_change_ui_main = false;
            }

            UI_dynamic_setting_menu(ui_setting_state, &global_volume); // 선택 강조/볼륨 출력

            int key = _getch();
            if (key == EXTENDED_KEY) key = _getch();

            UI_control_setting(&ui_main_state, &ui_setting_state, &global_volume, key);

            if (ui_main_state == UI_STATE_TITLE)
                is_change_ui_main = true;

            break;
        }
        } // switch end

        if (ui_main_state == UI_STATE_SELECT_GAME_MODE) {
            break;
        }
    }

    while(ui_main_state == UI_STATE_SELECT_GAME_MODE) { // --- 게임 모드 선택 ---
        if (is_change_ui_main) { 
            UI_cleaner_all_display();
            UI_static_main_box(COLOR_WHITE);
            UI_static_select_game_mode();
            is_change_ui_main = false;
        }
        UI_dynamic_select_game_mode(ui_mode_state);

        int key_mode = _getch();
        if (key_mode == EXTENDED_KEY) key_mode = _getch();

        // UI_control_game_mode가 ui_main_state를 변경
        UI_control_game_mode(&ui_main_state, &ui_mode_state, &game_mode, key_mode);
        if (ui_main_state != UI_STATE_SELECT_GAME_MODE) {
            is_change_ui_main = true; 
        }
    }

    while(ui_main_state == UI_STATE_SELECT_HERO) { // --- 영웅 선택 ---
        /*
        if (is_change_ui_main) {
             UI_static_hero_select(); // 영웅 선택 UI 그리기
             is_change_ui_main = false;
        }
        // ... 영웅 선택 로직 ...
        */
        
        // [임시 코드] 영웅 선택 기능 구현 전까지 'HERO_BREAKER'로 자동 선택
        choice_hero = HERO_BREAKER;
        ui_main_state = UI_STATE_CREATE_PLAYER_NAME;
        is_change_ui_main = true;
    }
     
    if(ui_main_state == UI_STATE_CREATE_PLAYER_NAME) { // --- 플레이어 이름 선택 (단일 함수 호출) --- 
        UI_cleaner_all_display();
        input_name = UI_dynamic_create_player_name();

        strncpy(player.name, input_name, sizeof(player.name) - 1);
        player.name[sizeof(player.name) - 1] = '\0';
        free(input_name);
    }

    UI_static_main_box(COLOR_WHITE);

    // --- 게임 데이터 초기화 ---
    item_init();
    inventory_init();
    store_init();
    player_init(&player, player.name, choice_hero);
    monster_init(&monster, currentStage);
    is_change_ui_main = true;
    ui_main_state = UI_STATE_BATTLE;

    player.action_value = 10000.0 / player.speed;
    monster.action_value = 10000.0 / monster.speed;

    // --- 메인 게임 루프 ---
    while (1)
    {
        if (ui_main_state == UI_STATE_BATTLE)
        {
            if (is_change_ui_main) {
                is_change_ui_main = false;
                UI_cleaner_all_display();
                UI_static_battle_box();
            }

            UI_dynamic_action_order(&player, &monster); 
            UI_dynamic_monster_info(&monster);
            UI_dynamic_player_info(&player);

            if (player.action_value <= monster.action_value) {
                UI_dynamic_player_action_selection(player_action_state);

                int key = _getch();
                if (key == EXTENDED_KEY) key = _getch();

                if (key == 's' || key == 'S') {
                    is_change_ui_main = true;
                    ui_main_state = UI_STATE_STORE;
                    continue;
                }

                player_action_t action = UI_control_player_action(&player_action_state, key);

                if (action != PLAYER_ACTION_NONE) {
                    if (action == PLAYER_ACTION_INVENTORY) {
                        is_change_ui_main = true;
                        ui_main_state = UI_STATE_INVENTORY;
                        continue; // 인벤토리로 이동
                    }

                    battle_result_t result = player_turn_process(&player, &monster, action);
                    if (result == BATTLE_RESULT_PLAYER_WIN) {
                        if (currentStage >= MAX_STAGE_BEFORE_BOSS) { // 메인 루프를 탈출하여 게임 클리어로 이동
                            break;
                        }
                        else { // 마지막이 아니라면 다음 스테이지로 이동
                            UI_cleaner_all_display();
                            utils_gotoxy(60, 14);
                            printf(">> 전투 승리! 다음 스테이지로 이동합니다. <<");
                            Sleep(2000);

                            currentStage++;
                            monster_init(&monster, currentStage);
                            player.action_value = 10000.0 / player.speed;
                            monster.action_value = 10000.0 / monster.speed;

                            is_change_ui_main = true;
                            continue;
                        }
                    }
                    // 행동 후 플레이어의 행동 가치 재설정
                    player.action_value += 10000.0 / player.speed;
                }
            }
            else {
                // === 몬스터 턴 ===
                Sleep(1000); 
                battle_result_t result = monster_turn_process(&monster, &player);
                if (result == BATTLE_RESULT_MONSTER_WIN) {
                    // TODO: 패배 처리
                    break;
                }
                // 행동 후 몬스터의 행동 가치 재설정
                monster.action_value += 10000.0 / monster.speed;
            }
        }
        else if (ui_main_state == UI_STATE_INVENTORY)
        {
            if (is_change_ui_main) {
                is_change_ui_main = false;
                set_inventory_state(INVENTORY_STATE_WEAPON);
                UI_cleaner_all_display();
                UI_static_inventory_box();
            }

            while (ui_main_state == UI_STATE_INVENTORY)
            {
                UI_dynamic_current_weapon_info(&player);
                UI_dynamic_current_armor_info(&player);

                UI_dynamic_inventory_info(&player);

                int key = _getch();
                if (key == EXTENDED_KEY) key = _getch();

                int change_equipment = UI_control_inventory(&ui_main_state, &player, key);
                if (change_equipment == 1) UI_cleaner_current_weapon_box();
                else if (change_equipment == 2) UI_cleaner_current_armor_box();

                else if (change_equipment == 3) { 
                    UI_cleaner_player_info(); 
                    UI_dynamic_player_info(&player); 
                }

                if (change_equipment != 0 && change_equipment != 3) UI_cleaner_player_info();
            }
            is_change_ui_main = true;
        }
        else if (ui_main_state == UI_STATE_STORE)
        {
            if (is_change_ui_main) {
                is_change_ui_main = false;
                UI_cleaner_all_display();
                UI_static_shop_box();
            }

            while (ui_main_state == UI_STATE_STORE)
            {
                UI_dynamic_store_info(&player);

                if (get_store_buy_sell_successful_state() != STORE_BUY_SELL_NONE) set_store_buy_sell_successful_state(STORE_BUY_SELL_NONE);

                int key = _getch();
                if (key == EXTENDED_KEY) key = _getch();

                UI_control_store(&ui_main_state, &player, key);
            }
            is_change_ui_main = true;
        }
    }

    UI_cleaner_all_display();
    utils_gotoxy(60, 14);
    printf("*********************************\n");
    utils_gotoxy(60, 15);
    printf("* *\n");
    utils_gotoxy(60, 16);
    printf("* G A M E   C L E A R      *\n");
    utils_gotoxy(60, 17);
    printf("* *\n");
    utils_gotoxy(60, 18);
    printf("*********************************\n");

    utils_gotoxy(0, 28);

    return 0;
}