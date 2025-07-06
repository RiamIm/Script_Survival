#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "player.h"
#include "item.h"

#define MAX_STAGE 12

void create_consumable_item(item_t* item, item_type_t type, int quantity)
{
    //item->type = type;
    //item->quantity = quantity;
    //switch (type) {
    //case ITEM_TYPE_HEAL_S:
    //    strcpy(item->name, "소형 회복제");
    //    item->use_item = use_heal_potion; // Ensure the function pointer is correctly assigned
    //    break;
    //case ITEM_TYPE_HEAL_M:
    //    strcpy(item->name, "중형 회복제");
    //    item->use_item = use_heal_potion;
    //    break;
    //case ITEM_TYPE_HEAL_L:
    //    strcpy(item->name, "대형 회복제");
    //    item->use_item = use_heal_potion;
    //    break;
    //case ITEM_TYPE_HEAL_MAX:
    //    strcpy(item->name, "최대 회복제");
    //    item->use_item = use_heal_potion;
    //    break;
    //case ITEM_TYPE_STAT_UP:
    //    strcpy(item->name, "스탯 증가 아이템");
    //    item->use_item = use_stat_up;
    //    break;
    //default:
    //    strcpy(item->name, "알 수 없는 아이템");
    //    item->use_item = NULL; // 사용 불가능한 아이템
    //    break;
    //}
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
