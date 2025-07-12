// UI_dynamic.c
#define _CRT_SECURE_NO_WARNINGS
#include "UI_dynamic.h"	

static void s_print_stat_bonus(equipment_t* current_equipment_list, player_t* player, int index, int y)
{
	if (current_equipment_list[index].attack_bonus > 0) {
		utils_gotoxy(80, ++y);

		// TODO: 현재 장착한 장비의 공격력과 비교하여 차이를 출력
		int stat_diff = current_equipment_list[index].attack_bonus;

		printf("공격력 +%d (%+d)", current_equipment_list[index].attack_bonus, stat_diff);
	}

	if (current_equipment_list[index].max_hp_bonus > 0) {
		utils_gotoxy(80, ++y);
		printf("최대 체력 +%d", current_equipment_list[index].max_hp_bonus);
	}

	if (current_equipment_list[index].speed_bonus > 0) {
		utils_gotoxy(80, ++y);
		printf("속도 +%d", current_equipment_list[index].speed_bonus);
	}

	if (current_equipment_list[index].evasion_bonus > 0) {
		utils_gotoxy(80, ++y);
		printf("회피율 +%.2f%%", current_equipment_list[index].evasion_bonus * 100);
	}

	if (current_equipment_list[index].defence_bonus > 0) {
		utils_gotoxy(80, ++y);
		printf("방어율 +%.2f%%", current_equipment_list[index].defence_bonus * 100);
	}
}

// type 0: 무기, 1: 방어구
static void s_print_item_page(equipment_t* current_equipment_list, player_t* player, int focus_level, int selected_item_index, int list_count, int page, int type)
{
	int start = page * 6;
	int end = start + 6;
	if (end > list_count) end = list_count;

	for (int i = start; i < end; i++) {
		int x = (i < ITEMS_PER_PAGE * page + 3) ? 13 : 45; // 각 페이지 마다 2열로 배치
		int y = 6 + (i % 3) * 4;

		// 포커스가 아이템 리스트에 있고, 현재 아이템이 선택되었다면 흰색
		if (focus_level == INVENTORY_FOCUS_LEVEL_ITEM_LIST && i == selected_item_index) {
			utils_set_color(COLOR_SELECT_MENU);
			UI_cleaner_inventory_item_description();
			utils_gotoxy(80, 6);
			// 아이템 설명 출력
			// 획득한 적이 없는 아이템은 설명 출력 x

			// 가져온 current_equipment_list[i]의 아이템이 무기인지 방어구인지 확인
			if (type == 0) {
				if (weapon_inventory[i].is_was_having == FALSE) {
					printf("획득하지 않은 무기입니다.");
				}
				else {
					printf("%s", current_equipment_list[i].description);
					int my = 7;
					s_print_stat_bonus(current_equipment_list, player, i, my);
				}
			}
			else if (type == 1) {
				if (armor_inventory[i].is_was_having == FALSE) {
					printf("획득하지 않은 방어구입니다.");
				}
				else {
					printf("%s", current_equipment_list[i].description);
					int my = 7;
					s_print_stat_bonus(current_equipment_list, player, i, my);
				}
			}
		}
		else {
			utils_set_color(COLOR_DEFAULT);
		}
		utils_gotoxy(x, y);
		printf("* ");
		if (type == 0) {
			if (weapon_inventory[i].is_was_having == FALSE) {
				printf(" ");
			}
			else {
				printf("%s", current_equipment_list[i].name);
			}
		}
		else if (type == 1) {
			if (armor_inventory[i].is_was_having == FALSE) {
				printf(" ");
			}
			else {
				printf("%s", current_equipment_list[i].name);
			}
		}
	}

	utils_set_color(COLOR_DEFAULT_TEXT);
	utils_gotoxy(35, 17);
	printf("%d / %d", page + 1, (list_count + 5) / 6);
}

static void s_print_sub_menu_box(void)
{
	utils_set_color(COLOR_DEFAULT_TEXT);
	for (int y = 5; y < 18; y++) {
		utils_gotoxy(9, y); putchar('|');
	}

	for (int x = 1; x < 9; x++) {
		utils_gotoxy(x, 9); putchar('=');
		utils_gotoxy(x, 13); putchar('=');
	}
}

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

void UI_dynamic_inventory_info(player_t* player, int ui_inventory_state, int focus_level, int selected_item_index, int weapon_page, int armor_page)
{
	UI_cleaner_inventory_item_list();
	UI_cleaner_inventory_item_description();

	typedef struct { int x; int y; const char* text; } top_menu_list;
	typedef struct { int x; int y; const char* text; } sub_menu_list;
	const top_menu_list top_items[] = {
		{3, 2, "◁---"}, {28, 2, "무기"}, {72, 2, "방어구"}, {115, 2, "소비 아이템"}
	};

	const sub_menu_list sub_items[] = {
		{4, 7, "숲"}, {3, 11, "사막"}, {3, 15, "설원"}
	};

	for (int i = 0; i < 5; i++) {
		// 포커스가 아이템 리스트에 있을 때, 현재 활성화된 카테고리를 노란색으로 표시
		if (focus_level == INVENTORY_FOCUS_LEVEL_ITEM_LIST && i == ui_inventory_state) {
			utils_set_color(COLOR_YELLOW);
		}
		// 포커스가 카테고리에 있을 때, 선택된 카테고리를 흰색으로 표시
		else if (focus_level == INVENTORY_FOCUS_LEVEL_TOP && i == ui_inventory_state) {
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
	int type = -1; // default

	if (ui_inventory_state == INVENTORY_STATE_WEAPON) {
		s_print_sub_menu_box();	
		current_equipment_list = weapons;
		item_count = sizeof(weapons) / sizeof(weapons[0]);
		type = 0;

		s_print_item_page(current_equipment_list, player, focus_level, selected_item_index, item_count, weapon_page, type);
	}
	else if (ui_inventory_state == INVENTORY_STATE_ARMOR) {
		s_print_sub_menu_box();
		current_equipment_list = armors;
		item_count = sizeof(armors) / sizeof(armors[0]);
		type = 1;

		s_print_item_page(current_equipment_list, player, focus_level, selected_item_index, item_count, armor_page, type);
	}
	else if (ui_inventory_state == INVENTORY_STATE_HEAL_ITEM) {
		UI_cleaner_sub_menu();
		item_count = 5;

		for (int i = 0; i < item_count; i++) {
			int x = (i < 3) ? 13 : 45;
			int y = 6 + (i % 3) * 4;

			// 포커스가 아이템 리스트에 있고, 현재 아이템이 선택되었다면 흰색
			if (focus_level == 1 && i == selected_item_index) {
				utils_set_color(COLOR_SELECT_MENU);
				UI_cleaner_inventory_item_description();
				utils_gotoxy(80, 6);
				// 아이템 설명 출력
				// 획득한 적이 없는 아이템은 설명 출력 x
				printf("%s", heal_items[i].description);

			}
			else {
				utils_set_color(COLOR_DEFAULT);
			}
			utils_gotoxy(x, y);
			printf("* %s", heal_items[i].name);
			
		}
		
	}
	else {
		UI_cleaner_sub_menu();
		UI_cleaner_inventory_item_list();
		UI_cleaner_inventory_item_description();
	}


	utils_set_color(COLOR_DEFAULT_TEXT);
	UI_dynamic_player_info(player);
}

void UI_dynamic_current_weapon_info(player_t* player)
{
	int start_x = 0;
	int end_x = 38;
	if (player->weapon_index == -1) {
		char* no_weapon_msg = "현재 장착된 무기가 없습니다.";
		int msg_len = (int)strlen(no_weapon_msg);
		int padding = (end_x - start_x - msg_len) / 2;
		utils_gotoxy(padding, 23);
		printf("%s", no_weapon_msg);
		return;
	}

	equipment_t* weapon = &weapons[player->weapon_index];
	int len = (int)strlen(weapon->name);
	int padding = start_x + (end_x - start_x - len) / 2;
	utils_gotoxy(padding, 23);
	printf("%s", weapon->name);
}

void UI_dynamic_current_armor_info(player_t* player)
{
	int start_x = 38;
	int end_x = 75;

	if (player->armor_index == -1) {
		char* no_armor_msg = "현재 장착된 방어구가 없습니다.";
		int msg_len = (int)strlen(no_armor_msg);
		int padding = (end_x - start_x - msg_len) / 2;
		utils_gotoxy(padding, 23);
		printf("%s", no_armor_msg);
		return;
	}

	equipment_t* armor = &armors[player->armor_index];
	int len = (int)strlen(armor->name);
	// 동적으로 중앙 정렬
	int padding = start_x + (end_x - start_x - len) / 2;
	utils_gotoxy(padding, 23);
	printf("%s", armor->name);
}