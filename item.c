#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "item.h"

void init_item(item_t* item, item_type_t type)
{
	item->type = type;
	switch (type) {
	case ITEM_TYPE_HEAL_S:
		/* intentional fallthrough */
	case ITEM_TYPE_HEAL_M:
		/* intentional fallthrough */
	case ITEM_TYPE_HEAL_L:
		/* intentional fallthrough */
	case ITEM_TYPE_HEAL_MAX:
		item->use_item = use_heal_potion;
		break;
	case ITEM_TYPE_STAT_UP:
		item->use_item = use_stat_up;
		break;
	case ITEM_TYPE_WEAPON:
		item->use_item = use_weapon;
		break;
	case ITEM_TYPE_ARMOR:
		item->use_item = use_armor;
		break;
	default:
		break;
	}
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
		float base = 0.005f;
		float range = 0.01f;
		float amount = base + (rand() / (float)RAND_MAX) * range; // 0.5 ~ 1.5%
		switch (stat) {
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
