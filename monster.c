#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "monster.h"

monster_t monsters[30] = { 


    {"¥ıπÃ", {"               a               ",
              "             =aaa=             ",
              "               a               ",
              "               a               "}
    ,0, 99999, 0, 0, 0, 0},
    {"ΩΩ∂Û¿”", {"                                              _____                                               \0",
        "                                           .-'     '-.                                           \0",
        "                                         .'           '.                                         \0",
        "                                        /   O     O    \\                                         \0",
        "                                      |                 |                                        \0",
        "                                       |    \\  ^  /     |                                        \0",
        "                                       |     \\___/      |                                        \0",
        "                                      |                 |                                        \0",
        "                                       \\               /                                         \0",
        "                                        '.           .'                                          \0",
        "                                          '-._____.-'                                            \0",
        "                                            '.....'                                              \0",
        "                                                                                                 \0"}
    ,10, 100, 100, 10, 5, 0.05},
    {"πˆº∏∏Û",  {"                                                                                                  \0",
    /*  1 */ "                                           .-''''''-.                                           \0",
    /*  2 */ "                                        .''          ''.                                        \0",
    /*  3 */ "                                     .'    .-''''-.    '.                                     \0",
    /*  4 */ "                                   /    (  O  O  )     \\                                   \0",
    /*  5 */ "                                  /      \\   ^   /       \\                                  \0",
    /*  6 */ "                                 ;        '-----'        ;                                 \0",
    /*  7 */ "                                 |                       |                                 \0",
    /*  8 */ "                                  \\                     /                                  \0",
    /*  9 */ "                                   '.                 .'                                   \0",
    /* 10 */ "                                     '-.______.--'                                     \0",
    /* 11 */ "                                        |      |                                        \0",
    /* 12 */ "                                        '------'                                        \0"}
    ,20, 200, 200, 5, 0, 0.5},
    {"Ω£∞Ò∑Ω", {"               a               ",
                "             =aaa=             ",
                "               a               ",
                "               a               "}
    ,50, 200, 200, 5, 0, 0.5},

};

int type=2;
bool monster_active = true;

void init_monster(monster_t* monster, int index)
{
	strcpy(monster->name, monsters[index].name);
    for (int i = 0; i < 13; ++i)
    {
        strcpy(monster->image[i], monsters[index].image[i]);
    }
	monster->attack = monsters[index].attack;
	monster->max_hp = monsters[index].max_hp;
    monster->current_hp = monsters[index].current_hp;
	monster->speed = monsters[index].speed;
	monster->evasion_rate = monsters[index].evasion_rate;
	monster->defence_rate = monsters[index].defence_rate;

    //monster_type(0); 
	//monster_itemdrop();
}

void monster_type(int map)
{
    switch (map)
    {
    case0: type = 0;
        break;
    case1: type = rand() % 1 + 1;
        break;
    //case2: type = rand() % 1
    }
    
    


}




void monster_itemdrop()
{
    int pick = 0;
    switch (type)
    {
    // ¥ıπÃ 
        case 0:
        pick = rand() % 5;
            switch (pick)
            {
            case 0: 

                break;
            }

        break;
    // ΩΩ∂Û¿” 
        case 1:
            pick = rand() % 5;
            switch (pick)
            {
            case 0:
                break;
            }
    // Ω£∞Ò∑Ω 
        case 2:
            pick = rand() % 5;
            switch (pick)
            {
            case 0:
                break;
            }
        break;
        case 3:
        break;
    }
}















