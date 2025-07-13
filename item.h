// item.h
#pragma once
#include "inout.h"
#include "utils.h"

#include "player.h"
#include "region.h"

//typedef struct item item_t;
typedef struct player player_t;

#define MAX_STAGE 12
#define WEAPON_COUNT 72
#define ARMOR_COUNT 72
#define HEAL_ITEM_COUNT 6

// 소비 아이템 구조체
typedef struct heal_item {
	char name[64];
	char description[256]; // 아이템 설명

	int hp_bonus;
} heal_item_t;

// 장비 등급 (일반, 희귀, 영웅, 유니크)
typedef enum {
    ITEM_NORMAL,
    ITEM_RARE,
    ITEM_HEROIC,
    ITEM_UNIQUE
} EquipmentGrade;

// 장비 지역 (숲, 사막, 설원)
typedef enum {
    ITEM_FOREST,
    ITEM_DESERT,
    ITEM_SNOWFIELD
} EquipmentRegion;

// 장비 구조체
// 아이템 이름, 아이템 설명, 공격력, 체력, 스피드, 회피율, 방어력
typedef struct equipment {
    char name[64];
    char description[256];

    EquipmentGrade grade;       
    EquipmentRegion region;     

    int attack_bonus;
    int max_hp_bonus;   
    int speed_bonus;

    double evasion_bonus;
    double defence_bonus;
} equipment_t;

extern equipment_t weapons[];
extern equipment_t armors[];
extern heal_item_t heal_items[];

void use_weapon(int next_index, player_t* player);
void use_armor(int next_index, player_t* player);