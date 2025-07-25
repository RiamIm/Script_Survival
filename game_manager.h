// game_manager.h
#pragma once
#include "player.h"
#include "monster.h"
#include "UI_info.h"

// 게임의 모든 데이터와 상태를 담는 통합 구조체
typedef struct {
    // --- 핵심 데이터 ---
    player_t player;
    monster_t monster;
    int game_mode;
    int currentStage;
    hero_t choice_hero;

    // --- UI 상태 ---
    UI_state_t ui_main_state;
    title_state_t ui_title_state;
    setting_state_t ui_setting_state;
    game_mode_state_t ui_mode_state;
    player_action_t player_action_state;
    int upgrade_selection;
    bool is_change_ui_main;

    // --- 시스템 설정 ---
    bool is_normal_mode_cleared;
    int global_volume;

} game_context_t;

// --- 게임 매니저 공개 함수 ---
void GameManager_Init();     // 게임 초기화
void GameManager_Run();      // 게임 실행
void GameManager_Shutdown(); // 게임 종료