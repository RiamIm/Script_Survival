#include "UI_controller.h"

int title_state = TITLE_STATE_START;
int setting_state = 0; 
int battle_state = BATTLE_STATE_ATTACK;
int inventory_state = INVENTORY_STATE_WEAPON;
int store_state = 0;	

static void s_draw_title_selection(int title_state)
{
	const char* menu[] = {
		"1. 시작",
		"2. 옵션",
		"3. 종료"
	};
	set_color(COLOR_DEFAULT);
	for (int i = 0; i < 3; i++) {
		int mx = (WIDTH - (int)strlen(menu[i])) / 2;
		gotoxy(mx, 15 + i * 2);

		if (title_state == i) {
			set_color(COLOR_SELECT_MENU);
			printf("%s", menu[i]);
			set_color(COLOR_DEFAULT);
		}
		else
		{
			printf("%s", menu[i]);
		}
	}

	set_color(COLOR_DEFAULT_TEXT);
}

static void s_draw_player_name_input_box(void)
{
	const int box_width = 28;
	const int box_height = 3;
	const int start_x = (WIDTH - box_width) / 2;
	const int start_y = (HEIGHT - box_height) / 2;

	for (int i = 0; i < box_height; i++) {
		gotoxy(start_x, start_y + i); putchar('|');
		gotoxy(start_x + box_width - 1, start_y + i); putchar('|');
	}

	for (int i = 0; i < box_width; i++) {
		gotoxy(start_x + i, start_y); putchar('=');
		gotoxy(start_x + i, start_y + box_height - 1); putchar('=');
	}
	

	const char* prompt = "플레이어 이름을 입력하세요 (최대 20자)";
	int px = (WIDTH - (int)strlen(prompt)) / 2;
	gotoxy(px, start_y - 2);
	printf("%s", prompt);

	gotoxy(start_x + 1, start_y + 1);
}

static void s_confirm_player_name_selection(int title_state)
{
	const char* menu[] = {
		"Y. 예",
		"N. 아니오"
	};
	set_color(COLOR_DEFAULT);
	for (int i = 0; i < 2; i++) {
		int mx = (WIDTH - (int)strlen(menu[i])) / 2;
		gotoxy(mx, 20 + i * 2);
		if (title_state == i) {
			set_color(COLOR_SELECT_MENU);
			printf("%s", menu[i]);
			set_color(COLOR_DEFAULT);
		}
		else
		{
			printf("%s", menu[i]);
		}
	}
	set_color(COLOR_DEFAULT_TEXT);
}

// 입력한 이름으로 할껀지 다시 물어보는 함수
static bool s_confirm_player_name(const char* name)
{
	clean_all_display();

	const int box_width = 28;
	const int box_height = 3;
	const int start_x = (WIDTH - box_width) / 2;
	const int start_y = (HEIGHT - box_height) / 2;

	for (int i = 0; i < box_height; i++) {
		gotoxy(start_x, start_y + i); putchar('|');
		gotoxy(start_x + box_width - 1, start_y + i); putchar('|');
	}

	for (int i = 0; i < box_width; i++) {
		gotoxy(start_x + i, start_y); putchar('=');
		gotoxy(start_x + i, start_y + box_height - 1); putchar('=');
	}

	const char* prompt_header = "입력하신 모험가님의 이름";
	int px = (WIDTH - (int)strlen(prompt_header)) / 2;
	gotoxy(px, start_y - 2);
	printf("%s", prompt_header);

	gotoxy(start_x + 1, start_y + 1);
	printf("%s", name);

	const char* prompt_confirm = "이 이름으로 시작하시겠습니까?";
	int pcx = (WIDTH - (int)strlen(prompt_confirm)) / 2;
	gotoxy(pcx, start_y + 3);
	printf("%s", prompt_confirm);

	int state = 0; // 0: Y, 1: N
	s_confirm_player_name_selection(state);

	while (1) {
		int key = _getch();
		if (key == UP || key == DOWN) {
			if (state == 0 && key == DOWN) {
				state = 1;
			} else if (state == 1 && key == UP) {
				state = 0;
			}
			s_confirm_player_name_selection(state);
		}
		else if (key == ENTER) {
			if (state == 0) {
				return true; // 예
			}
			else if (state == 1) {
				return false; // 아니오
			}
		}
	}
}

char* draw_create_player_name_ui(void)
{
	char* name = NULL;

	while (1)
	{
		clean_all_display();

		set_color(COLOR_DEFAULT_TEXT);
		s_draw_player_name_input_box();

		fflush(stdin);

		name = malloc(25);
		if (!name) {
			fprintf(stderr, "메모리 할당 실패\n");
			exit(1);
		}

		if (fgets(name, 25, stdin)) {
			name[strcspn(name, "\r\n")] = '\0';
		}
		else {
			name[0] = '\0';
		}

		if (s_confirm_player_name(name)) {
			break; // 이름이 확정되면 루프 종료
		} else {
			free(name); // 이름이 확정되지 않으면 메모리 해제 후 다시 입력 받기
			name = NULL;
		}
	}

	return name;
}

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
	
	s_draw_title_selection(title_state); 

	return UI_STATE_TITLE; // 타이틀 상태로 유지
}

UI_state_t setting_control(menu_key)
{

}

battle_state_t battle_control(int current_stage, player_t* player, monster_t* monster, int key)
{
	if (key == ENTER) {
		if (battle_state != BATTLE_STATE_ATTACK) {
			return battle_state; // 공격 상태가 아닐 경우 행동하지 않고 나가서 메인 그 행동 실행
		}
		return battle_state; // 선택한 행동 실행
	}

	if (key == UP || key == DOWN) {
		if (battle_state == BATTLE_STATE_ATTACK && key == DOWN) {
			battle_state = BATTLE_STATE_INVENTORY;
		} else if (battle_state == BATTLE_STATE_INVENTORY && key == UP) {
			battle_state = BATTLE_STATE_ATTACK;
		} else if (battle_state == BATTLE_STATE_INVENTORY && key == DOWN) {
			battle_state = BATTLE_STATE_EXTORTION;
		} else if (battle_state == BATTLE_STATE_EXTORTION && key == UP) {
			battle_state = BATTLE_STATE_INVENTORY;
		}
	}

	draw_battle_ui(current_stage, player, monster, battle_state);

	return UI_STATE_BATTLE; // 배틀 상태로 유지
}

UI_state_t inventory_control(menu_key)
{

}

UI_state_t store_control(menu_key)
{

}