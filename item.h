// item.h
#pragma once
#include "inout.h"
#include "utils.h"

typedef struct player player_t;

// ==================== 아이템 관련 상수 정의 ====================
#define NORMAL_ITEM_COUNT 24
#define RARE_ITEM_COUNT 18
#define EPIC_ITEM_COUNT 12
#define UNIQUE_ITEM_COUNT 6

#define ITEMS_PER_PAGE 6
#define ITEMS_PER_ROW 3

#define RARITY_COUNT 4

#define SET_EFFECT_COUNT 6

extern const int rarity_item_counts[RARITY_COUNT];
// =============================================================

#define EQUIPMENTS_COUNT 60

#define ITEM_COUNT 24

#define HEAL_ITEM_COUNT 6

#define BUFFER_SIZE 1024

typedef enum {
    ITEM_TYPE_WEAPON,
    ITEM_TYPE_ARMOR,
    ITEM_TYPE_HEAL_ITEM
} item_type_t;

// 장비 등급 (일반, 희귀, 영웅, 유니크)
typedef enum equipment_rarity {
    RARITY_NORMAL = 0,
    RARITY_RARE,
    RARITY_EPIC,
    RARITY_UNIQUE,
} equipment_rarity_t;

extern char rarity_name[RARITY_COUNT];

// 장비 구조체
typedef struct equipment {
    char name[64];
    char description[256];

    equipment_rarity_t rarity;  

    int attack_bonus;
    int max_hp_bonus;   
    int speed_bonus;

    double evasion_bonus;
    double defence_bonus;

	int buy_price;
	int sell_price;

	int id; // 아이템 고유 ID
} equipment_t;

// 소비 아이템 구조체
typedef struct heal_item {
    char name[64];
    char description[256]; 

    int hp_bonus;

    int buy_price;  
    int sell_price; 
} heal_item_t;  

// 세트 효과 구조체
typedef struct set_effect {
    char name[64];
	char description[256];

    int id;
} set_effect_t;

equipment_t temp_weapons[EQUIPMENTS_COUNT];
equipment_t temp_armors[EQUIPMENTS_COUNT];

extern equipment_t weapons[RARITY_COUNT][ITEM_COUNT];
extern equipment_t armors[RARITY_COUNT][ITEM_COUNT];

extern heal_item_t heal_items[HEAL_ITEM_COUNT];

extern set_effect_t set_effects[SET_EFFECT_COUNT];

// 정적 함수
static void s_read_equipment_csv(const char* filename, equipment_t items[], int max_items);
static equipment_rarity_t s_get_rarity_from_string(const char* str);

void item_init(void);

void use_weapon(equipment_rarity_t rarity, int next_index, player_t* player);
void use_armor(equipment_rarity_t rarity, int next_index, player_t* player);
bool use_heal_item(int item_index, player_t* player);

void apply_set_effects(player_t* player, int selected_index);

void remove_set_effects(player_t* player);
