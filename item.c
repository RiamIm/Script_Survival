#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "player.h"
#include "item.h"

#define MAX_STAGE 12

// weapons[]
equipment_t weapons[] = {
	{ "Wooden Sword",       "A basic sword carved from wood. Easy to craft and common.",                10,  0,  0,  0.00f, 0.00f },
	{ "Rusty Dagger",       "A small dagger with a rusty blade. Lightweight but dull.",                   8,   0,  1,  0.05f, 0.00f },
	{ "Stone Axe",          "An axe head made of stone bound to a wooden handle. Heavy swing.",          12,  0, -1,  0.00f, 0.00f },
	{ "Bone Club",          "A crude club fashioned from animal bones. Cheap but sturdy.",                9,   0, -1,  0.00f, 0.00f },
	{ "Bronze Hammer",      "A heavy hammer forged from bronze. Slower but packs a punch.",              15,  0, -2,  0.00f, 0.05f },
	{ "Iron Sword",         "A reliable sword made of iron. Balanced in speed and power.",               18,  0, -1,  0.00f, 0.10f },
	{ "Steel Spear",        "A long spear with a steel tip. Excellent for keeping foes at a distance.",   16,  0,  0,  0.00f, 0.00f },
	{ "Crystal Blade",      "A blade forged from enchanted crystal. Sharp and surprisingly light.",      20,  0,  1,  0.10f, 0.00f },
	{ "Flame Scimitar",     "A curved sword imbued with fire magic. Burns enemies on each strike.",       22,  0,  1,  0.00f, 0.00f },
	{ "Thunder Axe",        "An axe crackling with lightning energy. Electrifies targets on hit.",       24,  0, -1,  0.10f, 0.00f },
};

// armors[]
equipment_t armors[] = {
	{ "Wooden Armor",         "Armor crafted from reinforced wood panels. Provides minimal protection.",      0, 15,  0, 0.00f, 0.05f },
	{ "Leather Vest",         "A vest made from treated leather. Lightweight and flexible.",                0, 20,  1, 0.05f, 0.10f },
	{ "Bronze Mail",          "Chainmail woven with bronze rings. Good defense at moderate weight.",         0, 22, -1, 0.00f, 0.12f },
	{ "Iron Plate",           "Heavy plate armor forged from iron. Excellent defense but slows you down.",  0, 30, -2, 0.00f, 0.20f },
	{ "Steel Mail",           "Chainmail made of steel links. Balanced protection and mobility.",           0, 25, -1, 0.00f, 0.15f },
	{ "Crystal Guard",        "Armor inlaid with crystal shards. Light yet surprisingly durable.",           0, 20,  1, 0.10f, 0.15f },
	{ "Flame Guard",          "Armor enchanted to resist fire. Protects against burning attacks.",           0, 18,  0, 0.00f, 0.20f },
	{ "Thunder Helm",         "A helm crackling with electricity. Shocks attackers on impact.",              0, 10,  0, 0.10f, 0.10f },
	{ "Shadow Cloak",         "A dark cloak that enhances agility and stealth. Evasion improved.",           0, 12,  2, 0.15f, 0.05f },
	{ "Dragon Scale Armor",   "Armor forged from dragon scales. Exceptional durability and defense.",        0, 40, -1, 0.00f, 0.25f },
};

item_t create_weapon_item(const region_t region, const equipment_t* weapon)
{
	item_t new_item;

	strcpy(new_item.name, weapon->name);
	strcpy(new_item.description, weapon->description);

	new_item.type = ITEM_TYPE_WEAPON;
	new_item.quantity = 1; // 무기는 항상 1개씩만 생성
	new_item.region = region;
	new_item.attack_bonus = weapon->attack_bonus;
	new_item.hp_bonus = weapon->hp_bonus;
	new_item.speed_bonus = weapon->speed_bonus;

	new_item.evasion_bonus = weapon->evasion_bonus;
	new_item.defence_bonus = weapon->defence_bonus;

	new_item.use_item = use_weapon;

	return new_item;
}

item_t create_armor_item(const region_t region, const equipment_t* armor)
{
	item_t new_item;

	strcpy(new_item.name, armor->name);
	strcpy(new_item.description, armor->description);

	new_item.type = ITEM_TYPE_ARMOR;
	new_item.quantity = 1; // 무기는 항상 1개씩만 생성
	new_item.region = region;
	new_item.attack_bonus = armor->attack_bonus;
	new_item.hp_bonus = armor->hp_bonus;
	new_item.speed_bonus = armor->speed_bonus;

	new_item.evasion_bonus = armor->evasion_bonus;
	new_item.defence_bonus = armor->defence_bonus;

	new_item.use_item = use_armor;

	return new_item;
}

item_t create_consumable_item(item_type_t type, int quantity)
{
	item_t new_item;
	switch (type) {
		case ITEM_TYPE_HEAL_S:
			strcpy(new_item.name, "소형 치유 물약");
			break;
		case ITEM_TYPE_HEAL_M:
			strcpy(new_item.name, "중형 치유 물약");
			break;
		case ITEM_TYPE_HEAL_L:
			strcpy(new_item.name, "대형 치유 물약");
			break;
		case ITEM_TYPE_HEAL_MAX:
			strcpy(new_item.name, "최대 치유 물약");
			break;
		case ITEM_TYPE_STAT_UP:
			strcpy(new_item.name, "스텟 증가 물약");
			break;
		default:
			break;
	}

	new_item.type = type;
	new_item.quantity = quantity;
	new_item.region = REGION_NULL; // 소비 아이템은 지역 정보가 없음
	new_item.attack_bonus = 0;
	new_item.hp_bonus = 0;
	new_item.speed_bonus = 0;
	new_item.evasion_bonus = 0.0f;
	new_item.defence_bonus = 0.0f;
	new_item.use_item = use_heal_potion; // 소비 아이템은 사용 함수가 없음
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

void use_weapon(item_t* item, player_t* player) 
{
	// 1) 이전 무기 해제
	if (player->equipped_weapon) {
		player->attack -= player->equipped_weapon->attack_bonus;
	}
	// 2) 새 무기 장착
	player->equipped_weapon = item;
	player->attack = player->attack + item->attack_bonus;
}

// 방어구 장착
void use_armor(item_t* item, player_t* player)
{
	if (player->equipped_armor) {
		player->defence_rate -= player->equipped_armor->defence_bonus;
	}
	player->equipped_armor = item;
	player->defence_rate = player->defence_rate + item->defence_bonus;
}