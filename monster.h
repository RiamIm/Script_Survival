#pragma once

#include "inout.h"
#include "utils.h"

#include "player.h"

#define MAX_MONSTER 13
#define MAX_IMAGE_LINES 13

typedef struct {
    char     name[20];
    wchar_t* image[MAX_IMAGE_LINES];
    int      attack;
    int      max_hp;
    int      current_hp;
    int      max_toughness;
    int      current_toughness;
    int      speed;
    double   action_value;
    bool     is_groggy;
    int      stun_turns;
    double   evasion_rate;
    double   defence_rate;
} monster_t;

/**
 * monster 초기화
 *   - monsters.csv 에서 index 행 스탯 로드
 *   - change.log 에서 index 블록 와이드로 읽어 이미지 로드
 */
bool monster_init(monster_t* m, int monster_index);

void monster_item_drop(player_t* player, int index);
