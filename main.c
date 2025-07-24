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

#include "log.h"

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
static int               upgrade_selection = 0;

// ==================== 클리어 상태 파일 입출력 함수 (새로 추가) ====================
const char* CLEAR_STATUS_FILE = "save/clear_data.bin";

// 파일에서 클리어 상태를 불러오는 함수
bool load_clear_status() {
    FILE* file = fopen(CLEAR_STATUS_FILE, "rb");
    if (!file) return false; // 파일이 없으면 false 반환

    bool status = false;
    fread(&status, sizeof(bool), 1, file);
    fclose(file);
    return status;
}

// 파일에 클리어 상태를 저장하는 함수
void save_clear_status(bool status) {
    FILE* file = fopen(CLEAR_STATUS_FILE, "wb");
    if (!file) {
        perror("클리어 상태 저장 실패");
        return;
    }
    fwrite(&status, sizeof(bool), 1, file);
    fclose(file);
}

// ==================== 무한 모드 몬스터 강화 함수 (새로 추가) ====================
static void scale_monster_for_infinite_mode(monster_t* monster, int stage)
{
    // 1 스테이지는 기본 스탯 사용
    if (stage <= 1) return;

    // 2 스테이지부터 이전 스테이지 대비 5%씩 강해짐 (복리 적용)
    // 예: 3 스테이지 = 기본스탯 * 1.05 * 1.05
    double scale_multiplier = pow(1.05, stage - 1);

    monster->attack = (int)(monster->attack * scale_multiplier);
    monster->max_hp = (int)(monster->max_hp * scale_multiplier);
    monster->max_toughness = (int)(monster->max_toughness * scale_multiplier);
    monster->speed = (int)(monster->speed * scale_multiplier);

    // 현재 체력과 강인도를 강화된 최대치에 맞춰줌
    monster->current_hp = monster->max_hp;
    monster->current_toughness = monster->max_toughness;
}

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

    bool is_normal_mode_cleared = load_clear_status(); 
    
    const int MAX_STAGE_NORMAL = 2; // 일반 모드 최대 스테이지
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
        UI_dynamic_select_game_mode(ui_mode_state, is_normal_mode_cleared);

        int key_mode = _getch();
        if (key_mode == EXTENDED_KEY) key_mode = _getch();

        // UI_control_game_mode가 ui_main_state를 변경
        UI_control_game_mode(&ui_main_state, &ui_mode_state, &game_mode, key_mode, is_normal_mode_cleared);
        if (ui_main_state != UI_STATE_SELECT_GAME_MODE) {
            is_change_ui_main = true; 
        }
    }

    while(ui_main_state == UI_STATE_SELECT_HERO) { // --- 영웅 선택 ---
        if (is_change_ui_main) {
            UI_static_hero_select_box(); 
            is_change_ui_main = false;
        }

        UI_dynamic_hero_select(choice_hero);

        int key_hero = _getch();
        if (key_hero == EXTENDED_KEY) key_hero = _getch();

        UI_control_hero_select(&ui_main_state, &choice_hero, key_hero);

        if (ui_main_state != UI_STATE_SELECT_HERO) {
            is_change_ui_main = true;
        }
    }
     
    if(ui_main_state == UI_STATE_CREATE_PLAYER_NAME) { // --- 플레이어 이름 선택 (단일 함수 호출) --- 
        UI_cleaner_all_display();
        input_name = UI_dynamic_create_player_name();

        strncpy(player.name, input_name, sizeof(player.name) - 1);
        player.name[sizeof(player.name) - 1] = '\0';
        free(input_name);
    }

    UI_static_main_box(COLOR_WHITE);

    if (game_mode == GAME_MODE_INFINITY) {
        // 무한 모드이면 모든 장비 지급
        inventory_unlock_all_items();
    }
    else {
        // 일반 모드이면 인벤토리 초기화
        inventory_init();
    }

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
            UI_dynamic_player_action_selection(player_action_state);

            if (player.action_value <= monster.action_value) {
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
                        currentStage++;

                        // --- 무한 모드 전용 업그레이드 ---
                        if (game_mode == GAME_MODE_INFINITY) {
                            ui_main_state = UI_STATE_INFINITE_UPGRADE;
                            upgrade_type_t choices[3];
                            UI_control_generate_upgrade_choices(&player, choices);
                            upgrade_selection = 0;
                            is_change_ui_main = true; // 화면을 새로 그려야 함

                            while (ui_main_state == UI_STATE_INFINITE_UPGRADE) {
                                if (is_change_ui_main) {
                                    UI_static_infinite_upgrade_box();
                                    is_change_ui_main = false;
                                }
                                UI_dynamic_infinite_upgrade(&player, choices, upgrade_selection);

                                int key_upgrade = _getch();
                                if (key_upgrade == EXTENDED_KEY) key_upgrade = _getch();

                                // 새로 만든 제어 함수 호출
                                UI_control_handle_upgrade_selection(&ui_main_state, &player, choices, &upgrade_selection, key_upgrade);
                            }
                        }

                        // --- 다음 스테이지 준비 ---
                        UI_cleaner_all_display();
                        utils_gotoxy(60, 14);
                        printf(">> 다음 스테이지로 이동합니다. <<");
                        Sleep(1500);

                        if (game_mode == GAME_MODE_NORMAL) {
                            if (currentStage > MAX_STAGE_NORMAL) break;
                            monster_init(&monster, currentStage);
                        }
                        else {
                            int slime_index = 1;
                            monster_init(&monster, slime_index);
                            scale_monster_for_infinite_mode(&monster, currentStage);

                            log_buffer_clear();
                            int heal_point = (int)(player.auto_heal * player.max_hp);
                            player.current_hp += heal_point;
                            if (player.current_hp > player.max_hp)
                            {
                                heal_point -= (player.current_hp - player.max_hp);
                                player.current_hp = player.max_hp;
                            }
                            log_auto_heal(&player, heal_point);
                        }

                        is_change_ui_main = true;
                        continue;
                    }

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

    // --- 게임 클리어 처리 ---
   // [변경] 일반 모드를 클리어했을 때만 상태를 저장
    if (game_mode == GAME_MODE_NORMAL) {
        save_clear_status(true);
    }

    UI_cleaner_all_display();
    // ... (게임 클리어 화면 출력) ...

    return 0;
}