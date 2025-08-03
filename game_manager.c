// game_manager.c
#define _CRT_SECURE_NO_WARNINGS
#include "game_manager.h"

#include "inout.h"
#include "utils.h"

#include "battle.h"
#include "item.h"
#include "store.h"
#include "inventory.h"

#include "monster.h"
#include "player.h"

#include "log.h"

#include "story.h"

#include "UI_static.h"
#include "UI_dynamic.h"
#include "UI_control.h"
#include "UI_cleaner.h"

// 이 파일에서만 사용할 게임 컨텍스트 전역 변수
static game_context_t g_context;

#define MAX_STAGE 13

// === 내부 함수 선언 ===
static void state_pre_game_sequence();
static void state_battle(bool* is_game_over);
static void state_select_heal_or_store();
static void state_inventory();
static void state_store();
static void state_esc_menu();
static void state_save();
static void state_setting_menu();
static void handle_next_stage(bool* is_game_over);

const char* CLEAR_STATUS_FILE = "data/clear_data.dat";
static bool load_clear_status();
static void save_clear_status(bool status);
static void scale_monster_for_infinite_mode(monster_t* monster, int stage);

static bool is_come_esc_menu = false;   

// === 공개 함수 구현 ===
void GameManager_Init() {
    g_context = (game_context_t){
        .game_mode = MODE_STATE_NORMAL,
        .currentStage = 0,
        .choice_hero = HERO_BREAKER,
        .is_normal_mode_cleared = load_clear_status(),
        .is_change_ui_main = true,
        .global_volume = 50,
        .upgrade_selection = 0,
        .ui_esc_menu_state = ESC_MENU_STATE_BACK,
        .save_load_num = SAVE_LOAD_1,
        .new_or_load_game = NEW_GAME,
        .heal_or_store_state = HEAL_OR_STORE_HEAL,
    };
    UI_control_init(&g_context.ui_main_state, &g_context.ui_title_state, &g_context.player_action_state);
}

void GameManager_Run() {
    GameManager_Init();
    story_init();
    state_pre_game_sequence();
    
    load_image_log(&g_context.monster, g_context.currentStage);
    
    item_init();
    store_init();

    if (g_context.new_or_load_game == NEW_GAME) {
        char* player_name = UI_dynamic_create_player_name();
        if (g_context.game_mode == MODE_STATE_INFINITY) inventory_unlock_all_items();
        else inventory_init();
        player_init(&g_context.player, player_name, g_context.choice_hero);
        UI_cleaner_all_display();
        if (g_context.game_mode == MODE_STATE_NORMAL) {
            story_play("PROLOGUE", log_prologue);
        }
            
        else if (g_context.game_mode == MODE_STATE_INFINITY)
        {
            story_play("INFINITY", log_infinite_mode_start);
        }
            

        // 계승 정보(파일)이 있을 때 장착 중 이었던 장비 코인 계승
        if (player_load_legacy_data(&g_context.player)) {
            story_play("LAGACY", log_legacy);
            remove("data/legacy.dat");
        }

        free(player_name);
        if (!monster_init(&g_context.monster, g_context.currentStage)) {
            fprintf(stderr, "몬스터 초기화 실패 (stage %d)\n", g_context.currentStage);
            exit(1);
        }

        g_context.player.action_value = 10000.0 / g_context.player.speed;
        g_context.monster.action_value = 10000.0 / g_context.monster.speed;
    }

    g_context.ui_main_state = UI_STATE_BATTLE;
   
    bool is_game_over = false;
    while (!is_game_over && g_context.ui_main_state != UI_STATE_TITLE) {
        switch (g_context.ui_main_state) {
        case UI_STATE_BATTLE:    state_battle(&is_game_over); break;
        case UI_STATE_SELECT_HEAL_OR_STORE: state_select_heal_or_store(); break;
        case UI_STATE_INVENTORY: state_inventory();           break;
        case UI_STATE_STORE:     state_store();               break;
        case UI_STATE_SETTING:   is_come_esc_menu = true; state_setting_menu(); is_come_esc_menu = false;       break;
        case UI_STATE_ESC_MENU:  state_esc_menu();            break;
		case UI_STATE_SAVE:      state_save();                break;
        }
    }

    if (g_context.ui_main_state == UI_STATE_TITLE) {
        UI_cleaner_all_display();
        GameManager_Run();
	}

    if (g_context.game_mode == MODE_STATE_NORMAL && g_context.currentStage == 13) {
        story_play("CHAPTER4", log_chapter_4);
        save_clear_status(true);
    }

    GameManager_Shutdown();
}

void GameManager_Shutdown() {
    UI_cleaner_all_display();
    utils_gotoxy(60, 14);
    printf("--- GAME OVER ---\n");
    exit(0);
}

// === 내부 함수 구현 ===
static void state_pre_game_sequence()
{
    // --- 1. 타이틀 / 설정 화면 루프 ---
    while (g_context.ui_main_state == UI_STATE_TITLE || g_context.ui_main_state == UI_STATE_SETTING)
    {
        if (g_context.is_change_ui_main) {
            UI_cleaner_all_display();
            UI_static_main_box(COLOR_WHITE);
            if (g_context.ui_main_state == UI_STATE_TITLE) {
                UI_static_title();
            }
            else { // UI_STATE_SETTING
                UI_static_setting_menu();
            }
            g_context.is_change_ui_main = false;
        }

        UI_state_t previous_state = g_context.ui_main_state;

        if (g_context.ui_main_state == UI_STATE_TITLE) {
            UI_dynamic_title_selection(g_context.ui_title_state);
            int key = utils_getch();
            UI_control_title(&g_context.ui_main_state, &g_context.ui_title_state, key);
        }
        else { // UI_STATE_SETTING
            UI_dynamic_setting_menu(g_context.ui_setting_state, &g_context.global_volume);
            int key = utils_getch();
            UI_control_setting(&g_context.ui_main_state, &g_context.ui_setting_state, is_come_esc_menu, &g_context.global_volume, key);
        }

        if (g_context.ui_main_state != previous_state) {
            g_context.is_change_ui_main = true;
        }
    }

    // --- 2. 게임 모드 선택 루프 ---
    while (g_context.ui_main_state == UI_STATE_SELECT_GAME_MODE) {
        if (g_context.is_change_ui_main) {
            UI_cleaner_all_display(); UI_static_main_box(COLOR_WHITE); UI_static_select_game_mode(); g_context.is_change_ui_main = false;
        }
        UI_dynamic_select_game_mode(g_context.ui_mode_state, g_context.is_normal_mode_cleared);
        int key = utils_getch();
        UI_control_game_mode(&g_context.ui_main_state, &g_context.ui_mode_state, &g_context.game_mode, key, g_context.is_normal_mode_cleared);
        if (g_context.ui_main_state != UI_STATE_SELECT_GAME_MODE) g_context.is_change_ui_main = true;
    }

    while (g_context.ui_main_state == UI_STATE_NEW_OR_LOAD_GAME || g_context.ui_main_state == UI_STATE_LOAD) {
        if (g_context.ui_main_state == UI_STATE_NEW_OR_LOAD_GAME) {
            if (g_context.is_change_ui_main) {
                UI_cleaner_all_display(); g_context.is_change_ui_main = false;
            }
            UI_dynamic_select_new_or_load_game(&g_context.new_or_load_game);
            int key = utils_getch();
            UI_control_select_new_or_load_game(&g_context.ui_main_state, &g_context.new_or_load_game, key);
            if (g_context.ui_main_state == UI_STATE_SELECT_HERO || g_context.ui_main_state == UI_STATE_LOAD) {
                g_context.is_change_ui_main = true;
            }
        }

        if (g_context.ui_main_state == UI_STATE_LOAD) {
            if (g_context.is_change_ui_main) { UI_cleaner_all_display(); UI_static_save_load_box(); g_context.is_change_ui_main = false; }
            UI_dynamic_save_load_menu(&g_context.save_load_num);
            int key = utils_getch();
            UI_control_load_menu(&g_context.ui_main_state, &g_context.save_load_num, key, &g_context);
            if (g_context.ui_main_state != UI_STATE_LOAD) {
                g_context.is_change_ui_main = true;
            }
        }
    }

    // --- 3. 영웅 선택 루프 ---
    while (g_context.ui_main_state == UI_STATE_SELECT_HERO) {
        if (g_context.is_change_ui_main) { UI_static_hero_select_box(); g_context.is_change_ui_main = false; }
        UI_dynamic_hero_select(g_context.choice_hero);
        int key = utils_getch();
        UI_control_hero_select(&g_context.ui_main_state, &g_context.choice_hero, key);
        if (g_context.ui_main_state != UI_STATE_SELECT_HERO) g_context.is_change_ui_main = true;
    }
}

static void state_battle(bool* is_game_over) {
    if (g_context.is_change_ui_main) { UI_cleaner_all_display(); utils_set_color(COLOR_DEFAULT_TEXT); UI_static_battle_box(); g_context.is_change_ui_main = false; }
    UI_dynamic_monster_info(&g_context.monster, g_context.currentStage);
    UI_dynamic_player_info(&g_context.player);
    UI_dynamic_action_order(&g_context.player, &g_context.monster);
    
    battle_result_t result;
    if (g_context.player.action_value <= g_context.monster.action_value) {
        UI_dynamic_player_action_selection(g_context.player_action_state);
        int key = utils_getch();

        // 디버깅용
        if ((key == 's' || key == 'S') && g_context.game_mode != GAME_MODE_INFINITY) {
            g_context.ui_main_state = UI_STATE_STORE; g_context.is_change_ui_main = true; return;
        }

        if (key == ESC) {
			g_context.ui_main_state = UI_STATE_ESC_MENU; g_context.is_change_ui_main = true; return;
        }

        player_action_t action = UI_control_player_action(&g_context.player_action_state, key);
        if (action == PLAYER_ACTION_NONE) return;

        if (action == PLAYER_ACTION_INVENTORY) {
            g_context.ui_main_state = UI_STATE_INVENTORY; g_context.is_change_ui_main = true; return;
        }

        result = player_turn_process(&g_context.player, &g_context.monster, action);
        if (result == BATTLE_RESULT_PLAYER_WIN) { 
            monster_item_drop(&g_context.player, g_context.currentStage);
            handle_next_stage(is_game_over); 
            if (g_context.currentStage == 4) story_play("CHAPTER2", log_chapter_2);
            else if (g_context.currentStage == 8) story_play("CHAPTER3", log_chapter_3);
            if (g_context.game_mode != GAME_MODE_INFINITY)
                g_context.ui_main_state = UI_STATE_SELECT_HEAL_OR_STORE; // 다음 단계 넘어가기 전 회복 또는 상점 선택 창
        }
        else { g_context.player.action_value += 10000.0 / g_context.player.speed; }

    }
    else {
        Sleep(1000);
        int current_monster_turn = g_context.monster.action_value / (10000.0 / g_context.monster.speed);
        result = monster_turn_process(&g_context.monster, &g_context.player, (current_monster_turn % 3 == 0), g_context.currentStage);
        if (result == BATTLE_RESULT_PLAYER_WIN) { 
            if (g_context.game_mode != GAME_MODE_INFINITY) // 무한 모드에선 아이템 드랍 필요 없음
                monster_item_drop(&g_context.player, g_context.currentStage);
            handle_next_stage(is_game_over);    
            if (g_context.game_mode != GAME_MODE_INFINITY)
                g_context.ui_main_state = UI_STATE_SELECT_HEAL_OR_STORE; // 다음 단계 넘어가기 전 회복 또는 상점 선택 창
        }
        else if (result == BATTLE_RESULT_MONSTER_WIN) { *is_game_over = true; }
        else { g_context.monster.action_value += 10000.0 / g_context.monster.speed; }
    }
}

static void state_select_heal_or_store() {
    if (g_context.is_change_ui_main) { UI_cleaner_all_display(); UI_static_select_heal_or_store_box(); g_context.heal_or_store_state = HEAL_OR_STORE_HEAL;  g_context.is_change_ui_main = false; }
    while (g_context.ui_main_state == UI_STATE_SELECT_HEAL_OR_STORE) {
        UI_dynamic_select_heal_or_store(&g_context.heal_or_store_state, &g_context.player);
        int key = utils_getch();
        UI_control_select_heal_or_store(&g_context.ui_main_state, &g_context.heal_or_store_state, &g_context.player, key);
        if (g_context.player.run == true) {
            UI_cleaner_all_display();
            GameManager_Shutdown();
        }
        if (g_context.ui_main_state != UI_STATE_SELECT_HEAL_OR_STORE) g_context.is_change_ui_main = true;
    }
}

static void state_inventory() {
    if (g_context.is_change_ui_main) { UI_cleaner_all_display(); UI_static_inventory_box(); g_context.is_change_ui_main = false; set_inventory_state(INVENTORY_STATE_WEAPON); set_inventory_rarity_type(RARITY_NORMAL); set_inventory_selected_index(0); }

    while (g_context.ui_main_state == UI_STATE_INVENTORY) {
        UI_dynamic_current_weapon_info(&g_context.player);
        UI_dynamic_current_armor_info(&g_context.player);
        UI_dynamic_inventory_info(&g_context.player);
        int key = utils_getch();
        int change = UI_control_inventory(&g_context.ui_main_state, &g_context.player, key);
        if (change == 1) { UI_cleaner_current_weapon_box(); UI_cleaner_player_info(); }
        else if (change == 2) { UI_cleaner_current_armor_box(); UI_cleaner_player_info(); }
        else if (change == 3) { UI_cleaner_player_info(); UI_dynamic_player_info(&g_context.player); }
    }
    g_context.is_change_ui_main = true;
}

static void state_store() {
    if (g_context.is_change_ui_main) { UI_cleaner_all_display(); UI_static_shop_box(); g_context.is_change_ui_main = false; set_store_state(STORE_STATE_WEAPON); set_store_rarity_type(RARITY_NORMAL); set_store_selected_index(0); }
    while (g_context.ui_main_state == UI_STATE_STORE) {
        UI_dynamic_store_info(&g_context.player);
        if (get_store_buy_sell_successful_state() != STORE_BUY_SELL_NONE) set_store_buy_sell_successful_state(STORE_BUY_SELL_NONE);
        int key = utils_getch();
        UI_control_store(&g_context.ui_main_state, &g_context.player, key);
    }
    g_context.is_change_ui_main = true;
}

static void state_esc_menu() {
    if (g_context.is_change_ui_main) { UI_cleaner_all_display(); g_context.is_change_ui_main = false; }
    while (g_context.ui_main_state == UI_STATE_ESC_MENU) {
        UI_dynamin_esc_menu(&g_context.ui_esc_menu_state, g_context.game_mode);
        int key = utils_getch();
        UI_control_esc_menu(&g_context.ui_main_state, &g_context.ui_esc_menu_state, key, g_context.game_mode);
	} 
	g_context.is_change_ui_main = true;
}

static void state_save() {
    if (g_context.is_change_ui_main) { UI_cleaner_all_display(); UI_static_save_load_box(); g_context.is_change_ui_main = false; }
    while (g_context.ui_main_state == UI_STATE_SAVE) {
        UI_dynamic_save_load_menu(&g_context.save_load_num);
        int key = utils_getch();
        UI_control_save_load_menu(&g_context.ui_main_state, &g_context.save_load_num, key, &g_context);
    }
	g_context.is_change_ui_main = true;
}


static void state_setting_menu() {
    if (g_context.is_change_ui_main) { UI_cleaner_all_display(); UI_static_setting_menu(); g_context.is_change_ui_main = false; }
    while (g_context.ui_main_state == UI_STATE_SETTING) {
        UI_dynamic_setting_menu(g_context.ui_setting_state, &g_context.global_volume);
        int key = utils_getch();
        UI_control_setting(&g_context.ui_main_state, &g_context.ui_setting_state, is_come_esc_menu, &g_context.global_volume, key);
    }
	g_context.is_change_ui_main = true;
}

static void handle_next_stage(bool* is_game_over) {
    g_context.currentStage++;

    if (g_context.game_mode == GAME_MODE_INFINITY) {
        g_context.ui_main_state = UI_STATE_INFINITE_UPGRADE;
        upgrade_type_t choices[3];
        UI_control_generate_upgrade_choices(&g_context.player, choices);
        g_context.upgrade_selection = 0;
        g_context.is_change_ui_main = true;

        while (g_context.ui_main_state == UI_STATE_INFINITE_UPGRADE) {
            if (g_context.is_change_ui_main) { UI_cleaner_all_display(); UI_static_infinite_upgrade_box(); g_context.is_change_ui_main = false; }
            UI_dynamic_infinite_upgrade(&g_context.player, choices, g_context.upgrade_selection);
            int key = utils_getch();
            UI_control_handle_upgrade_selection(&g_context.ui_main_state, &g_context.player, choices, &g_context.upgrade_selection, key);
        }
    }

    UI_cleaner_all_display();
    utils_gotoxy(60, 14); printf(">> 다음 스테이지로 이동합니다. <<");
    Sleep(1500);

    if (g_context.game_mode == GAME_MODE_NORMAL) {
        if (g_context.currentStage >= MAX_STAGE) { *is_game_over = true; return; }
        monster_init(&g_context.monster, g_context.currentStage);
    }
    else {
        monster_init(&g_context.monster, 1);
        scale_monster_for_infinite_mode(&g_context.monster, g_context.currentStage);
    }

    g_context.player.action_value = 10000.0 / g_context.player.speed;
    g_context.monster.action_value = 10000.0 / g_context.monster.speed;
    g_context.is_change_ui_main = true;
}

static bool load_clear_status() {
    FILE* file = fopen(CLEAR_STATUS_FILE, "rb");
    if (!file) return false;
    bool status = false;
    fread(&status, sizeof(bool), 1, file);
    fclose(file);
    return status;
}

static void save_clear_status(bool status) {
    FILE* file = fopen(CLEAR_STATUS_FILE, "wb");
    if (!file) { perror("클리어 상태 저장 실패"); return; }
    fwrite(&status, sizeof(bool), 1, file);
    fclose(file);
}

static void scale_monster_for_infinite_mode(monster_t* monster, int stage) {
    if (stage <= 1) return;
    double scale_multiplier = pow(1.05, stage - 1);
    monster->attack = (int)(monster->attack * scale_multiplier);
    monster->max_hp = (int)(monster->max_hp * scale_multiplier);
    monster->max_toughness = (int)(monster->max_toughness * scale_multiplier);
    monster->speed = (int)(monster->speed * scale_multiplier);
    monster->current_hp = monster->max_hp;
    monster->current_toughness = monster->max_toughness;
}