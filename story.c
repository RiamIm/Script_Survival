// story.c
#include "story.h"
#include "utils.h"
#include "log.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STORY_FILE "data/story_seen.dat"
#define MAX_STORIES  32
static char* seen_list[MAX_STORIES];
static int  seen_count = 0;

// 스토리 함수 포인터 래퍼
void story_play(const char* storyId, void (*storyFunc)(void)) {
    bool seen = story_has_seen(storyId);

    if (!seen) {
        // 1) 한 번도 보지 않은 스토리: 무조건 플레이
        storyFunc();
        story_mark_seen(storyId);
        return;
    }
    
    int box_w = 40, box_h = 5;
    int x = (WIDTH - box_w) / 2;
    int y = (HEIGHT - box_h) / 2;


    // 질문 텍스트
    const char* msg1 = "이미 본 스토리입니다.";
    const char* msg2 = "스토리를 스킵하시겠습니까? (Y/N)";
    utils_set_color(COLOR_DEFAULT_TEXT);
    utils_gotoxy(x + (box_w - strlen(msg1)) / 2, y + 1);
    printf("%s", msg1);
    utils_gotoxy(x + (box_w - strlen(msg2)) / 2, y + 3);
    printf("%s", msg2);

    // 입력 대기
    char c;
    do {
        c = utils_getch();
        if (c == 'y' || c == 'Y' || c == 'ㅛ') {
            // 스킵
            UI_cleaner_all_display();
            log_buffer_clear();
            break;
        }
        else if (c == 'n' || c == 'N' || c == 'ㅜ') {
            // 재생
            UI_cleaner_all_display();
            storyFunc();
            break;
        }
    } while (1);

    // ──────────────────────────────────────────────
}

void story_init(void) {
    FILE* f = fopen(STORY_FILE, "r");
    if (!f) return;
    char buf[64];
    while (fgets(buf, sizeof(buf), f)) {
        buf[strcspn(buf, "\r\n")] = 0;
        if (seen_count < MAX_STORIES)
            seen_list[seen_count++] = _strdup(buf);
    }
    fclose(f);
}

bool story_has_seen(const char* id) {
    for (int i = 0; i < seen_count; i++)
        if (strcmp(seen_list[i], id) == 0)
            return true;
    return false;
}

void story_mark_seen(const char* id) {
    if (story_has_seen(id)) return;
    if (seen_count < MAX_STORIES) {
        seen_list[seen_count++] = _strdup(id);
        FILE* f = fopen(STORY_FILE, "a");
        if (f) {
            fprintf(f, "%s\n", id);
            fclose(f);
        }
    }
}
