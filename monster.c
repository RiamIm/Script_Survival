// monster.c
#define _CRT_SECURE_NO_WARNINGS
#include "monster.h"

monster_t monsters[30] = {

    {"더미", {
    L"               a               ",
    L"             =aaa=             ",
    L"               a               ",
    L"               a               "
}, 0, 99999, 99999, 0, 0, 0, 0, 0, 0, 0, 0},

{"슬라임", {
    L"            ▄▄▄▄▄▄■■■■■■■■■  ■■■■■■■■■▄▄▄▄▄▄",
    L"                                          .-'     '-.                                           ",
    L"                                         .'           '.                                         ",
    L"                                        /   O     O    \\                                         ",
    L"                                       |                |                                        ",
    L"                                       |    \\  ^  /     |                                        ",
    L"                                       |     \\___/      |                                        ",
    L"                                       |                |                                        ",
    L"                                       \\               /                                         ",
    L"                                        '.           .'                                          ",
    L"                                          '-._____.-'                                            ",
    L"                                            '.....'                                              ",
    L"                                                                                                 "
}, 10, 2000, 2000, 200, 200, 300, 0, 0, 0, 0.05, 0.05},

{"버섯몬", {
    L"                                                                                                  ",
    L"                                           .-''''''-.                                           ",
    L"                                        .''          ''.                                        ",
    L"                                     .'    .-''''-.    '.                                     ",
    L"                                   /    (  O  O  )     \\                                   ",
    L"                                  /      \\   ^   /       \\                                  ",
    L"                                 ;        '-----'        ;                                 ",
    L"                                 |                       |                                 ",
    L"                                  \\                     /                                  ",
    L"                                   '.                 .'                                   ",
    L"                                     '-.______.--'                                     ",
    L"                                        |      |                                        ",
    L"                                        '------'                                        "
}, 20, 200, 200, 100, 100, 300, 0, 0, 0, 0.05, 0.5}

};

int type=2;
bool monster_active = true;

void monster_init(monster_t* monster, int index)
{
    strcpy(monster->name, monsters[index].name);
    for (int i = 0; i < 13; ++i)
    {
        wcscpy(monster->image[i], monsters[index].image[i]); // wchar_t*용 복사 함수
    }
    monster->attack = monsters[index].attack;
    monster->max_hp = monsters[index].max_hp;
    monster->current_hp = monsters[index].current_hp;
    monster->max_toughness = monsters[index].max_toughness;
    monster->current_toughness = monsters[index].current_toughness;

    monster->speed = monsters[index].speed;

    monster->action_value = 0;
    monster->is_groggy = false;
    monster->stun_turns = 0;

    monster->evasion_rate = monsters[index].evasion_rate;
    monster->defence_rate = monsters[index].defence_rate;

    //monster_type(0); 
   //monster_itemdrop();
}

//void monster_type(int map)
//{
//    switch (map)
//    {
//    case0: type = 0;
//        break;
//    case1: type = rand() % 1 + 1;
//        break;
//    //case2: type = rand() % 1
//    }
//}

//void monster_itemdrop()
//{
//    int pick = 0;
//    switch (type)
//    {
//    // 더미 
//        case 0:
//        pick = rand() % 5;
//            switch (pick)
//            {
//            case 0: 
//
//                break;
//            }
//
//        break;
//    // 슬라임 
//        case 1:
//            pick = rand() % 5;
//            switch (pick)
//            {
//            case 0:
//                break;
//            }
//    // 숲골렘 
//        case 2:
//            pick = rand() % 5;
//            switch (pick)
//            {
//            case 0:
//                break;
//            }
//        break;
//        case 3:
//        break;
//    }
//}