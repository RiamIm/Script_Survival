// UI_dynamic.h
#pragma once
#include "inout.h"
#include "utils.h"

#include "UI_info.h"
#include "UI_static.h"
#include "UI_cleaner.h"

// 헬퍼 함수
static void helper_confirm_player_name_selection(int title_state);

// 사용자 이름 입력 관련 함수
void  UI_dynamic_player_name_input(void);
bool  UI_dynamic_confirm_player_name(const char* name);
char* UI_dynamic_create_player_name(void);

// 동적 ui 생성 함수
void  UI_dynamic_monster_info(monster_t* monster);
void  UI_dynamic_player_info(player_t* player);
void  UI_dynamic_inventory_info(player_t* player, int ui_inventory_state, int focus_level, int selected_item_index);

void  UI_dynamic_title_selection(int ui_title_state);
void  UI_dynamic_battle_selection(int ui_battle_state);