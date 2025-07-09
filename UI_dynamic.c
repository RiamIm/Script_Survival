// UI_dynamic.c
#define _CRT_SECURE_NO_WARNINGS
#include "UI_dynamic.h"	

void UI_dynamic_player_name_input(void)
{
	const int box_width = 28;
	const int box_height = 3;
	const int start_x = (WIDTH - box_width) / 2;
	const int start_y = (HEIGHT - box_height) / 2;

	for (int i = 0; i < box_height; i++) {
		utils_gotoxy(start_x, start_y + i); putchar('|');
		utils_gotoxy(start_x + box_width - 1, start_y + i); putchar('|');
	}

	for (int i = 0; i < box_width; i++) {
		utils_gotoxy(start_x + i, start_y); putchar('=');
		utils_gotoxy(start_x + i, start_y + box_height - 1); putchar('=');
	}
	

	const char* prompt = "플레이어 이름을 입력하세요 (최대 20자)";
	int px = (WIDTH - (int)strlen(prompt)) / 2;
	utils_gotoxy(px, start_y - 2);
	printf("%s", prompt);

	utils_gotoxy(start_x + 1, start_y + 1);
}

static void helper_confirm_player_name_selection(int title_state)
{
	const char* menu[] = {
		"Y. 예",
		"N. 아니오"
	};
	utils_set_color(COLOR_DEFAULT);
	for (int i = 0; i < 2; i++) {
		int mx = (WIDTH - (int)strlen(menu[i])) / 2;
		utils_gotoxy(mx, 20 + i * 2);
		if (title_state == i) {
			utils_set_color(COLOR_SELECT_MENU);
			printf("%s", menu[i]);
			utils_set_color(COLOR_DEFAULT);
		}
		else
		{
			printf("%s", menu[i]);
		}
	}
	utils_set_color(COLOR_DEFAULT_TEXT);
}

// 입력한 이름으로 할껀지 다시 물어보는 함수
bool UI_dynamic_confirm_player_name(const char* name)
{
	UI_cleaner_all_display();

	const int box_width = 28;
	const int box_height = 3;
	const int start_x = (WIDTH - box_width) / 2;
	const int start_y = (HEIGHT - box_height) / 2;

	for (int i = 0; i < box_height; i++) {
		utils_gotoxy(start_x, start_y + i); putchar('|');
		utils_gotoxy(start_x + box_width - 1, start_y + i); putchar('|');
	}

	for (int i = 0; i < box_width; i++) {
		utils_gotoxy(start_x + i, start_y); putchar('=');
		utils_gotoxy(start_x + i, start_y + box_height - 1); putchar('=');
	}

	const char* prompt_header = "입력하신 모험가님의 이름";
	int px = (WIDTH - (int)strlen(prompt_header)) / 2;
	utils_gotoxy(px, start_y - 2);
	printf("%s", prompt_header);

	utils_gotoxy(start_x + 1, start_y + 1);
	printf("%s", name);

	const char* prompt_confirm = "이 이름으로 시작하시겠습니까?";
	int pcx = (WIDTH - (int)strlen(prompt_confirm)) / 2;
	utils_gotoxy(pcx, start_y + 3);
	printf("%s", prompt_confirm);

	int state = 0; // 0: Y, 1: N
	helper_confirm_player_name_selection(state);

	while (1) {
		int key = _getch();
		if (key == EXTENDED_KEY) key = _getch();

		if (key == UP || key == DOWN) {
			state = !state; 
			helper_confirm_player_name_selection(state);
		}
		else if (key == ENTER) {
			return !state;
		}
	}
}

char* UI_dynamic_create_player_name(void)
{
	char* name = NULL;

	while (1)
	{
		UI_cleaner_all_display();

		utils_set_color(COLOR_DEFAULT_TEXT);
		UI_dynamic_player_name_input();

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

		if (UI_dynamic_confirm_player_name(name)) {
			break; // 이름이 확정되면 루프 종료
		}
		else {
			free(name); // 이름이 확정되지 않으면 메모리 해제 후 다시 입력 받기
			name = NULL;
		}
	}

	return name;
}

// =============================

void UI_dynamic_title_selection(int ui_title_state)
{
	const char* menu[] = {
		"1. 시작",
		"2. 옵션",
		"3. 종료"
	};
	utils_set_color(COLOR_DEFAULT);
	for (int i = 0; i < 3; i++) {
		int mx = (WIDTH - (int)strlen(menu[i])) / 2;
		utils_gotoxy(mx, 15 + i * 2);

		if (ui_title_state == i) {
			utils_set_color(COLOR_SELECT_MENU);
			printf("%s", menu[i]);
			utils_set_color(COLOR_DEFAULT);
		}
		else
		{
			printf("%s", menu[i]);
		}
	}

	utils_set_color(COLOR_DEFAULT_TEXT);
}

// 배틀 선택지 ui 그리는 함수
void UI_dynamic_battle_selection(int ui_battle_state)
{
	const char* menu[] = {
		"Attack",
		"Extortion",
		"Iventory / setting"
	};
	utils_set_color(COLOR_DEFAULT);

	int x = 3;
	int y = 22;
	for (int i = 0; i < 3; i++) {
		utils_gotoxy(x, y);

		if (ui_battle_state == i) {
			utils_set_color(COLOR_SELECT_MENU);
			printf("%s", menu[i]);
			utils_set_color(COLOR_DEFAULT);
		}
		else
		{
			printf("%s", menu[i]);
		}
		y++;
	}
	utils_set_color(COLOR_DEFAULT_TEXT);
}

void UI_dynamic_monster_info(monster_t* monster)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	int full_hp = monster->max_hp / 10;
	int current_hp = monster->current_hp / 10;
	int start_point = 72 - (full_hp / 2); // 체력바 시작 위치

	float hp_ratio = (float)monster->current_hp / monster->max_hp;

	int color; // 체력 비율에 따른 색깔 변환
	if (hp_ratio > 0.7f) color = COLOR_GREEN;
	else if (hp_ratio > 0.3f) color = COLOR_YELLOW;
	else if (hp_ratio > 0) color = COLOR_LIGHTRED;
	else color = COLOR_DARKGRAY;

	for (int i = 0; i < full_hp; i++) {
		utils_gotoxy(start_point + i, 2);
		if (i < current_hp) {
			SetConsoleTextAttribute(hConsole, color);
			printf("■");
		}
		else {
			SetConsoleTextAttribute(hConsole, COLOR_DARKGRAY);
			printf("□");
		}
	}

	SetConsoleTextAttribute(hConsole, COLOR_WHITE);	// 색상 초기화 (기본 흰색)

	for (int i = 0; i < 13; i++) { // 몬스터 이미지 출력
		utils_gotoxy(22, 4 + i);
		printf("%s", monster->image[i]);
	}

	utils_gotoxy(60, 19);
	SetConsoleTextAttribute(hConsole, 7);
	printf(" 지역 : ");
	SetConsoleTextAttribute(hConsole, 10);
	printf("숲");
	SetConsoleTextAttribute(hConsole, 7);
	printf("\t\t몬스터 : ");
	SetConsoleTextAttribute(hConsole, 12);
	printf("%s", monster->name);
	SetConsoleTextAttribute(hConsole, 7);
}

void UI_dynamic_player_info(player_t* player)
{
	utils_gotoxy(114, 21); printf("Player: %s", player->name);
	utils_gotoxy(114, 22);  printf("HP   : %d / %d", player->current_hp, player->max_hp);
	utils_gotoxy(114, 23);  printf("ATK  : %d", player->attack);
	utils_gotoxy(114, 24);  printf("SPD  : %d", player->speed);
	utils_gotoxy(114, 25);  printf("EVA  : %.2f%%", player->evasion_rate * 100);
	utils_gotoxy(114, 26);  printf("DEF  : %.2f%%", player->defence_rate * 100);
}

void UI_dynamic_inventory_info(player_t* player, int ui_inventory_state, int focus_level, int selected_item_index)
{
	UI_cleaner_inventory_item_list();
	UI_cleaner_inventory_item_description();

	typedef struct { int x; int y; const char* text; } TopMenuItem;
	const TopMenuItem top_items[] = {
		{3, 2, "◁---"}, {28, 2, "무기"}, {72, 2, "방어구"}, {115, 2, "소비 아이템"}
	};
	for (int i = 0; i < 5; i++) {
		// 포커스가 아이템 리스트에 있을 때, 현재 활성화된 카테고리를 노란색으로 표시
		if (focus_level == 1 && i == ui_inventory_state) {
			utils_set_color(COLOR_YELLOW);
		}
		// 포커스가 카테고리에 있을 때, 선택된 카테고리를 흰색으로 표시
		else if (focus_level == 0 && i == ui_inventory_state) {
			utils_set_color(COLOR_SELECT_MENU);
		}
		else {
			utils_set_color(COLOR_DEFAULT);
		}
		
		if (i == 4) {
			utils_gotoxy(143, 1); printf(" ()() ");
			utils_gotoxy(143, 2); printf("(====)");
			utils_gotoxy(143, 3); printf(" ()() ");
		}
		else {
			utils_gotoxy(top_items[i].x, top_items[i].y);
			printf("%s", top_items[i].text);
		}
	}

	UI_cleaner_inventory_item_list(); 

	equipment_t* current_equipment_list = NULL;
	int item_count = 0;

	if (ui_inventory_state == INVENTORY_STATE_WEAPON) {
		current_equipment_list = weapons;
		item_count = 10;
	}
	else if (ui_inventory_state == INVENTORY_STATE_ARMOR) {
		current_equipment_list = armors;
		item_count = 10;
	}

	if (current_equipment_list != NULL) {
		for (int i = 0; i < item_count; i++) {
			int x = (i < 5) ? 5 : 30; // 2열 배치
			int y = 6 + (i % 5);

			// 포커스가 아이템 리스트에 있고, 현재 아이템이 선택되었다면 흰색
			if (focus_level == 1 && i == selected_item_index) {
				utils_set_color(COLOR_SELECT_MENU);
				UI_cleaner_inventory_item_description(); 
				utils_gotoxy(80, 6);
				printf("%s", current_equipment_list[i].description);
			}
			else {
				utils_set_color(COLOR_DEFAULT_TEXT);
			}
			utils_gotoxy(x, y);
			printf("* %s", current_equipment_list[i].name);
		}
	}

	utils_set_color(COLOR_DEFAULT_TEXT);
	UI_dynamic_player_info(player);
}