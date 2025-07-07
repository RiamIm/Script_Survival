#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "player.h"
#include "item.h"

#define MAX_STAGE 12

equipment_t weapons[] = {
	{ "나무 검",
	  "나무로 깎아 만든 기본 검입니다. 제작이 쉽고 흔하게 볼 수 있습니다.",
	  10,  0,  0,  0.00f, 0.00f },
	{ "녹슨 단검",
	  "날이 무뎌진 녹슨 단검입니다. 가볍지만 공격력이 약합니다.",
	  8,   0,  1,  0.05f, 0.00f },
	{ "돌도끼",
	  "돌로 된 도끼 머리를 나무 자루에 고정한 무기입니다. 강력하지만 무겁습니다.",
	  12,  0, -1,  0.00f, 0.00f },
	{ "뼈 곤봉",
	  "동물의 뼈로 만든 조악한 곤봉입니다. 싸구려지만 단단합니다.",
	  9,   0, -1,  0.00f, 0.00f },
	{ "청동 망치",
	  "청동으로 단조한 무거운 망치입니다. 느리지만 강력한 타격을 줍니다.",
	  15,  0, -2,  0.00f, 0.05f },
	{ "철검",
	  "견고한 철로 만든 검입니다. 속도와 공격력의 균형이 좋습니다.",
	  18,  0, -1,  0.00f, 0.10f },
	{ "강철 창",
	  "강철 끝을 가진 긴 창입니다. 적을 멀리서 공격하기에 좋습니다.",
	  16,  0,  0,  0.00f, 0.00f },
	{ "수정 검",
	  "마법의 수정으로 만든 검입니다. 날카롭고 의외로 가볍습니다.",
	  20,  0,  1,  0.10f, 0.00f },
	{ "화염 삭도",
	  "불 마법이 깃든 곡선형 검입니다. 타격 시 적에게 불을 붙입니다.",
	  22,  0,  1,  0.00f, 0.00f },
	{ "번개 도끼",
	  "번개 에너지가 흐르는 도끼입니다. 맞은 적에게 감전 효과를 줍니다.",
	  24,  0, -1,  0.10f, 0.00f },
};

// 한글화된 방어구 목록
equipment_t armors[] = {
	{ "나무 갑옷",
	  "강화된 나무 판자로 만든 갑옷입니다. 기본적인 보호를 제공합니다.",
	  0, 15,  0, 0.00f, 0.05f },
	{ "가죽 조끼",
	  "처리된 가죽으로 만든 조끼입니다. 가볍고 유연합니다.",
	  0, 20,  1, 0.05f, 0.10f },
	{ "청동 사슬갑",
	  "청동 고리로 엮은 사슬갑입니다. 적당한 무게에 좋은 방어력을 제공합니다.",
	  0, 22, -1, 0.00f, 0.12f },
	{ "철판 금속갑",
	  "철로 단조한 무거운 판금 갑옷입니다. 뛰어난 방어력 대신 속도를 늦춥니다.",
	  0, 30, -2, 0.00f, 0.20f },
	{ "강철 사슬갑",
	  "강철 고리로 만든 사슬갑입니다. 방어와 기동성의 균형이 좋습니다.",
	  0, 25, -1, 0.00f, 0.15f },
	{ "수정 수호갑",
	  "수정 조각으로 장식된 갑옷입니다. 가볍지만 내구성이 뛰어납니다.",
	  0, 20,  1, 0.10f, 0.15f },
	{ "화염 방어구",
	  "불에 대한 저항력을 부여하는 마법이 깃든 갑옷입니다.",
	  0, 18,  0, 0.00f, 0.20f },
	{ "천둥 투구",
	  "전기를 품은 투구입니다. 공격자를 감전시킵니다.",
	  0, 10,  0, 0.10f, 0.10f },
	{ "그림자 망토",
	  "민첩성과 은신을 강화해 주는 어두운 망토입니다.",
	  0, 12,  2, 0.15f, 0.05f },
	{ "용 비늘 갑옷",
	  "용의 비늘로 만든 갑옷입니다. 뛰어난 내구성과 방어력을 제공합니다.",
	  0, 40, -1, 0.00f, 0.25f },
};

heal_item_t heal_items[] = {
	{ "사과", "체력을 50 회복합니다.", 50 },
	{ "치유 물약", "체력을 100 회복합니다.", 100 },
	{ "강화 물약", "체력을 200 회복합니다.", 200 },
	{ "마법의 물약", "체력을 300 회복합니다.", 300 },
	{ "황금 사과", "최대 체력을 회복합니다.",  -1 },
	{ "신비한 돌", "랜덤으로 스텟이 증가합니다.", -2 },
};

void use_weapon(int next_index, player_t* player)
{
	int current_index = player->weapon_index;

	if (current_index != -1)
	{
		player->attack -= weapons[current_index].attack_bonus;
		player->max_hp -= weapons[current_index].max_hp_bonus;
		player->speed -= weapons[current_index].speed_bonus;

		player->evasion_rate -= weapons[current_index].evasion_bonus;
		player->defence_rate -= weapons[current_index].defence_bonus;
	}

	player->weapon_index = next_index;

	player->attack += weapons[next_index].attack_bonus;
	player->max_hp += weapons[next_index].max_hp_bonus;
	player->speed += weapons[next_index].speed_bonus;

	player->evasion_rate += weapons[next_index].evasion_bonus;
	player->defence_rate += weapons[next_index].defence_bonus;
}

void use_armor(int next_index, player_t* player)
{
	int current_index = player->armor_index;

	if (current_index != -1)
	{
		player->attack -= armors[current_index].attack_bonus;
		player->max_hp -= armors[current_index].max_hp_bonus;
		player->current_hp -= armors[current_index].max_hp_bonus;
		player->speed -= armors[current_index].speed_bonus;

		player->evasion_rate -= armors[current_index].evasion_bonus;
		player->defence_rate -= armors[current_index].defence_bonus;
	}

	player->armor_index = next_index;

	player->attack += armors[next_index].attack_bonus;
	player->max_hp += armors[next_index].max_hp_bonus;
	player->current_hp += armors[next_index].max_hp_bonus;
	player->speed += armors[next_index].speed_bonus;

	player->evasion_rate += armors[next_index].evasion_bonus;
	player->defence_rate += armors[next_index].defence_bonus;
}