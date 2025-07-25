// UI_control.c
#define _CRT_SECURE_NO_WARNINGS
#include "UI_control.h"
#include "UI_dynamic.h"
#include "player.h"

void UI_control_init(UI_state_t* ui_main_state, title_state_t* ui_title_state, player_action_t* player_action_state)
{
    *ui_main_state = UI_STATE_TITLE;
    *ui_title_state = TITLE_STATE_START;
    *player_action_state = PLAYER_ACTION_ATTACK;
}

void UI_control_title(UI_state_t* ui_main_state, title_state_t* ui_title_state, int menu_key)
{
    if (menu_key == ENTER) {
        if (*ui_title_state == TITLE_STATE_START) *ui_main_state = UI_STATE_SELECT_GAME_MODE;
        else if (*ui_title_state == TITLE_STATE_OPTIONS) *ui_main_state = UI_STATE_SETTING;
        else if (*ui_title_state == TITLE_STATE_EXIT) exit(0);
    }
    else if (menu_key == UP) {
        *ui_title_state = (*ui_title_state - 1 + 3) % 3;
    }
    else if (menu_key == DOWN) {
        *ui_title_state = (*ui_title_state + 1) % 3;
    }
}

void UI_control_setting(UI_state_t* ui_main_state, setting_state_t* ui_setting_state, int* global_volume, int key)
{
    switch (key) {
    case UP:
        *ui_setting_state = (*ui_setting_state - 1 + SETTING_STATE_MAX) % SETTING_STATE_MAX;
        break;

    case DOWN:
        *ui_setting_state = (*ui_setting_state + 1) % SETTING_STATE_MAX;
        break;

    case LEFT:
        if (*ui_setting_state == SETTING_STATE_VOLUME && *global_volume > 0)
            *global_volume -= 5;
        break;

    case RIGHT:
        if (*ui_setting_state == SETTING_STATE_VOLUME && *global_volume < 100)
            *global_volume += 5;
        break;

    case ENTER:
        if (*ui_setting_state == SETTING_STATE_SOUND_ON) {
            // 예: 사운드 엔진에 알림
            // sound_set_enabled(true);
        }
        else if (*ui_setting_state == SETTING_STATE_SOUND_OFF) {
            // sound_set_enabled(false);
        }
        else if (*ui_setting_state == SETTING_STATE_BACK) {
            *ui_main_state = UI_STATE_TITLE;
        }
        break;
    }
}

// 모드 선택 화면 ↑↓ + 엔터 처리
void UI_control_game_mode(UI_state_t* ui_main_state, game_mode_state_t* ui_mode_state,
    int* game_mode, int key, bool is_infinite_unlocked
)
{
    if (key == ENTER) {
        if (*ui_mode_state == MODE_STATE_INFINITY) {
            if (is_infinite_unlocked) *game_mode = GAME_MODE_INFINITY;
            else return; // 아무것도 하지 않음
        }
        if (*ui_mode_state == MODE_STATE_NORMAL)
            *game_mode = GAME_MODE_NORMAL;

        *ui_main_state = UI_STATE_SELECT_HERO;
    }
    else if (key == UP && is_infinite_unlocked) {
        *ui_mode_state = (*ui_mode_state - 1 + MODE_STATE_MAX) % MODE_STATE_MAX;
    }
    else if (key == DOWN && is_infinite_unlocked) {
        *ui_mode_state = (*ui_mode_state + 1) % MODE_STATE_MAX;
    }
}

void UI_control_hero_select(UI_state_t* ui_main_state, hero_t* choice_hero, int key)
{
    int current_hero = (int)*choice_hero;

    if (key == LEFT) {
        current_hero = (current_hero - 1 + 3) % 3; // 3은 영웅 수
    }
    else if (key == RIGHT) {
        current_hero = (current_hero + 1) % 3;
    }
    else if (key == ENTER) {
        *ui_main_state = UI_STATE_CREATE_PLAYER_NAME; // 이름 생성 단계로 전환
    }

    *choice_hero = (hero_t)current_hero;
}

void UI_control_generate_upgrade_choices(player_t* player, upgrade_type_t out_choices[])
{
    upgrade_type_t possible[UPGRADE_MAX];
    int count = 0;

    for (int i = 0; i < UPGRADE_MAX; i++) {
        upgrade_type_t type = (upgrade_type_t)i;
        if (type == UPGRADE_LIFESTEAL && player->choice_hero != HERO_BERSERKER) continue;
        if (type == UPGRADE_CRIT_CHANCE && player->crit_chance >= 1.0) continue;
        possible[count++] = type;
    }

    // Fisher-Yates shuffle
    for (int i = count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        upgrade_type_t temp = possible[i];
        possible[i] = possible[j];
        possible[j] = temp;
    }

    for (int i = 0; i < 3; i++) {
        out_choices[i] = (i < count) ? possible[i] : UPGRADE_NONE;
    }
}

// 선택한 업그레이드를 플레이어에게 적용하는 함수
void UI_control_handle_upgrade_selection(UI_state_t* ui_main_state, player_t* player,
    const upgrade_type_t choices[], int* selection, int key)
{
    if (key == LEFT) {
        *selection = (*selection - 1 + 3) % 3;
    }
    else if (key == RIGHT) {
        *selection = (*selection + 1) % 3;
    }
    else if (key == ENTER) {
        upgrade_type_t choice = choices[*selection];
        if (choice == UPGRADE_NONE) return;

        switch (choice) {
        case UPGRADE_HP: player->max_hp = (int)(player->max_hp * 1.05); break;
        case UPGRADE_ATK: player->attack = (int)(player->attack * 1.05); break;
        case UPGRADE_SPD: player->speed = (int)(player->speed * 1.05); break;
        case UPGRADE_BREAK: player->break_damage += 10; break;
        case UPGRADE_STUN: player->stun_duration += 1; break;
        case UPGRADE_CRIT_CHANCE: player->crit_chance += 0.05; break;
        case UPGRADE_CRIT_DMG: player->crit_damage_modifier += 0.1; break;
        case UPGRADE_LIFESTEAL: player->life_steal = player->life_steal * 1.05; break;
        }

        if (choice == UPGRADE_HP) player->current_hp = player->max_hp;
        *ui_main_state = UI_STATE_BATTLE; // 전투 상태로 복귀
    }
}

player_action_t UI_control_player_action(player_action_t* player_action_state, int menu_key)
{
    if (menu_key == ENTER) {
        return *player_action_state; // return player_action_t
    }
    else if (menu_key == UP) {
        *player_action_state = (*player_action_state - 1 + 3) % 3;
    }
    else if (menu_key == DOWN) {
        *player_action_state = (*player_action_state + 1) % 3;
    }
    return PLAYER_ACTION_NONE;
}

// 무기 장착 여부 반환 (0 변경 없음, 1 무기 변경, 2 방어구 변경)
int UI_control_inventory(UI_state_t* ui_main_state, player_t* player, int menu_key)
{
    inventory_state_t current_state = get_inventory_state();
    focus_level_t focus_level = get_inventory_focus_level();
    equipment_rarity_t current_rarity = get_inventory_rarity_type();
    int selected_index = get_inventory_selected_index();
    int weapon_page = get_inventory_weapon_page();
    int armor_page = get_inventory_armor_page();
    int* page = (current_state == INVENTORY_STATE_WEAPON) ? &weapon_page : &armor_page;

    if (focus_level == FOCUS_LEVEL_TOP)
    {
        if (menu_key == ENTER) {
            if (current_state == INVENTORY_STATE_BACK) {
                *ui_main_state = UI_STATE_BATTLE;
            }
            else if (current_state == INVENTORY_STATE_WEAPON || current_state == INVENTORY_STATE_ARMOR) {
                focus_level = FOCUS_LEVEL_ITEM_LIST;
                selected_index = 0;
            }
            else if (current_state == INVENTORY_STATE_HEAL_ITEM) {
                focus_level = FOCUS_LEVEL_ITEM_LIST;
                selected_index = 0;
            }
        }
        else if (menu_key == LEFT) {
            current_state = (current_state - 1 + 5) % 5;
        }
        else if (menu_key == RIGHT) {
            current_state = (current_state + 1) % 5;
        }
        else if (menu_key == ESC) {
            *ui_main_state = UI_STATE_BATTLE;
        }
    }
    else if (focus_level == FOCUS_LEVEL_ITEM_LIST)
    {
        if (menu_key == ENTER) {
            if (current_state == INVENTORY_STATE_WEAPON) {
                bool is_same_item = (current_rarity == player->weapon_rarity && selected_index == player->weapon_index);
                if (!is_same_item) {
                    use_weapon(current_rarity, selected_index, player);
                    return 1; // 무기 변경됨
                }
            }
            else if (current_state == INVENTORY_STATE_ARMOR) {
                bool is_same_item = (current_rarity == player->armor_rarity && selected_index == player->armor_index);
                if (!is_same_item) {
                    use_armor(current_rarity, selected_index, player);
                    return 2; // 방어구 변경됨
                }
            }
            else if (current_state == INVENTORY_STATE_HEAL_ITEM) {
                if (use_heal_item(selected_index, player)) {
                    return 3;
                }
                // 아이템이 없어서 사용에 실패하면 아무것도 하지 않음
            }
            return 0;
        }

        if ((current_state == INVENTORY_STATE_WEAPON || current_state == INVENTORY_STATE_ARMOR) && page != NULL) {

            if (menu_key == 'N' || menu_key == 'n') {
                current_rarity = RARITY_NORMAL;
                selected_index = 0;   // 새 등급이면 첫 아이템으로
                *page = 0;            // 첫 페이지로
            }
            else if (menu_key == 'R' || menu_key == 'r') {
                current_rarity = RARITY_RARE;
                selected_index = 0;
                *page = 0;
            }
            else if (menu_key == 'E' || menu_key == 'e') {
                current_rarity = RARITY_EPIC;
                selected_index = 0;
                *page = 0;
            }
            else if (menu_key == 'U' || menu_key == 'u') {
                current_rarity = RARITY_UNIQUE;
                selected_index = 0;
                *page = 0;
            }
 
            int total_items = rarity_item_counts[current_rarity];
            int max_index = total_items - 1;
            int items_per_page = ITEMS_PER_PAGE;
            int max_page = total_items / items_per_page;

            if (menu_key == ESC) {
                focus_level = FOCUS_LEVEL_TOP;
            }
            else if (menu_key == UP) {
                if (selected_index > 0) selected_index--;
            }
            else if (menu_key == DOWN) {
                if (selected_index < max_index) selected_index++;
            }
            else if (menu_key == LEFT) {
                if (selected_index - ITEMS_PER_ROW >= 0) selected_index -= ITEMS_PER_ROW;
            }
            else if (menu_key == RIGHT) {
                if (selected_index + ITEMS_PER_ROW <= max_index) selected_index += ITEMS_PER_ROW;
            }

            // 경계 검사 및 페이지 재계산
            if (selected_index < 0)          selected_index = 0;
            if (selected_index > max_index)  selected_index = max_index;
            if (*page < 0)                   *page = 0;
            if (*page > max_page)            *page = max_page;
            *page = selected_index / items_per_page;
        }
        else if (current_state == INVENTORY_STATE_HEAL_ITEM) {
            if (menu_key == ESC) {
                focus_level = FOCUS_LEVEL_TOP;
            }
            else if (menu_key == UP) {
                if (selected_index > 0) {
                    selected_index--;
                }
            }
            else if (menu_key == DOWN) {
                if (selected_index < HEAL_ITEM_COUNT - 1) {
                    selected_index++;
                }
            }
            else if (menu_key == LEFT) {
                if (selected_index - ITEMS_PER_ROW >= 0) {
                    selected_index -= ITEMS_PER_ROW;
                }
            }
            else if (menu_key == RIGHT) {
                if (selected_index + ITEMS_PER_ROW < HEAL_ITEM_COUNT) {
                    selected_index += ITEMS_PER_ROW;
                }
            }
        }
    }

    set_inventory_state(current_state);
    set_inventory_focus_level(focus_level);
    set_inventory_rarity_type(current_rarity);
    set_inventory_selected_index(selected_index);
    set_inventory_weapon_page(weapon_page);
    set_inventory_armor_page(armor_page);

    return 0;
}

void UI_control_store(UI_state_t* ui_main_state, player_t* player, int menu_key)
{
    store_state_t current_state = get_store_state();
    focus_level_t focus_level = get_store_focus_level();
    equipment_rarity_t current_rarity = get_store_rarity_type();
    store_state_t buy_sell_state = get_store_buy_sell_state();
    int selected_index = get_store_selected_index();
    int weapon_page = get_store_weapon_page();
    int armor_page = get_store_armor_page();
    int* page = (current_state == STORE_STATE_WEAPON) ? &weapon_page : &armor_page;

    if (focus_level == FOCUS_LEVEL_TOP)
    {
        if (menu_key == ENTER) {
            if (current_state == STORE_STATE_BACK) *ui_main_state = UI_STATE_BATTLE;
            else if (current_state == STORE_STATE_WEAPON || current_state == STORE_STATE_ARMOR) focus_level = FOCUS_LEVEL_ITEM_LIST;
            else if (current_state == STORE_STATE_HEAL_ITEM) {
                focus_level = FOCUS_LEVEL_ITEM_LIST;
                selected_index = 0;
            }
        }
        else if (menu_key == LEFT) {
            current_state = (current_state - 1 + 4) % 4;
        }
        else if (menu_key == RIGHT) {
            current_state = (current_state + 1) % 4;
        }
        else if (menu_key == ESC) {
            *ui_main_state = UI_STATE_BATTLE;
        }
    }
    else if (focus_level == FOCUS_LEVEL_ITEM_LIST)
    {
        if ((current_state == STORE_STATE_WEAPON || current_state == STORE_STATE_ARMOR) && page != NULL) {

            if (menu_key == 'N' || menu_key == 'n') {
                current_rarity = RARITY_NORMAL;
                selected_index = 0;
                *page = 0;
            }
            else if (menu_key == 'R' || menu_key == 'r') {
                current_rarity = RARITY_RARE;
                selected_index = 0;
                *page = 0;
            }
            else if (menu_key == 'E' || menu_key == 'e') {
                current_rarity = RARITY_EPIC;
                selected_index = 0;
                *page = 0;
            }
            else if (menu_key == 'U' || menu_key == 'u') {
                current_rarity = RARITY_UNIQUE;
                selected_index = 0;
                *page = 0;
            }

            int total_items = rarity_item_counts[current_rarity];
            int max_index = total_items - 1;
            int items_per_page = ITEMS_PER_PAGE;
            int max_page = total_items / items_per_page;

            // 항목 선택
            if (menu_key == ENTER) {
                focus_level = FOCUS_LEVEL_ITEM_BUY_SELL;
            }
            else if (menu_key == ESC) {
                focus_level = FOCUS_LEVEL_TOP;
            }
            else if (menu_key == UP) {
                if (selected_index > 0) {
                    selected_index--;
                }
            }
            else if (menu_key == DOWN) {
                if (selected_index < max_index) {
                    selected_index++;
                }
            }
            else if (menu_key == LEFT) {
                if (selected_index - ITEMS_PER_ROW >= 0) {
                    selected_index -= ITEMS_PER_ROW;
                }
            }
            else if (menu_key == RIGHT) {
                if (selected_index + ITEMS_PER_ROW <= max_index) {
                    selected_index += ITEMS_PER_ROW;
                }
            }

            // 페이지 보정
            if (selected_index < 0)        selected_index = 0;
            if (selected_index > max_index) selected_index = max_index;
            if (*page < 0)                 *page = 0;
            if (*page > max_page)          *page = max_page;

            // page 계산 (선택 인덱스 기준)
            *page = selected_index / items_per_page;
        }
        else if (current_state == STORE_STATE_HEAL_ITEM) {
            if (menu_key == ENTER) {
                focus_level = FOCUS_LEVEL_ITEM_BUY_SELL;
            }
            else if (menu_key == ESC) {
                focus_level = FOCUS_LEVEL_TOP;
            }
            else if (menu_key == UP) { if (selected_index > 0) selected_index--; }
            else if (menu_key == DOWN) { if (selected_index < HEAL_ITEM_COUNT - 1) selected_index++; }
            else if (menu_key == LEFT) { if (selected_index - ITEMS_PER_ROW >= 0) selected_index -= ITEMS_PER_ROW; }
            else if (menu_key == RIGHT) { if (selected_index + ITEMS_PER_ROW < HEAL_ITEM_COUNT) selected_index += ITEMS_PER_ROW; }
        }
    }
    else if (focus_level == FOCUS_LEVEL_ITEM_BUY_SELL)
    {
        if (menu_key == ESC) {
            focus_level = FOCUS_LEVEL_ITEM_LIST;
        }
        else if (menu_key == LEFT || menu_key == RIGHT) {
            buy_sell_state = (buy_sell_state == STORE_STATE_BUY) ? STORE_STATE_SELL : STORE_STATE_BUY;
        }
        else if (menu_key == ENTER) {
            if (buy_sell_state == STORE_STATE_BUY) {
                if (current_state == STORE_STATE_WEAPON) {
                    if (player->coin >= weapons[current_rarity][selected_index].buy_price) {
                        player->coin -= weapons[current_rarity][selected_index].buy_price;
                        get_item(current_rarity, selected_index, ITEM_TYPE_WEAPON);
                        set_store_buy_sell_successful_state(STORE_BUY_SUCCESS);
                    }
                    else {
                        set_store_buy_sell_successful_state(STORE_BUY_FAIL);
                    }
                }
                else if (current_state == STORE_STATE_ARMOR) {
                    if (player->coin >= armors[current_rarity][selected_index].buy_price) {
                        player->coin -= armors[current_rarity][selected_index].buy_price;
                        get_item(current_rarity, selected_index, ITEM_TYPE_ARMOR);
                        set_store_buy_sell_successful_state(STORE_BUY_SUCCESS);
                    }
                    else {
                        set_store_buy_sell_successful_state(STORE_BUY_FAIL);
                    }
                }
                else if (current_state == STORE_STATE_HEAL_ITEM) {
                    if (player->coin >= heal_items[selected_index].buy_price) {
                        player->coin -= heal_items[selected_index].buy_price;
                        get_item(current_rarity, selected_index, ITEM_TYPE_HEAL_ITEM);
                        set_store_buy_sell_successful_state(STORE_BUY_SUCCESS);
                    }
                    else {
                        set_store_buy_sell_successful_state(STORE_BUY_FAIL);
                    }
                }
            }
            else { // STORE_STATE_SELL
                if (current_state == STORE_STATE_WEAPON) {
                    if (weapon_inventory[current_rarity][selected_index].count > 0) {
                        player->coin += weapons[current_rarity][selected_index].sell_price;
                        sell_item(current_rarity, selected_index, ITEM_TYPE_WEAPON);
                        set_store_buy_sell_successful_state(STORE_SELL_SUCCESS);
                    }
                    else {
                        set_store_buy_sell_successful_state(STORE_SELL_FAIL);
                    }
                }
                else if (current_state == STORE_STATE_ARMOR) {
                    if (armor_inventory[current_rarity][selected_index].count > 0) {
                        player->coin += armors[current_rarity][selected_index].sell_price;
                        sell_item(current_rarity, selected_index, ITEM_TYPE_ARMOR);
                        set_store_buy_sell_successful_state(STORE_SELL_SUCCESS);
                    }
                    else {
                        set_store_buy_sell_successful_state(STORE_SELL_FAIL);
                    }
                }
                else if (current_state == STORE_STATE_HEAL_ITEM) {
                    if (heal_item_inventory[selected_index] > 0) {
                        player->coin += heal_items[selected_index].sell_price;
                        sell_item(current_rarity, selected_index, ITEM_TYPE_HEAL_ITEM);
                        set_store_buy_sell_successful_state(STORE_SELL_SUCCESS);
                    }
                    else {
                        set_store_buy_sell_successful_state(STORE_SELL_FAIL);
                    }
                }
            }
            focus_level = FOCUS_LEVEL_ITEM_LIST;
        }
    }

    set_store_state(current_state);
    set_store_focus_level(focus_level);
    set_store_rarity_type(current_rarity);
    set_store_buy_sell_state(buy_sell_state);
    set_store_selected_index(selected_index);
    set_store_weapon_page(weapon_page);
    set_store_armor_page(armor_page);
}