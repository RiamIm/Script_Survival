#pragma once
#include "player.h"
#include "region.h"

typedef struct item item_t;
typedef struct player player_t;

typedef enum item_type {
	ITEM_TYPE_NULL, // ��� ����
	ITEM_TYPE_HEAL_S,
	ITEM_TYPE_HEAL_M,
	ITEM_TYPE_HEAL_L,
	ITEM_TYPE_HEAL_MAX,
	ITEM_TYPE_STAT_UP,
	ITEM_TYPE_WEAPON,
	ITEM_TYPE_ARMOR,
	ITEM_TYPE_LOOT, // ����ǰ ������
} item_type_t;

typedef struct item {
	char name[64];
	item_type_t type;
	int quantity;
	region_t region;
	int attack_bonus;
	float defence_bonus;
	void (*use_item)(item_t* item, player_t* player); // ���� ȿ��
} item_t;

const char* get_random_weapon_name(region_t region, int stage);
const char* get_random_armor_name(region_t region, int stage);

// ������ �ʱ�ȭ
void create_weapon_item(item_t* item, region_t region, int stage);
void create_armor_item(item_t* item, region_t region, int stage);
void create_consumable_item(item_t* item, item_type_t type, int quantity);
void create_loot_item(item_t* item, region_t region);



// ������ ȿ��
void use_heal_potion(item_t* item, player_t* player);
void use_stat_up(item_t* item, player_t* player);
void use_weapon(item_t* item, player_t* player);
void use_armor(item_t* item, player_t* player);