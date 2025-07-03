#pragma once
#include "player.h"

typedef struct item item_t;
typedef struct player player_t;

typedef enum item_type {
	ITEM_TYPE_HEAL_S,
	ITEM_TYPE_HEAL_M,
	ITEM_TYPE_HEAL_L,
	ITEM_TYPE_HEAL_MAX,
	ITEM_TYPE_STAT_UP,
	ITEM_TYPE_WEAPON,
	ITEM_TYPE_ARMOR
} item_type_t;

typedef struct item {
	item_type_t type;
	void (*use_item)(item_t* item, player_t* player);
} item_t;

// 아이템 초기화
void init_item(item_t* item, item_type_t type);

// 아이템 효과
void use_heal_potion(item_t* item, player_t* player);
void use_stat_up(item_t* item, player_t* player);
void use_weapon(item_t* item, player_t* player);
void use_armor(item_t* item, player_t* player);