// UI_dynamic.c
#define _CRT_SECURE_NO_WARNINGS
#include "UI_dynamic.h"	

static void s_print_diff_stat(int diff_stat, int x, int y)
{
	utils_gotoxy(x, y);
	printf("MAX HP : ");
	if (diff_stat > 0) utils_set_color(COLOR_STAT_UP);
	else utils_set_color(COLOR_STAT_DOWN);
	printf("%+d", diff_stat);
	utils_set_color(COLOR_DEFAULT_TEXT);
}

// type 0: 무기, 1: 방어구
static void s_print_stat_bonus(equipment_t* current_equipment_list, player_t* player, int index, int y, int type)
{
	equipment_t* selected_item = &current_equipment_list[index];
	equipment_t* current_equipped_item = NULL;

	UI_cleaner_stat_bonus_display();

	if (type == 0) { // 무기
		if (player->weapon_index != -1) {
			current_equipped_item = &weapons[player->weapon_index];
		}
	}
	else { // 방어구 (type == 1)
		if (player->armor_index != -1) {
			current_equipped_item = &armors[player->armor_index];
		}
	}

	int diff_x = 78;
	int diff_y = 22;

	// --- 최대 체력 ---
	int selected_hp = selected_item->max_hp_bonus;
	int current_hp = (current_equipped_item != NULL) ? current_equipped_item->max_hp_bonus : 0;
	if (selected_hp != 0 || current_hp != 0) {
		utils_gotoxy(79, ++y);
		int stat_diff = selected_hp - current_hp;
		printf("최대 체력 %+d", selected_hp);

		if (stat_diff != 0) {
			utils_gotoxy(diff_x, diff_y++);
			printf("MAX HP : ");
			if (stat_diff > 0) utils_set_color(COLOR_STAT_UP);
			else utils_set_color(COLOR_STAT_DOWN);
			printf("%+d", stat_diff);
			utils_set_color(COLOR_DEFAULT_TEXT);
		}
	}

	// --- 공격력 ---
	int selected_attack = selected_item->attack_bonus;
	int current_attack = (current_equipped_item != NULL) ? current_equipped_item->attack_bonus : 0;
	if (selected_attack != 0 || current_attack != 0) {
		utils_gotoxy(79, ++y);
		int stat_diff = selected_attack - current_attack;
		printf("공격력 %+d", selected_attack);

		if (stat_diff != 0) {
			utils_gotoxy(diff_x, diff_y++);
			printf("ATK    : ");
			if (stat_diff > 0) utils_set_color(COLOR_STAT_UP);
			else utils_set_color(COLOR_STAT_DOWN);
			printf("%+d", stat_diff);
			utils_set_color(COLOR_DEFAULT_TEXT);
		}
	}

	// --- 속도 ---
	int selected_speed = selected_item->speed_bonus;
	int current_speed = (current_equipped_item != NULL) ? current_equipped_item->speed_bonus : 0;
	if (selected_speed != 0 || current_speed != 0) {
		utils_gotoxy(79, ++y);
		int stat_diff = selected_speed - current_speed;
		printf("속도 %+d", selected_speed);

		if (stat_diff != 0) {
			utils_gotoxy(diff_x, diff_y++);
			printf("SPD    : ");
			if (stat_diff > 0) utils_set_color(COLOR_STAT_UP);
			else utils_set_color(COLOR_STAT_DOWN);
			printf("%+d", stat_diff);
			utils_set_color(COLOR_DEFAULT_TEXT);
		}
	}

	// --- 회피율 ---
	double selected_evasion = selected_item->evasion_bonus;
	double current_evasion = (current_equipped_item != NULL) ? current_equipped_item->evasion_bonus : 0.0;
	if (selected_evasion != 0.0 || current_evasion != 0.0) {
		utils_gotoxy(79, ++y);
		double stat_diff = selected_evasion - current_evasion;
		printf("회피율 %+.2f%%", selected_evasion * 100);

		if (stat_diff != 0.0) {
			utils_gotoxy(diff_x, diff_y++);
			printf("EVA    : ");
			if (stat_diff > 0) utils_set_color(COLOR_STAT_UP);
			else utils_set_color(COLOR_STAT_DOWN);
			printf("%+.2f%%", stat_diff * 100);
			utils_set_color(COLOR_DEFAULT_TEXT);
		}
	}

	// --- 방어율 ---
	double selected_defence = selected_item->defence_bonus;
	double current_defence = (current_equipped_item != NULL) ? current_equipped_item->defence_bonus : 0.0;
	if (selected_defence != 0.0 || current_defence != 0.0) {
		utils_gotoxy(79, ++y);
		double stat_diff = selected_defence - current_defence;
		printf("방어율 %+.2f%%", selected_defence * 100);

		if (stat_diff != 0.0) {
			utils_gotoxy(diff_x, diff_y++);
			printf("DEF    : ");
			if (stat_diff > 0) utils_set_color(COLOR_STAT_UP);
			else utils_set_color(COLOR_STAT_DOWN);
			printf("%+.2f%%", stat_diff * 100);
			utils_set_color(COLOR_DEFAULT_TEXT);
		}
	}
}
	
// type 0: 무기, 1: 방어구
static void s_print_inventory_item_page(
	equipment_t* current_equipment_list, pair_t* inventory, region_t current_region,
	player_t* player, focus_level_t focus_level, int selected_item_index, int page, int type
)
{
	int region_start_index = current_region * ITEMS_PER_REGION;
	int start = region_start_index + (page * ITEMS_PER_PAGE);
	int end = start + ITEMS_PER_PAGE;
	if (end > region_start_index + ITEMS_PER_REGION) end = region_start_index + ITEMS_PER_REGION;

	for (int i = start; i < end; i++) {
		int local_index_on_page = i - (region_start_index + page * ITEMS_PER_PAGE);
		int x = (local_index_on_page < ITEMS_PER_ROW) ? 13 : 45;
		int y = 6 + (local_index_on_page % ITEMS_PER_ROW) * 4;

		if (focus_level == FOCUS_LEVEL_ITEM_LIST && i == selected_item_index) {
			utils_set_color(COLOR_SELECT_MENU);
			UI_cleaner_inventory_item_description();
			utils_gotoxy(79, 6);

			if (inventory[i].is_was_having == FALSE) {
				printf("획득하지 않은 아이템입니다.");
			}
			else {
				printf("%s", current_equipment_list[i].description);
				s_print_stat_bonus(current_equipment_list, player, i, 7, type);
			}
		}
		else {
			utils_set_color(COLOR_DEFAULT);
		}

		utils_gotoxy(x, y);
		printf("* ");
		if (inventory[i].is_was_having) {
			printf("%s (%d)", current_equipment_list[i].name, inventory[i].count);
		}
		else {
			printf("미획득");
		}
	}

	utils_set_color(COLOR_DEFAULT_TEXT);
	utils_gotoxy(35, 17);
	printf("%d / %d", page + 1, ITEMS_PER_REGION / ITEMS_PER_PAGE);
}

// [수정]
static void s_print_store_item_page(
	equipment_t* current_equipment_list, pair_t* inventory, region_t current_region,
	player_t* player, focus_level_t focus_level, int selected_item_index,
	store_state_t buy_sell_state, int page, int type
)
{
	menu_list buy_sell_menu[] = { { 15, 23, "구매하기" }, { 53, 23, "판매하기" }, };
	int region_start_index = current_region * ITEMS_PER_REGION;
	int start = region_start_index + (page * ITEMS_PER_PAGE);
	int end = start + ITEMS_PER_PAGE;
	if (end > region_start_index + ITEMS_PER_REGION) end = region_start_index + ITEMS_PER_REGION;

	for (int i = start; i < end; i++) {
		int local_index_on_page = i - (region_start_index + page * ITEMS_PER_PAGE);
		int x = (local_index_on_page < ITEMS_PER_ROW) ? 13 : 45;
		int y = 6 + (local_index_on_page % ITEMS_PER_ROW) * 4;

		if ((focus_level == FOCUS_LEVEL_ITEM_LIST || focus_level == FOCUS_LEVEL_ITEM_BUY_SELL) && i == selected_item_index) {
			UI_cleaner_inventory_item_description();
			utils_gotoxy(79, 6);
			utils_set_color(COLOR_DEFAULT_TEXT);

			printf("%s", current_equipment_list[i].description);
			s_print_stat_bonus(current_equipment_list, player, i, 7, type);

			for (int j = 0; j < 2; j++) {
				if (focus_level == FOCUS_LEVEL_ITEM_BUY_SELL && buy_sell_state == (j == 0 ? STORE_STATE_BUY : STORE_STATE_SELL)) {
					utils_set_color(COLOR_SELECT_MENU);
				}
				else {
					utils_set_color(COLOR_DEFAULT);
				}
				utils_gotoxy(buy_sell_menu[j].x, buy_sell_menu[j].y);
				printf("%s", buy_sell_menu[j].text);

				char price_buf[32];
				int price = (j == 0) ? current_equipment_list[i].buy_price : current_equipment_list[i].sell_price;
				snprintf(price_buf, sizeof(price_buf), "%d C", price);

				int text_len = (int)strlen(buy_sell_menu[j].text);
				int len_price = (int)strlen(price_buf);
				int offset = (text_len - len_price) / 2;

				utils_gotoxy(buy_sell_menu[j].x + offset, buy_sell_menu[j].y + 2);
				printf("%s", price_buf);
			}
			utils_set_color(COLOR_SELECT_MENU);
		}
		else {
			utils_set_color(COLOR_DEFAULT);
		}

		if (focus_level == FOCUS_LEVEL_ITEM_BUY_SELL && i == selected_item_index) {
			utils_set_color(COLOR_YELLOW);
		}
		else if (focus_level != FOCUS_LEVEL_ITEM_LIST) {
			utils_set_color(COLOR_DEFAULT);
		}

		utils_gotoxy(x, y);
		printf("* %s (%d)", current_equipment_list[i].name, inventory[i].count);
	}

	utils_set_color(COLOR_DEFAULT_TEXT);
	utils_gotoxy(35, 17);
	printf("%d / %d", page + 1, ITEMS_PER_REGION / ITEMS_PER_PAGE);

	int coin_start = 111;
	int coin_end = 151;
	char coin_buf[32];
	snprintf(coin_buf, sizeof(coin_buf), "%d C", player->coin);
	int len = (int)strlen(coin_buf);
	int offset = coin_start + ((coin_end - coin_start + 1) - len) / 2;
	utils_gotoxy(offset, 23);
	printf("%s", coin_buf);
}

static void s_print_sub_menu_box(const menu_list menus[], focus_level_t focus_level, region_t current_region)
{
	utils_set_color(COLOR_DEFAULT_TEXT);
	for (int y = 5; y < 18; y++) {
		utils_gotoxy(9, y); putchar('|');
	}
	for (int x = 1; x < 9; x++) {
		utils_gotoxy(x, 9); putchar('=');
		utils_gotoxy(x, 13); putchar('=');
	}

	for (int i = 0; i < REGION_COUNT; i++) {
		if ((focus_level == FOCUS_LEVEL_ITEM_LIST || focus_level == FOCUS_LEVEL_ITEM_BUY_SELL) && current_region == i) {
			utils_set_color(COLOR_YELLOW);
		}
		else if (focus_level == FOCUS_LEVEL_SUB && current_region == i) {
			utils_set_color(COLOR_SELECT_MENU);
		}
		else {
			utils_set_color(COLOR_DEFAULT);
		}
		utils_gotoxy(menus[i].x, menus[i].y);
		printf("%s", menus[i].text);
	}
}

static void s_confirm_player_name_selection(int title_state)
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

// ------------------------------

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
	s_confirm_player_name_selection(state);

	while (1) {
		int key = _getch();
		if (key == EXTENDED_KEY) key = _getch();

		if (key == UP || key == DOWN) {
			state = !state;
			s_confirm_player_name_selection(state);
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

	int full_hp = monster->max_hp / 100;
	int current_hp = monster->current_hp / 100;
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
	utils_gotoxy(114, 21);  printf("Name : %s", player->name);
	utils_gotoxy(114, 22);  printf("HP   : %d / %d", player->current_hp, player->max_hp);
	utils_gotoxy(114, 23);  printf("ATK  : %d", player->attack);
	utils_gotoxy(114, 24);  printf("SPD  : %d", player->speed);
	utils_gotoxy(114, 25);  printf("EVA  : %.2f%%", player->evasion_rate * 100);
	utils_gotoxy(114, 26);  printf("DEF  : %.2f%%", player->defence_rate * 100);
}


// =========================

// [수정] 
void UI_dynamic_inventory_info(
	player_t* player, pair_t* weapon_inventory, pair_t* armor_inventory,
	inventory_state_t current_inventory_state, region_t current_region, focus_level_t focus_level,
	int selected_item_index, int weapon_page, int armor_page
)
{
	const menu_list top_items[] = {
		{3, 2, "◁---"}, {28, 2, "무기"}, {72, 2, "방어구"}, {115, 2, "소비 아이템"}, { 144, 2, "옵션"}
	};
	const menu_list sub_menu[] = {
		{4, 7, "숲"}, {3, 11, "사막"}, {3, 15, "설원"}
	};

	for (int i = 0; i < 5; i++) {
		if ((focus_level == FOCUS_LEVEL_ITEM_LIST || focus_level == FOCUS_LEVEL_SUB) && i == current_inventory_state) {
			utils_set_color(COLOR_YELLOW);
		}
		else if (focus_level == FOCUS_LEVEL_TOP && i == current_inventory_state) {
			utils_set_color(COLOR_SELECT_MENU);
		}
		else {
			utils_set_color(COLOR_DEFAULT);
		}
		utils_gotoxy(top_items[i].x, top_items[i].y);
		printf("%s", top_items[i].text);
	}

	UI_cleaner_inventory_item_list();
	UI_cleaner_inventory_item_description();

	if (current_inventory_state == INVENTORY_STATE_WEAPON) {
		s_print_sub_menu_box(sub_menu, focus_level, current_region);
		s_print_inventory_item_page(weapons, weapon_inventory, current_region, player, focus_level, selected_item_index, weapon_page, 0);
	}
	else if (current_inventory_state == INVENTORY_STATE_ARMOR) {
		s_print_sub_menu_box(sub_menu, focus_level, current_region);
		s_print_inventory_item_page(armors, armor_inventory, current_region, player, focus_level, selected_item_index, armor_page, 1);
	}
	else if (current_inventory_state == INVENTORY_STATE_HEAL_ITEM) {
		// ... 소모품 그리는 로직 ...
	}
	else {
		UI_cleaner_sub_menu();
	}

	utils_set_color(COLOR_DEFAULT_TEXT);
	UI_dynamic_player_info(player);
}

// [수정] 
void UI_dynamic_store_info(
	player_t* player, pair_t* weapon_inventory, pair_t* armor_inventory,
	store_state_t current_store_state, region_t current_region, focus_level_t focus_level,
	int selected_item_index, store_state_t buy_sell_state, int weapon_page, int armor_page
)
{
	menu_list store_menu[] = {
		{3, 2, "◁---"}, { 30, 2, "무기" }, {77, 2, "방어구"}, {121, 2, "소비 아이템"}
	};
	const menu_list sub_menu[] = {
		{4, 7, "숲"}, {3, 11, "사막"}, {3, 15, "설원"}
	};

	for (int i = 0; i < 4; i++) {
		if ((focus_level == FOCUS_LEVEL_ITEM_LIST || focus_level == FOCUS_LEVEL_SUB || focus_level == FOCUS_LEVEL_ITEM_BUY_SELL) && i == current_store_state) {
			utils_set_color(COLOR_YELLOW);
		}
		else if (focus_level == FOCUS_LEVEL_TOP && i == current_store_state) {
			utils_set_color(COLOR_SELECT_MENU);
		}
		else {
			utils_set_color(COLOR_DEFAULT);
		}
		utils_gotoxy(store_menu[i].x, store_menu[i].y);
		printf("%s", store_menu[i].text);
	}

	UI_cleaner_inventory_item_list();
	UI_cleaner_inventory_item_description();

	if (current_store_state == STORE_STATE_WEAPON) {
		s_print_sub_menu_box(sub_menu, focus_level, current_region);
		s_print_store_item_page(weapons, weapon_inventory, current_region, player, focus_level, selected_item_index, buy_sell_state, weapon_page, 0);
	}
	else if (current_store_state == STORE_STATE_ARMOR) {
		s_print_sub_menu_box(sub_menu, focus_level, current_region);
		s_print_store_item_page(armors, armor_inventory, current_region, player, focus_level, selected_item_index, buy_sell_state, armor_page, 1);
	}
	else if (current_store_state == STORE_STATE_HEAL_ITEM) {
		// ... 소모품 그리는 로직 ...
	}
	else {
		UI_cleaner_sub_menu();
	}
	utils_set_color(COLOR_DEFAULT_TEXT);
}

// =========================

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
	int padding = start_x + (end_x - start_x - len) / 2;
	utils_gotoxy(padding, 23);
	printf("%s", armor->name);
}