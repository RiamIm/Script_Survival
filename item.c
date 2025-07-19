// item.c
#define _CRT_SECURE_NO_WARNINGS
#include "item.h"

equipment_t temp_weapons[WEAPON_COUNT];
equipment_t temp_armors[ARMOR_COUNT];
equipment_t weapons[WEAPON_COUNT];
equipment_t armors[ARMOR_COUNT];
heal_item_t heal_items[HEAL_ITEM_COUNT];

heal_item_t heal_items[] = {
    { "사과", "체력을 50 회복합니다.", 50 },
    { "치유 물약", "체력을 100 회복합니다.", 100 },
    { "강화 물약", "체력을 200 회복합니다.", 200 },
    { "마법의 물약", "체력을 300 회복합니다.", 300 },
    { "황금 사과", "최대 체력까지 모두 회복합니다.",  99999 }, 
    { "신비한 돌", "랜덤으로 능력치가 소폭 증가합니다.", -1 } 
};

static equipment_rarity_t s_get_rarity_from_string(const char* str)
{
    if (strcmp(str, "ITEM_NORMAL") == 0) return ITEM_NORMAL;
	if (strcmp(str, "ITEM_RARE") == 0) return ITEM_RARE;
	if (strcmp(str, "ITEM_EPIC") == 0) return ITEM_EPIC;
    if (strcmp(str, "ITEM_UNIQUE") == 0) return ITEM_UNIQUE;

	return ITEM_NORMAL; // 기본값
}

static equipment_region_t s_get_region_from_string(const char* str)
{
    if (strcmp(str, "ITEM_FOREST") == 0) return ITEM_FOREST;
    if (strcmp(str, "ITEM_DESERT") == 0) return ITEM_DESERT;
    if (strcmp(str, "ITEM_SNOWFIELD") == 0) return ITEM_SNOW;
    return ITEM_FOREST; // 기본값
}

static void s_read_equipment_csv(const char* filename, equipment_t items[], int max_items) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("파일 열기 실패");
        return;
    }

    char buffer[BUFFER_SIZE];
    int count = 0;

    // 1. 헤더(첫 번째 줄) 건너뛰기
    if (fgets(buffer, BUFFER_SIZE, fp) == NULL) {
        printf("파일이 비어있거나 헤더를 읽을 수 없습니다.\n");
        fclose(fp);
        return;
    }

    // 2. 데이터 라인 읽기
    while (fgets(buffer, BUFFER_SIZE, fp) != NULL && count < max_items) {
        // strtok()은 원본을 수정하므로, 복사본을 만들어 사용하는 것이 더 안전할 수 있습니다.
        // 이 예제에서는 간단히 buffer를 직접 사용합니다.

        char* token;

        token = strtok(buffer, ",");
        if (token) strcpy(items[count].name, token);

        token = strtok(NULL, ",");
        if (token) strcpy(items[count].description, token);

        token = strtok(NULL, ",");
		if (token) items[count].rarity = s_get_rarity_from_string(token); // 문자열을 enum으로 변환

        token = strtok(NULL, ",");
		if (token) items[count].region = s_get_region_from_string(token); // 문자열을 enum으로 변환

        token = strtok(NULL, ",");
        if (token) items[count].attack_bonus = atoi(token); // 문자열을 정수로

        token = strtok(NULL, ",");
        if (token) items[count].max_hp_bonus = atoi(token);

        token = strtok(NULL, ",");
        if (token) items[count].speed_bonus = atoi(token);

        token = strtok(NULL, ",");
        if (token) items[count].evasion_bonus = atof(token); // 문자열을 실수로

        token = strtok(NULL, ",");
        if (token) items[count].defence_bonus = atof(token);

		token = strtok(NULL, ",");
		if (token) items[count].buy_price = atoi(token); // 문자열을 정수로

		token = strtok(NULL, ",\n");
		if (token) items[count].sell_price = atoi(token); // 문자열을 정수로

        count++;
    }

    fclose(fp);
}

// scv 파일을 불러와서 equipment_t 구조체 배열에 저장
void item_init(void)
{
	s_read_equipment_csv("data/weapons.csv", temp_weapons, WEAPON_COUNT);
	s_read_equipment_csv("data/armors.csv", temp_armors, ARMOR_COUNT);

	int weapon_index = 0;
	int armor_index = 0;

    for (int region = ITEM_FOREST; region <= ITEM_SNOW; region++) {
        for (int rarity = ITEM_NORMAL; rarity <= ITEM_UNIQUE; rarity++) {
            for (int i = region * 24; i < region * 24 + 24; i++) {
                if (temp_weapons[i].region == region && temp_weapons[i].rarity == rarity) {
                    if (weapon_index < WEAPON_COUNT) {
                        weapons[weapon_index] = temp_weapons[i];
                        weapon_index++;
                    }
                }

                if (temp_armors[i].region == region && temp_armors[i].rarity == rarity) {
                    if (armor_index < ARMOR_COUNT) {
                        armors[armor_index] = temp_armors[i];
                        armor_index++;
                    }
                }
			}
		}
	}
}

void use_weapon(int next_index, player_t* player)
{
    if (weapon_inventory[next_index].count == 0) return;

    int current_index = player->weapon_index;

    if (current_index != -1) {
        player->attack -= weapons[current_index].attack_bonus;
        player->speed -= weapons[current_index].speed_bonus;
        player->evasion_rate -= weapons[current_index].evasion_bonus;
        player->defence_rate -= weapons[current_index].defence_bonus;
    }

    player->weapon_index = next_index;

    if (next_index != -1) {
        player->attack += weapons[next_index].attack_bonus;
        player->speed += weapons[next_index].speed_bonus;
        player->evasion_rate += weapons[next_index].evasion_bonus;
        player->defence_rate += weapons[next_index].defence_bonus;
    }
}

void use_armor(int next_index, player_t* player)
{
    if (armor_inventory[next_index].count == 0) return;

    int current_index = player->armor_index;

    if (current_index != -1) {
        player->max_hp -= armors[current_index].max_hp_bonus;
        player->current_hp -= armors[current_index].max_hp_bonus;
        player->speed -= armors[current_index].speed_bonus;
        player->evasion_rate -= armors[current_index].evasion_bonus;
        player->defence_rate -= armors[current_index].defence_bonus;
    }

    player->armor_index = next_index;

    if (next_index != -1) {
        player->max_hp += armors[next_index].max_hp_bonus;
        player->current_hp += armors[next_index].max_hp_bonus;
        player->speed += armors[next_index].speed_bonus;
        player->evasion_rate += armors[next_index].evasion_bonus;
        player->defence_rate += armors[next_index].defence_bonus;
    }

    if (player->current_hp > player->max_hp) {
        player->current_hp = player->max_hp;
    }
    if (player->current_hp <= 0) {
        player->current_hp = 1;
    }
}

bool use_heal_item(int item_index, player_t* player)
{
    if (item_index < 0 || item_index >= HEAL_ITEM_COUNT || heal_item_inventory[item_index] <= 0) {
        return false; // 아이템이 없으면 실패
    }

    heal_item_t* item = &heal_items[item_index];
    heal_item_inventory[item_index]--; // 아이템 개수 감소

    // 스탯 랜덤 증가 아이템
    if (item->hp_bonus == -1) {
        int stat_choice = rand() % 3; // 0: 공격력, 1: 최대체력, 2: 속도 (beta)
        switch (stat_choice) {
        case 0:
            player->attack += 5;
            break;
        case 1:
            player->max_hp += 10;
            player->current_hp += 10;
            break;
        case 2:
            player->speed += 3;
            break;
        }
    }
    // 체력 회복 아이템 (99999 포함)
    else if (item->hp_bonus > 0) {
        player->current_hp += item->hp_bonus;
        if (player->current_hp > player->max_hp) {
            player->current_hp = player->max_hp;
        }
    }

    return true;
}