// UI_dynamic.c
#define _CRT_SECURE_NO_WARNINGS
#include "UI_dynamic.h"   
#include "item.h"
#include "save_load.h"

#define BAR_LENGTH 50

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
static void s_print_stat_bonus(
    equipment_rarity_t rarity,
    equipment_t current_equipment_list[RARITY_COUNT][ITEM_COUNT],
    player_t* player,
    int index, int y, int type
) {
    equipment_t selected_item = current_equipment_list[rarity][index];
    equipment_t* current_equipped_item = NULL;

    UI_cleaner_stat_bonus_display();

    if (type == 0) { // 무기
        if (player->weapon_index != -1) {
            current_equipped_item = &weapons[player->weapon_rarity][player->weapon_index];
        }
    }
    else { // 방어구
        if (player->armor_index != -1) {
            current_equipped_item = &armors[player->armor_rarity][player->armor_index];
        }
    }

    int diff_x = 78;
    int diff_y = 22;

    if (type == 0) { // 무기
        // --- 공격력 ---
        int selected_attack = selected_item.attack_bonus;
        int current_attack = current_equipped_item ? current_equipped_item->attack_bonus : 0;
        if (selected_attack != 0 || current_attack != 0) {
            utils_gotoxy(79, ++y);
            int stat_diff = selected_attack - current_attack;
            printf("공격력 %+d", selected_attack);
            if (stat_diff != 0) {
                utils_gotoxy(diff_x, diff_y++);
                printf("ATK    : ");
                utils_set_color(stat_diff > 0 ? COLOR_STAT_UP : COLOR_STAT_DOWN);
                printf("%+d", stat_diff);
                utils_set_color(COLOR_DEFAULT_TEXT);
            }
        }

        // --- 치명타 확률 ---
        double selected_crit_chance = selected_item.crit_chance_bonus;
        double current_crit_chance = current_equipped_item ? current_equipped_item->crit_chance_bonus : 0.0;
        if (selected_crit_chance != 0.0 || current_crit_chance != 0.0) {
            utils_gotoxy(79, ++y);
            double stat_diff = selected_crit_chance - current_crit_chance;
            printf("치명타 확률 %+.2f%%", selected_crit_chance * 100);
            if (stat_diff != 0.0) {
                utils_gotoxy(diff_x, diff_y++);
                printf("CRIT CH : ");
                utils_set_color(stat_diff > 0 ? COLOR_STAT_UP : COLOR_STAT_DOWN);
                printf("%+.2f%%", stat_diff * 100);
                utils_set_color(COLOR_DEFAULT_TEXT);
            }
        }

        // --- 치명타 피해 ---
        double selected_crit_dmg = selected_item.crit_damage_bonus;
        double current_crit_dmg = current_equipped_item ? current_equipped_item->crit_damage_bonus : 0.0;
        if (selected_crit_dmg != 0.0 || current_crit_dmg != 0.0) {
            utils_gotoxy(79, ++y);
            double stat_diff = selected_crit_dmg - current_crit_dmg;
            printf("치명타 피해 %+.2f%%", selected_crit_dmg * 100);
            if (stat_diff != 0.0) {
                utils_gotoxy(diff_x, diff_y++);
                printf("CRIT DMG: ");
                utils_set_color(stat_diff > 0 ? COLOR_STAT_UP : COLOR_STAT_DOWN);
                printf("%+.2f%%", stat_diff * 100);
                utils_set_color(COLOR_DEFAULT_TEXT);
            }
        }

        // --- 격파 추가 피해 ---
        int selected_break_extra = selected_item.break_extra_damage_bonus;
        int current_break_extra = current_equipped_item ? current_equipped_item->break_extra_damage_bonus : 0;
        if (selected_break_extra != 0 || current_break_extra != 0) {
            utils_gotoxy(79, ++y);
            int stat_diff = selected_break_extra - current_break_extra;
            printf("격파 추가 피해 %+d", selected_break_extra);
            if (stat_diff != 0) {
                utils_gotoxy(diff_x, diff_y++);
                printf("BRK DMG : ");
                utils_set_color(stat_diff > 0 ? COLOR_STAT_UP : COLOR_STAT_DOWN);
                printf("%+d", stat_diff);
                utils_set_color(COLOR_DEFAULT_TEXT);
            }
        }
    }
    else if (type == 1) { // 방어구
        // --- 최대 체력 ---
        int selected_hp = selected_item.max_hp_bonus;
        int current_hp = current_equipped_item ? current_equipped_item->max_hp_bonus : 0;
        if (selected_hp != 0 || current_hp != 0) {
            utils_gotoxy(79, ++y);
            int stat_diff = selected_hp - current_hp;
            printf("최대 체력 %+d", selected_hp);
            if (stat_diff != 0) {
                utils_gotoxy(diff_x, diff_y++);
                printf("MAX HP : ");
                utils_set_color(stat_diff > 0 ? COLOR_STAT_UP : COLOR_STAT_DOWN);
                printf("%+d", stat_diff);
                utils_set_color(COLOR_DEFAULT_TEXT);
            }
        }

        // --- 속도 ---
        int selected_speed = selected_item.speed_bonus;
        int current_speed = current_equipped_item ? current_equipped_item->speed_bonus : 0;
        if (selected_speed != 0 || current_speed != 0) {
            utils_gotoxy(79, ++y);
            int stat_diff = selected_speed - current_speed;
            printf("속도 %+d", selected_speed);
            if (stat_diff != 0) {
                utils_gotoxy(diff_x, diff_y++);
                printf("SPD    : ");
                utils_set_color(stat_diff > 0 ? COLOR_STAT_UP : COLOR_STAT_DOWN);
                printf("%+d", stat_diff);
                utils_set_color(COLOR_DEFAULT_TEXT);
            }
        }

        // --- 회피율 ---
        double selected_evasion = selected_item.evasion_bonus;
        double current_evasion = current_equipped_item ? current_equipped_item->evasion_bonus : 0.0;
        if (selected_evasion != 0.0 || current_evasion != 0.0) {
            utils_gotoxy(79, ++y);
            double stat_diff = selected_evasion - current_evasion;
            printf("회피율 %+.2f%%", selected_evasion * 100);
            if (stat_diff != 0.0) {
                utils_gotoxy(diff_x, diff_y++);
                printf("EVA    : ");
                utils_set_color(stat_diff > 0 ? COLOR_STAT_UP : COLOR_STAT_DOWN);
                printf("%+.2f%%", stat_diff * 100);
                utils_set_color(COLOR_DEFAULT_TEXT);
            }
        }

        // --- 방어율 ---
        double selected_defence = selected_item.defence_bonus;
        double current_defence = current_equipped_item ? current_equipped_item->defence_bonus : 0.0;
        if (selected_defence != 0.0 || current_defence != 0.0) {
            utils_gotoxy(79, ++y);
            double stat_diff = selected_defence - current_defence;
            printf("방어율 %+.2f%%", selected_defence * 100);
            if (stat_diff != 0.0) {
                utils_gotoxy(diff_x, diff_y++);
                printf("DEF    : ");
                utils_set_color(stat_diff > 0 ? COLOR_STAT_UP : COLOR_STAT_DOWN);
                printf("%+.2f%%", stat_diff * 100);
                utils_set_color(COLOR_DEFAULT_TEXT);
            }
        }
    }
}

// type 0: 무기, 1: 방어구
static void s_print_inventory_item_page(
    equipment_t current_equipment_list[RARITY_COUNT][ITEM_COUNT], pair_t inventory[RARITY_COUNT][ITEM_COUNT], equipment_rarity_t current_rarity,
    player_t* player, focus_level_t focus_level, int selected_item_index, int page, int type
)
{
    int total_items = rarity_item_counts[current_rarity];
    int items_per_page = ITEMS_PER_PAGE;
    int total_pages = total_items / items_per_page;

    // 페이지 범위 보정
    if (page < 0) page = 0;
    if (page >= total_pages) page = total_pages - 1;

    // 현재 페이지에 속한 아이템만 출력
    int start = page * items_per_page;
    int end = start + items_per_page;
    if (end > total_items) end = total_items;

    for (int i = start; i < end; i++) {
        int local_index = i - start;  // 0 ~ items_per_page-1
        int x = (local_index < ITEMS_PER_ROW) ? 13 : 45;
        int y = 6 + (local_index % ITEMS_PER_ROW) * 4;

        // 선택된 아이템 강조 및 설명 표시
        if (focus_level == FOCUS_LEVEL_ITEM_LIST && i == selected_item_index) {
            utils_set_color(COLOR_SELECT_MENU);
            UI_cleaner_inventory_item_description();
            utils_gotoxy(79, 6);

            if (!inventory[current_rarity][i].is_was_having) {
                printf("획득하지 않은 아이템입니다.");
            }
            else {
                printf("%s", current_equipment_list[current_rarity][i].description);
                s_print_stat_bonus(current_rarity, current_equipment_list, player, i, 7, type);

                if (current_rarity == RARITY_UNIQUE) {
                    // 세트 효과가 적용 되면 흰색, 아니면 회색
                    if (player->weapon_index == player->armor_index &&
						player->weapon_index == selected_item_index &&
                        player->weapon_rarity == RARITY_UNIQUE && player->armor_rarity == RARITY_UNIQUE) {
						utils_set_color(COLOR_YELLOW);
                    }
                    else {
						utils_set_color(COLOR_DEFAULT);
                    }
                    utils_gotoxy(79, 14);
                    printf("%s 세트 효과", set_effects[selected_item_index].name);
                    utils_gotoxy(79, 15);
                    printf("%s", set_effects[selected_item_index].description);

                    utils_set_color(COLOR_DEFAULT_TEXT);
                }
            }
        }
        else {
            utils_set_color(COLOR_DEFAULT);
        }

        // 아이템 리스트 출력
        utils_gotoxy(x, y);
        printf("* ");
        if (inventory[current_rarity][i].is_was_having) {
            printf("%s (%d)", current_equipment_list[current_rarity][i].name,
                inventory[current_rarity][i].count);
        }
        else {
            printf("미획득");
        }
    }

    // 페이지 정보 출력
    utils_set_color(COLOR_DEFAULT_TEXT);
    utils_gotoxy(35, 17);
    printf("%d / %d", page + 1, total_pages);
}

static void s_print_heal_item_list(player_t* player, focus_level_t focus_level, int selected_item_index)
{
    for (int i = 0; i < HEAL_ITEM_COUNT; i++) {
        int local_index_on_page = i;
        int x = (local_index_on_page < ITEMS_PER_ROW) ? 13 : 45;
        int y = 6 + (local_index_on_page % ITEMS_PER_ROW) * 4;

        if (focus_level == FOCUS_LEVEL_ITEM_LIST && i == selected_item_index) {
            utils_set_color(COLOR_SELECT_MENU);

            UI_cleaner_inventory_item_description();
            utils_gotoxy(79, 6);
            utils_set_color(COLOR_DEFAULT_TEXT);
            printf("%s", heal_items[i].description);

            utils_set_color(COLOR_SELECT_MENU);
        }
        else {
            utils_set_color(COLOR_DEFAULT);
        }

        utils_gotoxy(x, y);
        printf("* %s (%d)", heal_items[i].name, heal_item_inventory[i]);
    }

    utils_set_color(COLOR_DEFAULT_TEXT);
}

static void s_print_store_item_page(
    equipment_t current_equipment_list[RARITY_COUNT][ITEM_COUNT], pair_t inventory[RARITY_COUNT][ITEM_COUNT], equipment_rarity_t current_rarity,
    player_t* player, focus_level_t focus_level, int selected_item_index,
    store_state_t buy_sell_state, int page, int type
)
{
    menu_list buy_sell_menu[] = { { 15, 23, "구매하기" }, { 53, 23, "판매하기" } };

    int total_items = rarity_item_counts[current_rarity];
    int items_per_page = ITEMS_PER_PAGE;
    int total_pages = total_items / items_per_page;

    // 페이지 범위 보정
    if (page < 0) page = 0;
    if (page >= total_pages) page = total_pages - 1;

    int start = page * items_per_page;
    int end = start + items_per_page;
    if (end > total_items) end = total_items;

    for (int i = start; i < end; i++) {
        int local = i - start;  // 0 ~ items_per_page-1
        int x = (local < ITEMS_PER_ROW) ? 13 : 45;
        int y = 6 + (local % ITEMS_PER_ROW) * 4;

        // 설명 및 구매/판매 메뉴 출력
        if ((focus_level == FOCUS_LEVEL_ITEM_LIST || focus_level == FOCUS_LEVEL_ITEM_BUY_SELL) && i == selected_item_index) {
            UI_cleaner_inventory_item_description();
            utils_gotoxy(79, 6);
            utils_set_color(COLOR_DEFAULT_TEXT);
            printf("%s", current_equipment_list[current_rarity][i].description);
            if (current_rarity == RARITY_UNIQUE) {
                utils_gotoxy(79, 14);
                printf("%s 세트 효과", set_effects[selected_item_index].name);
                utils_gotoxy(79, 15);
                printf("%s", set_effects[selected_item_index].description);
            }
            s_print_stat_bonus(current_rarity, current_equipment_list, player, i, 7, type);

            for (int j = 0; j < 2; j++) {
                bool sel = (focus_level == FOCUS_LEVEL_ITEM_BUY_SELL &&
                    buy_sell_state == (j == 0 ? STORE_STATE_BUY : STORE_STATE_SELL));
                utils_set_color(sel ? COLOR_SELECT_MENU : COLOR_DEFAULT);
                utils_gotoxy(buy_sell_menu[j].x, buy_sell_menu[j].y);
                printf("%s", buy_sell_menu[j].text);

                char price_buf[32];
                int price = (j == 0)
                    ? current_equipment_list[current_rarity][i].buy_price
                    : current_equipment_list[current_rarity][i].sell_price;
                snprintf(price_buf, sizeof(price_buf), "%d C", price);

                int text_len = (int)strlen(buy_sell_menu[j].text);
                int price_len = (int)strlen(price_buf);
                int offset = (text_len - price_len) / 2;
                utils_gotoxy(buy_sell_menu[j].x + offset, buy_sell_menu[j].y + 2);
                printf("%s", price_buf);
            }
            utils_set_color(COLOR_SELECT_MENU);
        }
        else {
            utils_set_color(COLOR_DEFAULT);
        }

        // 아이템 리스트 출력
        utils_gotoxy(x, y);
        printf("* %s (%d)",
            current_equipment_list[current_rarity][i].name,
            inventory[current_rarity][i].count);
    }

    // 페이지 정보
    utils_set_color(COLOR_DEFAULT_TEXT);
    utils_gotoxy(35, 17);
    printf("%d / %d", page + 1, total_pages);

    // 플레이어 코인 중앙정렬
    int coin_start = 111;
    int coin_end = 151;
    char coin_buf[32];
    snprintf(coin_buf, sizeof(coin_buf), "%d C", player->coin);
    int coin_len = (int)strlen(coin_buf);
    int coin_offset = coin_start + ((coin_end - coin_start + 1) - coin_len) / 2;
    utils_gotoxy(coin_offset, 23);
    printf("%s", coin_buf);

    utils_set_color(COLOR_DEFAULT_TEXT);
}

static void s_print_store_heal_item_page(
    player_t* player, focus_level_t focus_level, int selected_item_index, store_state_t buy_sell_state
)
{
    menu_list buy_sell_menu[] = { { 15, 23, "구매하기" }, { 53, 23, "판매하기" } };

    for (int i = 0; i < HEAL_ITEM_COUNT; i++) {
        int local_index_on_page = i;
        int x = (local_index_on_page < ITEMS_PER_ROW) ? 13 : 45;
        int y = 6 + (local_index_on_page % ITEMS_PER_ROW) * 4;

        if ((focus_level == FOCUS_LEVEL_ITEM_LIST || focus_level == FOCUS_LEVEL_ITEM_BUY_SELL) && i == selected_item_index) {
            UI_cleaner_inventory_item_description();
            utils_gotoxy(79, 6);
            utils_set_color(COLOR_DEFAULT_TEXT);
            printf("%s", heal_items[i].description);

            for (int j = 0; j < 2; j++) {
                utils_gotoxy(buy_sell_menu[j].x, buy_sell_menu[j].y);
                if (focus_level == FOCUS_LEVEL_ITEM_BUY_SELL && buy_sell_state == (j == 0 ? STORE_STATE_BUY : STORE_STATE_SELL)) {
                    utils_set_color(COLOR_SELECT_MENU);
                }
                else {
                    utils_set_color(COLOR_DEFAULT);
                }
                printf("%s", buy_sell_menu[j].text);

                char price_buf[32];
                int price = (j == 0) ? heal_items[i].buy_price : heal_items[i].sell_price;
                snprintf(price_buf, sizeof(price_buf), "%d C", price);
                int offset = (int)(strlen(buy_sell_menu[j].text) - strlen(price_buf)) / 2;
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
        printf("* %s (%d개)", heal_items[i].name, heal_item_inventory[i]);
    }

    utils_set_color(COLOR_DEFAULT_TEXT);
    int coin_start = 111;
    int coin_end = 151;
    char coin_buf[32];
    snprintf(coin_buf, sizeof(coin_buf), "%d C", player->coin);
    int len = (int)strlen(coin_buf);
    int offset = coin_start + ((coin_end - coin_start + 1) - len) / 2;
    utils_gotoxy(offset, 23);
    printf("%s", coin_buf);
}

static void s_print_sub_menu_box(const menu_list menus[], focus_level_t focus_level, equipment_rarity_t rarity)
{
    utils_set_color(COLOR_DEFAULT_TEXT);
    for (int y = 5; y < 18; y++) {
        utils_gotoxy(9, y); putchar('|');
    }

    for (int i = 0; i < 4; i++) {
        if ((focus_level == FOCUS_LEVEL_ITEM_LIST || focus_level == FOCUS_LEVEL_ITEM_BUY_SELL) && rarity == i) {
            utils_set_color(COLOR_YELLOW);
        }
        else {
            utils_set_color(COLOR_DEFAULT);
        }
        utils_gotoxy(menus[i].x, menus[i].y);
        printf("%s", menus[i].text);
    }

    utils_set_color(COLOR_DEFAULT_TEXT);
}

// ==============================

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
        int key = utils_getch();

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
       "시작",
       "옵션",
       "종료"
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

void UI_dynamic_setting_menu(setting_state_t selected, int* global_volume)
{
    const char* options[SETTING_STATE_MAX] = {
       "사운드 켜기",
       "사운드 끄기",
       "사운드 조절",
       "뒤로가기"
    };

    for (int i = 0; i < SETTING_STATE_MAX; i++) {
        int option_len = (int)strlen(options[i]);
        int mx = (WIDTH - option_len - 2) / 2;
        utils_gotoxy(mx, 10 + i * 2);

        if (selected == i) {
            utils_set_color(COLOR_SELECT_MENU);
            printf("%s", options[i]);
        }
        else {
            utils_set_color(COLOR_DEFAULT);
            printf("%s", options[i]);
        }
        utils_set_color(COLOR_DEFAULT);
    }

    const int base_y = 12;
    int vol_y = base_y + SETTING_STATE_VOLUME;
    int vol_opt_len = (int)strlen(options[SETTING_STATE_VOLUME]);
    int mx_vol = (WIDTH - vol_opt_len - 2) / 2 + vol_opt_len + 4;
    utils_gotoxy(mx_vol, vol_y);

    if (selected == SETTING_STATE_VOLUME)
        utils_set_color(COLOR_SELECT_MENU);
    else
        utils_set_color(COLOR_DEFAULT);

    printf("%3d%%", *global_volume);

    utils_set_color(COLOR_DEFAULT_TEXT);
}

void UI_dynamin_esc_menu(esc_menu_state_t* selected, int gamemode)
{
    const char* options[4] = {
       "돌아가기",
       "저장하기",
       "옵션",
       "메인메뉴"
    };
    
    utils_set_color(COLOR_DEFAULT);
    for (int i = 0; i < 4; i++) {
        if (gamemode == MODE_STATE_NORMAL && i == 1) {
            continue;
        }
        int mx = (WIDTH - (int)strlen(options[i])) / 2;
        utils_gotoxy(mx, 15 + i * 2);

        if (*selected == i) {
            utils_set_color(COLOR_SELECT_MENU);
            printf("%s", options[i]);
            utils_set_color(COLOR_DEFAULT);
        }
        else
        {
            printf("%s", options[i]);
        }
    }

    utils_set_color(COLOR_DEFAULT_TEXT);
}

void UI_dynamic_save_load_menu(save_load_num_t* selected)
{
    int box_width = 38;
    int padding = (WIDTH - (box_width * 3)) / 4;

    save_slot_info_t slots[3];
    load_save_slot_info(slots);

    for (int i = 0; i < 3; i++) {
        int start_x = padding + (i * (box_width + padding));
        int content_x = start_x + 3;
        int content_y = 11;

        // 선택된 슬롯은 강조 색상
        if (i == *selected) {
            utils_set_color(COLOR_SELECT_MENU);
        }
        else {
            utils_set_color(COLOR_DEFAULT);
        }

        // 슬롯 제목
        char title[32];
        snprintf(title, sizeof(title), "저장 슬롯 %d", i + 1);
        utils_gotoxy(start_x + (box_width - (int)strlen(title)) / 2, content_y);
        printf("%s", title);

        // 저장이 안 되어 있으면 색은 회색, 있으면 기본 흰색
        utils_gotoxy(start_x + (box_width - (int)strlen(slots[i].timestamp)) / 2, content_y + 2);
        printf("%s", slots[i].timestamp);
    }
}

void UI_dynamic_select_game_mode(game_mode_state_t selected, bool is_infinite_unlocked)
{
    const char* modes[] = {
       "일반 모드",
       "무한 모드"
    };

    utils_set_color(COLOR_DEFAULT);
    for (int i = 0; i < 2; i++) {
        int mx = (WIDTH - (int)strlen(modes[i])) / 2;
        utils_gotoxy(mx, 13 + i * 2);

        if (i == MODE_STATE_INFINITY && !is_infinite_unlocked) {
            utils_set_color(COLOR_DARKGRAY); // 잠겼으면 회색으로
        }
        else if (selected == i) {
            utils_set_color(COLOR_SELECT_MENU);
        }
        else {
            utils_set_color(COLOR_DEFAULT);
        }
        printf("%s", modes[i]); // 텍스트 출력

        // 잠겼을 때 안내 문구 추가
        if (i == MODE_STATE_INFINITY && !is_infinite_unlocked) {
            utils_gotoxy(mx + (int)strlen(modes[i]) + 2, 15);
            printf("(일반 모드 클리어 시 해금)");
        }
    }

    utils_set_color(COLOR_DEFAULT_TEXT);
}

void UI_dynamic_select_new_or_load_game(new_or_load_game_t* selected)
{
    const char* modes[] = {
       "새 게임",
       "불러오기"
    };

    utils_set_color(COLOR_DEFAULT);
    for (int i = 0; i < 2; i++) {
        int mx = (WIDTH - (int)strlen(modes[i])) / 2;
        utils_gotoxy(mx, 15 + i * 2);

        if (*selected == i) {
            utils_set_color(COLOR_SELECT_MENU);
            printf("%s", modes[i]);
            utils_set_color(COLOR_DEFAULT);
        }
        else
        {
            printf("%s", modes[i]);
        }
    }

    utils_set_color(COLOR_DEFAULT_TEXT);
}

void UI_dynamic_hero_select(hero_t selected_hero)
{
    int box_width = 38;
    int padding = (WIDTH - (box_width * 3)) / 4;

    const char* hero_names[] = { "브레이커", "카운터", "버서커" };
    const char* hero_descs[] = { "강인도 파괴 특화", "안정적인 전투 운영", "체력을 대가로 한 공격" };

    for (int i = 0; i < 3; i++) {
        int start_x = padding + (i * (box_width + padding));
        int content_x = start_x + 3;
        int content_y = 11;

        // 선택된 영웅은 다른 색으로 표시
        if (i == selected_hero) {
            utils_set_color(COLOR_SELECT_MENU);
        }
        else {
            utils_set_color(COLOR_DEFAULT);
        }

        // 박스 안에 영웅 이름과 설명 그리기
        utils_gotoxy(start_x + (box_width - (int)strlen(hero_names[i])) / 2, content_y);
        printf("%s", hero_names[i]);

        utils_gotoxy(start_x + (box_width - (int)strlen(hero_descs[i])) / 2, content_y + 3);
        printf("%s", hero_descs[i]);
    }
    utils_set_color(COLOR_DEFAULT_TEXT);
}



void UI_dynamic_infinite_upgrade(player_t* player, const upgrade_type_t choices[], int selection)
{
    const char* titles[UPGRADE_MAX] = {
        "최대 체력", "공격력", "속도", "강인도 피해", "스턴 시간", "치명타 확률", "치명타 피해", "흡혈량"
    };
    char buffer[100];

    int box_width = 38;
    int box_height = 15;
    int start_y = 8;
    int padding = (WIDTH - (box_width * 3)) / 4;

    for (int i = 0; i < 3; i++) {
        int start_x = padding + (i * (box_width + padding));
        upgrade_type_t current_choice = choices[i];

        // [추가] 박스 내부를 먼저 깨끗하게 정리 (잔상 제거)
        for (int y_clean = start_y + 1; y_clean < start_y + box_height - 1; y_clean++) {
            utils_gotoxy(start_x + 1, y_clean);
            printf("%*s", box_width - 2, "");
        }

        if (current_choice == UPGRADE_NONE) continue;

        if (i == selection) {
            utils_set_color(COLOR_SELECT_MENU); // 선택된 항목은 흰색
        }
        else {
            utils_set_color(COLOR_DEFAULT);     // 선택되지 않은 항목은 회색
        }

        // 제목 출력
        utils_gotoxy(start_x + (box_width - (int)strlen(titles[current_choice])) / 2, start_y + 3);
        printf("%s", titles[current_choice]);

        // 내용 계산
        switch (current_choice) {
        case UPGRADE_HP: snprintf(buffer, sizeof(buffer), "%d -> %d", player->max_hp, (int)(player->max_hp * 1.05)); break;
        case UPGRADE_ATK: snprintf(buffer, sizeof(buffer), "%d -> %d", player->attack, (int)(player->attack * 1.05)); break;
        case UPGRADE_SPD: snprintf(buffer, sizeof(buffer), "%d -> %d", player->speed, (int)(player->speed * 1.05)); break;
        case UPGRADE_BREAK: snprintf(buffer, sizeof(buffer), "%d -> %d", player->break_damage, player->break_damage + 10); break;
        case UPGRADE_STUN: snprintf(buffer, sizeof(buffer), "%d턴 -> %d턴", player->stun_duration, player->stun_duration + 1); break;
        case UPGRADE_CRIT_CHANCE: snprintf(buffer, sizeof(buffer), "%.0f%% -> %.0f%%", player->crit_chance * 100, (player->crit_chance + 0.05) * 100); break;
        case UPGRADE_CRIT_DMG: snprintf(buffer, sizeof(buffer), "%.0f%% -> %.0f%%", player->crit_damage * 100, (player->crit_damage + 0.1) * 100); break;
        case UPGRADE_LIFESTEAL: snprintf(buffer, sizeof(buffer), "%.1f%% -> %.1f%%", player->life_steal * 100.0, player->life_steal * 1.05 * 100.0); break;
        }

        // 내용 출력
        utils_gotoxy(start_x + (box_width - (int)strlen(buffer)) / 2, start_y + 7);
        printf("%s", buffer);
    }
    utils_set_color(COLOR_DEFAULT_TEXT);
}

// 배틀 선택지 ui 그리는 함수
void UI_dynamic_player_action_selection(int player_action_state)
{
    const char* menu[] = {
       "Attack",
       "Skill",
       "Iventory / setting"
    };
    utils_set_color(COLOR_DEFAULT);

    int x = 3;
    int y = 22;
    for (int i = 0; i < 3; i++) {
        utils_gotoxy(x, y);

        if (player_action_state == i) {
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

static void s_UI_dynamic_monster_info_internal(monster_t* monster, int body_color)
{
    int bar_length = BAR_LENGTH;
    int start_point = 75 - (bar_length / 2);

    {
        float hp_ratio = (float)monster->current_hp / monster->max_hp;
		int current_hp_blocks = monster->current_hp / (monster->max_hp / bar_length);
        if (monster->current_hp > 0 && current_hp_blocks == 0) {
            current_hp_blocks = 1;
        }

        int hp_color;
        if (hp_ratio > 0.7f) hp_color = COLOR_GREEN;
        else if (hp_ratio > 0.3f) hp_color = COLOR_YELLOW;
        else if (hp_ratio > 0) hp_color = COLOR_LIGHTRED;
        else hp_color = COLOR_DARKGRAY;

        for (int i = 0; i < bar_length; ++i) {
            utils_gotoxy(start_point + i, 2);
            if (i < current_hp_blocks) {
                utils_set_color(hp_color);
                printf("▒");
            }
            else {
                utils_set_color(COLOR_DARKGRAY);
                printf("_");
            }
        }
    }
    {
        double toughness_ratio = (double)monster->current_toughness / monster->max_toughness;
        int current_toughness_blocks = (int)(bar_length * toughness_ratio);
        if (monster->current_toughness > 0 && current_toughness_blocks == 0) {
            current_toughness_blocks = 1;
        }

        int toughness_color = COLOR_WHITE;
        for (int i = 0; i < bar_length; ++i)
        {
            utils_gotoxy(start_point + i, 1);
            if (i < current_toughness_blocks) {
                utils_set_color(toughness_color);
                printf("▒");
            }
            else {
                utils_set_color(COLOR_DARKGRAY);
                printf("_");
            }
        }
    }

    int prev = _setmode(_fileno(stdout), _O_U16TEXT);
    if (prev == -1) { perror("_setmode 실패"); return; }

    utils_set_color(body_color);

    // 1) 줄 수 세기 (NULL 직전까지)
    int lines = 0;
    while (lines < MAX_IMAGE_LINES && monster->image[lines])
        lines++;

    // 2) 중앙 좌표
    const int center_x = 75;
    const int center_y = 10;

    // 3) 출력 시작 Y 계산 (세로 중앙 정렬)
    int start_y = center_y - (lines / 2);

    for (int i = 0; i < lines; i++) {
        // 4) 각 줄 길이 구해서 X 중앙 정렬
        size_t len = wcslen(monster->image[i]);
        int start_x = center_x - (int)len / 2;

        utils_gotoxy(start_x, start_y + i);
        wprintf(L"%ls", monster->image[i]);
    }

    fflush(stdout);

    // 모드 복원
    if (_setmode(_fileno(stdout), prev) == -1) {
        perror("모드 복원 실패");
        return;
    }
}

void UI_dynamic_monster_info(monster_t* monster, int current_stage)
{
    s_UI_dynamic_monster_info_internal(monster, COLOR_DEFAULT_TEXT);

    utils_gotoxy(50, 19);
    utils_set_color(COLOR_LIGHTGRAY);
    printf("스테이지 : ");
    printf("%d", current_stage + 1);
    utils_gotoxy(80, 19);
    printf("몬스터 : ");
    utils_set_color(COLOR_LIGHTRED);
    printf("%s", monster->name);
    utils_set_color(COLOR_DEFAULT_TEXT);
}

void UI_dynamic_monster_flash_effect(monster_t* monster)
{
    for (int r = 0; r < 2; ++r)
    {
        s_UI_dynamic_monster_info_internal(monster, COLOR_DARKGRAY);
        Sleep(50);

        s_UI_dynamic_monster_info_internal(monster, COLOR_DEFAULT_TEXT);
        Sleep(50);
    }
}

void UI_dynamic_player_info(player_t* player)
{
    utils_gotoxy(114, 21);  printf("Name : %s", player->name);
    utils_gotoxy(114, 22);  printf("HP   : %5d / %5d", player->current_hp, player->max_hp);
    utils_gotoxy(114, 23);  printf("ATK  : %d", player->attack);
    utils_gotoxy(114, 24);  printf("SPD  : %d", player->speed);
    utils_gotoxy(114, 25);  printf("EVA  : %.2f%%", player->evasion_rate * 100);
    utils_gotoxy(114, 26);  printf("DEF  : %.2f%%", player->defence_rate * 100);
}

void UI_dynamic_select_heal_or_store(heal_or_store_t* selected, player_t* player) {

    int box_width = 38;
    int padding = (WIDTH - (box_width * 3)) / 4;

    const char* menu[] = {
       "휴식",
       "상점",
       "후퇴"
	};

    const char* menu_desc[] = {
       "최대 체력의 30%을 회복합니다",
       "상점으로 이동합니다",
       "전장에서 후퇴합니다"
    };

    for (int i = 0; i < 3; i++) {
        int start_x = padding + (i * (box_width + padding));
        int content_x = start_x + 3;
        int content_y = 11;
        if (i == *selected) {
            utils_set_color(COLOR_SELECT_MENU);
        }
        else {
            utils_set_color(COLOR_DEFAULT);
        }

        utils_gotoxy(start_x + (box_width - (int)strlen(menu[i])) / 2, content_y);
        printf("%s", menu[i]);

        utils_gotoxy(start_x + (box_width - (int)strlen(menu_desc[i])) / 2, content_y + 3);
        printf("%s", menu_desc[i]);

        if (i == 1) {
            char coin_buf[32];
            snprintf(coin_buf, sizeof(coin_buf), "%d C", player->coin);
            int coin_len = (int)strlen(coin_buf);

            // 박스 내부 가로 중앙 계산
            int coin_x = start_x + (box_width - coin_len) / 2;
            // 설명 아래로 2칸 내려서 찍어볼게요 (content_y+5)
            int coin_y = content_y + 8;

            utils_gotoxy(coin_x, coin_y);
            printf("%s", coin_buf);
        }
    }

    utils_set_color(COLOR_DEFAULT_TEXT);
}
// =========================

void UI_dynamic_action_order(player_t* player, monster_t* monster)
{
    // 임시 변수에 현재 행동 가치를 복사
    double p_av = player->action_value;
    double m_av = monster->action_value;

    // 플레이어와 몬스터의 기본 행동 가치 (턴 행동 후 더해줄 값)
    double p_base_av = 10000.0 / player->speed;
    double m_base_av = 10000.0 / monster->speed;

    int monster_turn = monster->action_value / (10000.0 / monster->speed);

    int x = 2, y = 2;
    utils_gotoxy(x, y);
    printf("행동 서열 beta");
    y += 2;

    // 앞으로 5턴 정도의 순서를 예측해서 출력
    for (int i = 0; i < 5; i++) {
        utils_gotoxy(x, y);
        if (p_av <= m_av) {
            printf("[플레이어]");
            p_av += p_base_av;
        }
        else {
            if (monster_turn % 3 == 0) utils_set_color(COLOR_RED);
            else utils_set_color(COLOR_DEFAULT_TEXT);
            monster_turn++;
            printf("[ 몬스터 ]");
            m_av += m_base_av;
            utils_set_color(COLOR_DEFAULT_TEXT);
        }
        y++;
    }
    utils_gotoxy(x, y);
    printf("...");
}

// [수정] 
void UI_dynamic_inventory_info(player_t* player)
{

    equipment_rarity_t current_rarity = get_inventory_rarity_type();
    inventory_state_t current_inventory_state = get_inventory_state();
    focus_level_t focus_level = get_inventory_focus_level();
    int selected_item_index = get_inventory_selected_index();
    int weapon_page = get_inventory_weapon_page();
    int armor_page = get_inventory_armor_page();

    const menu_list top_items[] = {
       {3, 2, "◁---"}, {28, 2, "무기"}, {72, 2, "방어구"}, {115, 2, "소비 아이템"}, { 144, 2, "옵션"}
    };
    const menu_list sub_menu[] = {
       {5, 8, "N"}, {5, 10, "R"}, {5, 12, "E"}, {5, 14, "U"}
    };

    for (int i = 0; i < 5; i++) {
        if (focus_level == FOCUS_LEVEL_ITEM_LIST && i == current_inventory_state) {
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
        s_print_sub_menu_box(sub_menu, focus_level, current_rarity);
        s_print_inventory_item_page(weapons, weapon_inventory, current_rarity, player, focus_level, selected_item_index, weapon_page, 0);
    }
    else if (current_inventory_state == INVENTORY_STATE_ARMOR) {
        s_print_sub_menu_box(sub_menu, focus_level, current_rarity);
        s_print_inventory_item_page(armors, armor_inventory, current_rarity, player, focus_level, selected_item_index, armor_page, 1);
    }
    else if (current_inventory_state == INVENTORY_STATE_HEAL_ITEM) {
        UI_cleaner_sub_menu();
        s_print_heal_item_list(player, focus_level, selected_item_index);
    }
    else {
        UI_cleaner_sub_menu();
    }

    utils_set_color(COLOR_DEFAULT_TEXT);
    UI_dynamic_player_info(player);
}

// [수정] 
void UI_dynamic_store_info(player_t* player)
{
    store_state_t current_store_state = get_store_state();
    equipment_rarity_t current_rarity = get_store_rarity_type();
    focus_level_t focus_level = get_store_focus_level();
    store_buy_sell_state_t buy_sell_successful = get_store_buy_sell_successful_state();
    store_state_t buy_sell_state = get_store_buy_sell_state();
    int selected_item_index = get_store_selected_index();
    int weapon_page = get_store_weapon_page();
    int armor_page = get_store_armor_page();


    menu_list store_menu[] = {
       {3, 2, "◁---"}, { 30, 2, "무기" }, {77, 2, "방어구"}, {121, 2, "소비 아이템"}
    };
    const menu_list sub_menu[] = {
       {5, 8, "N"}, {5, 10, "R"}, {5, 12, "E"}, {5, 14, "U"}
    };

    for (int i = 0; i < 4; i++) {
        if ((focus_level == FOCUS_LEVEL_ITEM_LIST || focus_level == FOCUS_LEVEL_ITEM_BUY_SELL) && i == current_store_state) {
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

    utils_set_color(COLOR_SELECT_MENU);
    if (buy_sell_successful == STORE_BUY_SUCCESS) {
        UI_cleaner_current_weapon_box();
        utils_gotoxy(10, 23);
        printf("아이템 구매 성공!");
    }
    else if (buy_sell_successful == STORE_SELL_SUCCESS) {
        UI_cleaner_current_armor_box();
        utils_gotoxy(48, 23);
        printf("아이템 판매 성공!");
    }
    else if (buy_sell_successful == STORE_BUY_FAIL) {
        UI_cleaner_current_weapon_box();
        utils_gotoxy(10, 23);
        printf("아이템 구매 실패!");
    }
    else if (buy_sell_successful == STORE_SELL_FAIL) {
        UI_cleaner_current_armor_box();
        utils_gotoxy(48, 23);
        printf("아이템 판매 실패!");
    }

    if (buy_sell_successful != STORE_BUY_SELL_NONE) {
        Sleep(1000);
        UI_cleaner_current_weapon_box();
        UI_cleaner_current_armor_box();
    }

    UI_cleaner_inventory_item_list();
    UI_cleaner_inventory_item_description();
    UI_cleaner_buy_sell_box();

    if (current_store_state == STORE_STATE_WEAPON) {
        s_print_sub_menu_box(sub_menu, focus_level, current_rarity);
        s_print_store_item_page(weapons, weapon_inventory, current_rarity, player, focus_level, selected_item_index, buy_sell_state, weapon_page, 0);
    }
    else if (current_store_state == STORE_STATE_ARMOR) {
        s_print_sub_menu_box(sub_menu, focus_level, current_rarity);
        s_print_store_item_page(armors, armor_inventory, current_rarity, player, focus_level, selected_item_index, buy_sell_state, armor_page, 1);
    }
    else if (current_store_state == STORE_STATE_HEAL_ITEM) {
        UI_cleaner_sub_menu();
        s_print_store_heal_item_page(player, focus_level, selected_item_index, buy_sell_state);
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

    equipment_t* weapon = &weapons[player->weapon_rarity][player->weapon_index];
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

    equipment_t* armor = &armors[player->armor_rarity][player->armor_index];
    int len = (int)strlen(armor->name);
    int padding = start_x + (end_x - start_x - len) / 2;
    utils_gotoxy(padding, 23);
    printf("%s", armor->name);
}