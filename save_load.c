#define _CRT_SECURE_NO_WARNINGS

#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#include "save_load.h"
#include "inventory.h"

void load_save_slot_info(save_slot_info_t* slots)
{
    for (int i = 0; i < 3; i++) {
        char filename[64];
        snprintf(filename, sizeof(filename), "save/save%d.dat", i);

        struct stat file_stat;
        if (stat(filename, &file_stat) == 0) {
            slots[i].exists = true;

            // 마지막 수정 시간 포맷팅
            struct tm* tm_info = localtime(&file_stat.st_mtime);
            strftime(slots[i].timestamp, sizeof(slots[i].timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
        }
        else {
            slots[i].exists = false;
            strcpy(slots[i].timestamp, "세이브 파일 없음");
        }
    }
}

void save_slot(int slot, const game_context_t* context)
{
    char filename[64];
    snprintf(filename, sizeof(filename), "save/save%d.dat", slot);

    FILE* file = fopen(filename, "wb");
    if (!file) {
        perror("파일 열기 실패");
        return;
    }

    fwrite(context, sizeof(game_context_t), 1, file);
    fwrite(weapon_inventory, sizeof(weapon_inventory), 1, file);
    fwrite(armor_inventory, sizeof(armor_inventory), 1, file);
    fwrite(heal_item_inventory, sizeof(heal_item_inventory), 1, file);
    fclose(file);
}

bool load_slot(int slot, game_context_t* context)
{
    char filename[64];
    snprintf(filename, sizeof(filename), "save/save%d.dat", slot);

    FILE* file = fopen(filename, "rb");
    if (!file)
        return false;

    fread(context, sizeof(game_context_t), 1, file);
    fread(weapon_inventory, sizeof(weapon_inventory), 1, file);
    fread(armor_inventory, sizeof(armor_inventory), 1, file);
    fread(heal_item_inventory, sizeof(heal_item_inventory), 1, file);
    fclose(file);
    return true;
}