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
    item_init();
    inventory_init();
    is_change_ui_main = true;
    player_init(&player, player.name);
    monster_init(&monster, currentStage);

    //test
    //inventory_get_all_items_for_test();

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
                // 디버깅용
                item_init();
                inventory_init();
                ui_main_state = UI_STATE_STORE;
                ui_store_state = STORE_STATE_WEAPON; // 초기 상태는 무기
                int store_focus_level = FOCUS_LEVEL_TOP;
                int ui_store_sub_title_state = STORE_SUB_TITLE_FOREST; // 계속 초기 상태는 숲
				int store_buy_sell_state = STORE_STATE_BUY; // 초기 상태는 구매
                int selected_item_index = 0;   // 선택된 아이템 인덱스
                int weapon_page = 0; // 페이지 번호 초기화
                int armor_page = 0;  // 페이지 번호 초기화

                UI_cleaner_all_display();
                UI_static_shop_box();

                while (ui_main_state == UI_STATE_STORE)
                {
                    UI_dynamic_store_info(&player, ui_store_state, ui_store_sub_title_state, store_focus_level, selected_item_index, store_buy_sell_state, weapon_page, armor_page);

                    int key = _getch();
                    if (key == EXTENDED_KEY) key = _getch();

                    if (key == ENTER && store_focus_level == FOCUS_LEVEL_ITEM_BUY_SELL) {
                        if (store_buy_sell_state == STORE_STATE_BUY) {
                            // 구매 로직      
							// 선택된 아이템 인덱스 계산
							int new_equipment_index = (ui_store_sub_title_state * 24) + selected_item_index;
                            // 구매할 아이템의 종류에 따라 처리
                            if (ui_store_state == STORE_STATE_WEAPON) {
                                if (player.coin >= weapons[new_equipment_index].buy_price) {
                                    player.coin -= weapons[new_equipment_index].buy_price;
									get_item(new_equipment_index, ITEM_TYPE_WEAPON);
                                    UI_cleaner_current_weapon_box();
                                }
                                else {
                                    printf("골드가 부족합니다.\n");
                                }
                            }
                            else if (ui_store_state == STORE_STATE_ARMOR) {
                                if (player.coin >= armors[new_equipment_index].buy_price) {
                                    player.coin -= armors[new_equipment_index].buy_price;
                                    get_item(new_equipment_index, ITEM_TYPE_ARMOR);
                                    UI_cleaner_current_armor_box();
                                    use_armor(new_equipment_index, &player);
                                }
                                else {
                                    printf("골드가 부족합니다.\n");
                                }
							}
                        }
                        else if (store_buy_sell_state == STORE_STATE_SELL) {
                            // 판매 로직  
							int new_equipment_index = (ui_store_sub_title_state * 24) + selected_item_index;
                            // 판매할 아이템의 종류에 따라 처리
                            if (ui_store_state == STORE_STATE_WEAPON) {
                                if (weapon_inventory[new_equipment_index].is_was_having) {
                                    player.coin += weapons[new_equipment_index].sell_price;
                                    sell_item(new_equipment_index, ITEM_TYPE_WEAPON);
                                    UI_cleaner_current_weapon_box();
                                }
                                else {
                                    printf("판매할 수 없는 아이템입니다.\n");
                                }
                            }
                            else if (ui_store_state == STORE_STATE_ARMOR) {
                                if (armor_inventory[new_equipment_index].is_was_having) {
                                    player.coin += armors[new_equipment_index].sell_price;
                                    sell_item(new_equipment_index, ITEM_TYPE_ARMOR);
                                    UI_cleaner_current_armor_box();
                                }
                                else {
                                    printf("판매할 수 없는 아이템입니다.\n");
                                }
							}
						}
                    }
                    else {
                        UI_cleaner_buy_sell_box();
                        UI_control_store(&ui_main_state, &ui_store_state, &ui_store_sub_title_state, &store_focus_level, &selected_item_index, key, &weapon_page, &armor_page, &store_buy_sell_state);
                    }
                   
                }
                break;
            case BATTLE_ACTION_NONE:
            default:
                break;
            }

        }
        else if (ui_main_state == UI_STATE_INVENTORY)
        {
            ui_inventory_state = INVENTORY_STATE_WEAPON; // 초기 상태는 무기
            int inventory_focus_level = FOCUS_LEVEL_TOP;
            int ui_inventory_sub_title_state = INVENTORY_SUB_TITLE_FOREST; // 계속 초기 상태는 숲
            int selected_item_index = 0;   // 선택된 아이템 인덱스
            int weapon_page = 0; // 페이지 번호 초기화
            int armor_page = 0;  // 페이지 번호 초기화

            if (is_change_ui_main) {
                is_change_ui_main = false;
                UI_cleaner_all_display();
                UI_static_inventory_box();             
            }

            // 인벤토리 화면 루프 시작
            while (ui_main_state == UI_STATE_INVENTORY)
            {
                // 현재 장착중인 무기와 장비 표시 
                // 장비 변경 할 때 마다 바꿔지도록 하기 위해 루프 안에 위치
                UI_dynamic_current_weapon_info(&player);
				UI_dynamic_current_armor_info(&player);

                // [수정] 새로운 상태 변수들을 넘겨줌
                UI_dynamic_inventory_info(&player, ui_inventory_state, ui_inventory_sub_title_state, inventory_focus_level, selected_item_index, weapon_page, armor_page);

                int key = _getch();
                if (key == EXTENDED_KEY) key = _getch();

                // ENTER 키이면서, 동시에 ITEM_LIST 상태일 때만 장착 로직을 실행
                if (key == ENTER && inventory_focus_level == FOCUS_LEVEL_ITEM_LIST)
                {
                    int new_equipment_index = (ui_inventory_sub_title_state * 24) + selected_item_index;

                    if (ui_inventory_state == INVENTORY_STATE_WEAPON)
                    {
                        UI_cleaner_current_weapon_box();
                        use_weapon(new_equipment_index, &player);
                    }
                    else if (ui_inventory_state == INVENTORY_STATE_ARMOR)
                    {
                        UI_cleaner_current_armor_box();
                        use_armor(new_equipment_index, &player);
                    }

                    UI_cleaner_player_info();
                }
                else // 그 외 모든 경우 (키가 ENTER가 아니거나, ENTER라도 소비 아이템/옵션/뒤로가기 상태일 때)
                {
                    // 인벤토리 내 다른 모든 키 조작(이동, 포커스 변경 등)을 처리
                    UI_control_inventory(&ui_main_state, &ui_inventory_state, &ui_inventory_sub_title_state, &inventory_focus_level, &selected_item_index, key, &weapon_page, &armor_page);
                }
            }

            // 인벤토리에서 빠져나왔으므로, 전투 화면을 다시 그리도록 설정
            is_change_ui_main = true;
        }
    }

    return 0;
}