#pragma once
#include "player.h"
#include "region.h"

typedef struct item item_t;
typedef struct player player_t;

//typedef enum item_type {
//	ITEM_TYPE_NULL, // 비어 있음
//	ITEM_TYPE_HEAL,
//	ITEM_TYPE_STAT_UP,
//	ITEM_TYPE_WEAPON,
//	ITEM_TYPE_ARMOR,
//	ITEM_TYPE_LOOT, // 전리품 아이템
//} item_type_t;

// 소비 아이템 구조체
typedef struct heal_item {
	char name[64];
	char description[256]; // 아이템 설명

	int hp_bonus;
} heal_item_t;

// 장비 구조체
// 아이템 이름, 아이템 설명, 공격력, 체력, 스피드, 회피율, 방어력
typedef struct equipment {
	char name[64];
	char description[256];	

	int attack_bonus;
	int max_hp_bonus;
	int speed_bonus;

	float evasion_bonus;
	float defence_bonus;
} equipment_t;

equipment_t weapons[];

equipment_t armors[];

// 아이템 초기화
