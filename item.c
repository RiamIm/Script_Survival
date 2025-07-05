#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "player.h"
#include "item.h"

#define MAX_STAGE 12

static const char* s_forest_weapon_names[] = { "나무 검", "그림자 검", "숲의 도끼", "초록 궁", "오크 창",
"청록 단검", "이끼 스태프", "덩굴 채찍", "별빛 활", "나뭇잎 부메랑"
};

static const char* s_snow_weapon_names[] = { "눈보라 단검", "빙결 창", "얼음 망치", "서리 채찍", "빙하 둔기",
	"서리 고리검", "눈꽃 활", "빙하 포크암", "크리스탈 단검", "스노우 킬러"
};

static const char* s_desert_weapon_names[] = { "모래 폭풍 검", "태양의 창", "사막의 칼날", "오아시스 활", "용의 키스소드",
	"선라이즈 뱅가드", "사구 채찍", "안개의 검", "선셋 섬광검", "황금 척후병검"
};

static const char* s_forest_armor_names[] = { "숲의 가죽 갑옷", "초록 갑옷", "이끼 로브", "덩굴 갑주", "나뭇잎 투구",
	"자연의 방패", "숲 수호자 갑옷", "나무깃털 튜닉", "엔트의 판금", "별의 수호갑옷"
};

static const char* s_snow_armor_names[] = { "겨울 로브", "얼음 판금", "서리 망토", "빙하 갑옷", "눈보라 방패",
	"서리 투구", "빙결 경갑", "눈꽃 갑주", "크리스탈 실드", "겨울의 수호자"
};

static const char* s_desert_armor_names[] = { "모래 로브", "사막 가죽", "태양 경갑", "오아시스 갑주", "황금 투구",
	"바람의 갑옷", "태양의 방패", "선라이즈 판금", "모래 샌드루프", "황혼의 투구"
};

static const char* s_select_strong_name(const char** list, size_t count, int stage)
{
	if (stage < 1) {
		stage = 1;
	}
	if (stage > MAX_STAGE) {
		stage = MAX_STAGE;
	}

	size_t segment = (count + MAX_STAGE - 1) / MAX_STAGE; // 각 단계별로 아이템을 나눔
	size_t start = (stage - 1) * segment;

	if (start >= count) {
		start = count - 1; // 범위를 벗어나면 마지막 아이템으로 설정
	}

	size_t end = start + segment;
	if (end > count) {
		end = count; // 범위를 벗어나면 마지막 아이템까지
	}

	size_t range = end - start;
	return list[start + rand() % range];
}

const char* get_random_weapon_name(region_t region, int stage)
{
	switch (region)
	{
		case REGION_FOREST:
			return s_select_strong_name(s_forest_weapon_names, 10, stage);
			break;
		case REGION_SNOW:
			return s_select_strong_name(s_snow_weapon_names, 10, stage);
			break;
		case REGION_DESERT:
			return s_select_strong_name(s_desert_weapon_names, 10, stage);
			break;
		default:
			return "기본 검";
	}
}

const char* get_random_armor_name(region_t region, int stage)
{
	switch (region)
	{
	case REGION_FOREST:
		return s_select_strong_name(s_forest_armor_names, 10, stage);
		break;
	case REGION_SNOW:
		return s_select_strong_name(s_snow_armor_names, 10, stage);
		break;
	case REGION_DESERT:
		return s_select_strong_name(s_desert_armor_names, 10, stage);
		break;
	default:
		return "기본 갑옷";
	}
}

void create_weapon_item(item_t* item, region_t region, int stage)
{
	strcpy(item->name, get_random_weapon_name(region, stage));
	item->type = ITEM_TYPE_WEAPON;
	item->quantity = 1;
	item->region = region;
	int base = 5 + rand() % 6;
	item->attack_bonus = base * stage;
	item->defence_bonus = 0.0f; // 무기는 방어력 보너스 없음
	item->use_item = use_weapon;
}

void create_armor_item(item_t* item, region_t region, int stage)
{
	strcpy(item->name, get_random_armor_name(region, stage));
	item->type = ITEM_TYPE_ARMOR;
	item->quantity = 1;
	item->region = region;
	int base = 5 + rand() % 6;
	item->attack_bonus = 0; // 방어구는 공격력 보너스 없음
	item->defence_bonus = 0.02f + (rand() / (float)RAND_MAX) * 0.03f; // 2% ~ 5% 방어력 보너스 
	item->use_item = use_armor;
}

void create_consumable_item(item_t* item, item_type_t type, int quantity)
{
	item->type = type;
	item->quantity = quantity;
	switch (type) {
	case ITEM_TYPE_HEAL_S:
		strcpy(item->name, "소형 회복제");
		item->use_item = use_heal_potion;
		break;
	case ITEM_TYPE_HEAL_M:
		strcpy(item->name, "중형 회복제");
		item->use_item = use_heal_potion;
		break;
	case ITEM_TYPE_HEAL_L:
		strcpy(item->name, "대형 회복제");
		item->use_item = use_heal_potion;
		break;
	case ITEM_TYPE_HEAL_MAX:
		strcpy(item->name, "최대 회복제");
		item->use_item = use_heal_potion;
		break;
	case ITEM_TYPE_STAT_UP:
		strcpy(item->name, "스탯 증가 아이템");
		item->use_item = use_stat_up;
		break;
	default:
		strcpy(item->name, "알 수 없는 아이템");
		item->use_item = NULL; // 사용 불가능한 아이템
		break;
	}
}

void create_loot_item(item_t* item, region_t region)
{
	item->type = ITEM_TYPE_LOOT;
	// TODO
}

void use_heal_potion(item_t* item, player_t* player)
{
	switch (item->type) {
	case ITEM_TYPE_HEAL_S:
		player->current_hp += 10;
		break;
	case ITEM_TYPE_HEAL_M:
		player->current_hp += 40;
		break;
	case ITEM_TYPE_HEAL_L:
		player->current_hp += 100;
		break;
	case ITEM_TYPE_HEAL_MAX:
		player->current_hp = player->max_hp;
		return;
	default:
		return;
	}

	if (player->current_hp >= player->max_hp) {
		player->current_hp = player->max_hp;
	}
}

void use_stat_up(item_t* item, player_t* player)
{
	// 0: attack, 1: max_hp 2: speed, 3: evasion_rate, 4: defence_rate
	int stat = rand() % 5;
	if (0 >= stat && stat < 3) {
		int amout = rand() % 51; // 0 ~ 50
		switch (stat) {
		case 0:
			player->attack += amout;
			break;
		case 1:
			player->max_hp += amout;
			break;
		case 2:
			player->speed += amout;
			break;
		default:
			break;
		}
	}
	else if (stat < 5) {
		float base = 0.05f;
		float range = 0.1f;
		float amount = base + (rand() / (float)RAND_MAX) * range; // 0.5 ~ 1.5%
		switch (stat) 
		{
		case 3:
			player->evasion_rate += amount;
			break;
		case 4:
			player->defence_rate += amount;
			break;
		default:
			break;
		}
	}
}

void use_weapon(item_t * item, player_t* player)
{
	// TODO
}

void use_armor(item_t* item, player_t* player)
{
	// TODO
}
