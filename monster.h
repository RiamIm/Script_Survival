#ifndef MONSTER_H
#define MONSTER_H

#include <stdbool.h>

typedef struct monster {
    char name[20];
    char image[13][100];

    int attack;
    int max_hp;
    int current_hp;
    int speed;

    float evasion_rate;
    float defence_rate;

} monster_t;

extern monster_t monsters[30];
extern int type;
extern bool monster_active;

void init_monster(monster_t* monster, int index);
void monster_type(int map);
void monster_itemdrop(void);

#endif // MONSTER_H