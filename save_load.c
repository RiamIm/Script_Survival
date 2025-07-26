#define _CRT_SECURE_NO_WARNINGS

#include <sys/stat.h>
#include <time.h>

#include "save_load.h"

void load_save_slot_info(save_slot_info_t* slots)
{
    for (int i = 0; i < 3; i++) {
        char filename[64];
        snprintf(filename, sizeof(filename), "save_slot_%d.dat", i + 1);

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

void save_game(const char* filename, game_context_t* context)
{
    FILE* file = fopen(filename, "wb");
    if (!file) {
        perror("파일 열기 실패");
        return;
    }
    fwrite(context, sizeof(game_context_t), 1, file);
    fclose(file);
}