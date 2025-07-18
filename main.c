// main.c 
#define _CRT_SECURE_NO_WARNINGS

#include "inout.h"
#include "utils.h"

#include "battle.h"
#include "battle_eval.h"
#include "player.h"
#include "monster.h"
#include "item.h"
#include "inventory.h"
#include "region.h"

#include "UI_info.h"
#include "UI_control.h"
#include "UI_static.h"
#include "UI_dynamic.h"
#include "UI_cleaner.h"

// --- UI 상태 전역 변수 ---
static UI_state_t        ui_main_state;
static title_state_t     ui_title_state;
static battle_state_t    ui_battle_state;

// --- Inventory 상태 전역 변수 ---
static inventory_state_t g_inventory_state;
static focus_level_t     g_inventory_focus_level;
static region_t          g_inventory_region;
static int               g_inventory_selected_index;
static int               g_inventory_weapon_page;
static int               g_inventory_armor_page;

// -- store 상태 전역 변수 ---
static store_state_t     g_store_state;
static focus_level_t     g_store_focus_level;
static region_t          g_store_region;
static store_state_t     g_store_buy_sell_state;
static int               g_store_selected_index;
static int               g_store_weapon_page;
static int               g_store_armor_page;


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

    // UI 상태 초기화
    UI_control_init(
        &ui_main_state, &ui_title_state, &ui_battle_state,
        &g_inventory_state, &g_inventory_focus_level, &g_inventory_region, &g_inventory_selected_index, &g_inventory_weapon_page, &g_inventory_armor_page,
        &g_store_state, &g_store_focus_level, &g_store_region, &g_store_buy_sell_state, &g_store_selected_index, &g_store_weapon_page, &g_store_armor_page
    );
    UI_static_main_box();

    bool is_change_ui_main = true; // 화면 UI를 메인 화면으로 변경해야 하는지 여부를 나타내는 플래그
    int currentStage = 1;          // 현재 게임의 스테이지 번호 (1단계부터 시작)
    char* input_name = NULL;       // 사용자로부터 입력받은 이름을 저장할 포인터 (메모리 할당 전 NULL로 초기화)
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
            exit(0); // 임시 (사운드 조절 옵션창 추가할 곳)
            // break; (switch 문이므로 추가 후 break; 로 끝나야 함)
        }
        case UI_STATE_CREATE_PLAYER_NAME:
        {
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

    // --- 게임 데이터 초기화 ---
    item_init();
    inventory_init();
    is_change_ui_main = true;
    player_init(&player, player.name);
    monster_init(&monster, currentStage);

    // inventory_get_all_items_for_test(); 

    // --- 메인 게임 루프 ---
    while (1)
    {
        if (ui_main_state == UI_STATE_BATTLE)
        {
            if (is_change_ui_main) {
                is_change_ui_main = false;
                UI_cleaner_all_display();
                UI_static_battle_box();
                UI_dynamic_battle_selection(ui_battle_state);
            }
            UI_dynamic_monster_info(&monster);
            UI_dynamic_player_info(&player);

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
                is_change_ui_main = true;
                ui_main_state = UI_STATE_STORE;
                break;
            default:
                break;
            }
        }
        else if (ui_main_state == UI_STATE_INVENTORY)
        {
            if (is_change_ui_main) {
                is_change_ui_main = false;
                UI_cleaner_all_display();
                UI_static_inventory_box();
            }

            while (ui_main_state == UI_STATE_INVENTORY)
            {
                UI_dynamic_current_weapon_info(&player);
                UI_dynamic_current_armor_info(&player);

                UI_dynamic_inventory_info(
                    &player, weapon_inventory, armor_inventory,
                    g_inventory_state, g_inventory_region, g_inventory_focus_level,
                    g_inventory_selected_index, g_inventory_weapon_page, g_inventory_armor_page
                );

                int key = _getch();
                if (key == EXTENDED_KEY) key = _getch();

                UI_control_inventory(
                    &ui_main_state, &g_inventory_state, &g_inventory_region,
                    &g_inventory_focus_level, &g_inventory_selected_index, key,
                    &g_inventory_weapon_page, &g_inventory_armor_page, &player
                );
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
                UI_dynamic_store_info(
                    &player, weapon_inventory, armor_inventory,
                    g_store_state, g_store_region, g_store_focus_level,
                    g_store_selected_index, g_store_buy_sell_state,
                    g_store_weapon_page, g_store_armor_page
                );

                int key = _getch();
                if (key == EXTENDED_KEY) key = _getch();

                UI_control_store(
                    &ui_main_state, &g_store_state, &g_store_region,
                    &g_store_focus_level, &g_store_selected_index, key,
                    &g_store_weapon_page, &g_store_armor_page, &g_store_buy_sell_state, &player
                );
            }
            is_change_ui_main = true;
        }
    }

    return 0;
}