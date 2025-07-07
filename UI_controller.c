
#include "UI_controller.h"

int title_state = TITLE_STATE_START;
int setting_state = 0; 
int battle_state = BATTLE_STATE_ATTACK;
int inventory_state = INVENTORY_STATE_WEAPON;
int store_state = 0;	

UI_state_t title_control(int key)
{
	if (key == ENTER) {
		if (title_state == TITLE_STATE_START) {
			return UI_STATE_BATTLE; // 게임 시작
		} else if (title_state == TITLE_STATE_OPTIONS) {
			return UI_STATE_SETTING; // 옵션 설정
		} else if (title_state == TITLE_STATE_EXIT) {
			exit(0); // 게임 종료
		}
		return UI_STATE_TITLE; // 기본적으로 타이틀 상태로 유지
	}

	if(key == UP || key == DOWN) {
		if (title_state == TITLE_STATE_START && key == DOWN) {
			title_state = TITLE_STATE_OPTIONS;
		} else if (title_state == TITLE_STATE_OPTIONS && key == UP) {
			title_state = TITLE_STATE_START;
		} else if (title_state == TITLE_STATE_OPTIONS && key == DOWN) {
			title_state = TITLE_STATE_EXIT;
		} else if (title_state == TITLE_STATE_EXIT && key == UP) {
			title_state = TITLE_STATE_OPTIONS;
		}
	}
	
	draw_title_ui(title_state);

	return UI_STATE_TITLE; // 타이틀 상태로 유지
}

UI_state_t setting_control(menu_key)
{

}

UI_state_t battle_control(menu_key)
{

}
UI_state_t inventory_control(menu_key)
{

}

UI_state_t store_control(menu_key)
{

}