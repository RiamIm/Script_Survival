// monster.h
#pragma once
#include "inout.h"
#include "utils.h"

typedef struct monster {
    char name[20];
    wchar_t image[13][100];

    int attack;
    int max_hp;
    int current_hp;
    int max_toughness;
    int current_toughness;
    int speed;

    double action_value;

    bool is_groggy;    // 그로기 상태 여부 플래그
    int stun_turns;    // 남은 기절 턴 수

    double evasion_rate;
    double defence_rate;

} monster_t;

extern monster_t monsters[30];
extern int type;
extern bool monster_active;

void monster_init(monster_t* monster, int index);
//void monster_type(int map);
//void monster_itemdrop(void);