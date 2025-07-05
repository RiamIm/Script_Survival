#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "player.h"
#include "item.h"

#define MAX_STAGE 12

static const char* s_forest_weapon_names[] = { "���� ��", "�׸��� ��", "���� ����", "�ʷ� ��", "��ũ â",
"û�� �ܰ�", "�̳� ������", "���� ä��", "���� Ȱ", "������ �θ޶�"
};

static const char* s_snow_weapon_names[] = { "������ �ܰ�", "���� â", "���� ��ġ", "���� ä��", "���� �б�",
	"���� ����", "���� Ȱ", "���� ��ũ��", "ũ����Ż �ܰ�", "����� ų��"
};

static const char* s_desert_weapon_names[] = { "�� ��ǳ ��", "�¾��� â", "�縷�� Į��", "���ƽý� Ȱ", "���� Ű���ҵ�",
	"�������� �𰡵�", "�籸 ä��", "�Ȱ��� ��", "���� ������", "Ȳ�� ô�ĺ���"
};

static const char* s_forest_armor_names[] = { "���� ���� ����", "�ʷ� ����", "�̳� �κ�", "���� ����", "������ ����",
	"�ڿ��� ����", "�� ��ȣ�� ����", "�������� Ʃ��", "��Ʈ�� �Ǳ�", "���� ��ȣ����"
};

static const char* s_snow_armor_names[] = { "�ܿ� �κ�", "���� �Ǳ�", "���� ����", "���� ����", "������ ����",
	"���� ����", "���� �氩", "���� ����", "ũ����Ż �ǵ�", "�ܿ��� ��ȣ��"
};

static const char* s_desert_armor_names[] = { "�� �κ�", "�縷 ����", "�¾� �氩", "���ƽý� ����", "Ȳ�� ����",
	"�ٶ��� ����", "�¾��� ����", "�������� �Ǳ�", "�� �������", "Ȳȥ�� ����"
};

static const char* s_select_strong_name(const char** list, size_t count, int stage)
{
	if (stage < 1) {
		stage = 1;
	}
	if (stage > MAX_STAGE) {
		stage = MAX_STAGE;
	}

	size_t segment = (count + MAX_STAGE - 1) / MAX_STAGE; // �� �ܰ躰�� �������� ����
	size_t start = (stage - 1) * segment;

	if (start >= count) {
		start = count - 1; // ������ ����� ������ ���������� ����
	}

	size_t end = start + segment;
	if (end > count) {
		end = count; // ������ ����� ������ �����۱���
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
			return "�⺻ ��";
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
		return "�⺻ ����";
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
	item->defence_bonus = 0.0f; // ����� ���� ���ʽ� ����
	item->use_item = use_weapon;
}

void create_armor_item(item_t* item, region_t region, int stage)
{
	strcpy(item->name, get_random_armor_name(region, stage));
	item->type = ITEM_TYPE_ARMOR;
	item->quantity = 1;
	item->region = region;
	int base = 5 + rand() % 6;
	item->attack_bonus = 0; // ���� ���ݷ� ���ʽ� ����
	item->defence_bonus = 0.02f + (rand() / (float)RAND_MAX) * 0.03f; // 2% ~ 5% ���� ���ʽ� 
	item->use_item = use_armor;
}

void create_consumable_item(item_t* item, item_type_t type, int quantity)
{
	item->type = type;
	item->quantity = quantity;
	switch (type) {
	case ITEM_TYPE_HEAL_S:
		strcpy(item->name, "���� ȸ����");
		item->use_item = use_heal_potion;
		break;
	case ITEM_TYPE_HEAL_M:
		strcpy(item->name, "���� ȸ����");
		item->use_item = use_heal_potion;
		break;
	case ITEM_TYPE_HEAL_L:
		strcpy(item->name, "���� ȸ����");
		item->use_item = use_heal_potion;
		break;
	case ITEM_TYPE_HEAL_MAX:
		strcpy(item->name, "�ִ� ȸ����");
		item->use_item = use_heal_potion;
		break;
	case ITEM_TYPE_STAT_UP:
		strcpy(item->name, "���� ���� ������");
		item->use_item = use_stat_up;
		break;
	default:
		strcpy(item->name, "�� �� ���� ������");
		item->use_item = NULL; // ��� �Ұ����� ������
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
