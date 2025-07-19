// log.c 
#define _CRT_SECURE_NO_WARNINGS
#include "log.h"

static char* s_log_buffer[LOG_MAX_LINES] = { NULL };
static int s_current_line_index = 0;

static void s_print_typing_anim(const char* str, int delay_ms) {
    for (int i = 0; str[i] != '\0'; i++) {
        putchar(str[i]);
        fflush(stdout);
        Sleep(delay_ms);
    }
}

static void s_log_print_buffer(void)
{
    int current_index = s_current_line_index;

    for (int i = 0; i < LOG_MAX_LINES; i++) {
        utils_gotoxy(LOG_START_X, LOG_START_Y + i);
        if (s_log_buffer[i]) {
            if (i == current_index) {
                utils_set_color(COLOR_CURRENT_LOG);
                s_print_typing_anim(s_log_buffer[i], 15);
            }
            else {
                utils_set_color(COLOR_HISTORY_LOG);
                printf("%-*s", LOG_BUFFER_SIZE - 1, s_log_buffer[i]);
            }
        }
        else {
            printf("%-*s", LOG_BUFFER_SIZE - 1, "");
        }
    }
    utils_set_color(COLOR_DEFAULT_TEXT);
}

static void s_log_push_line(void) {
    free(s_log_buffer[0]); // 가장 오래된 로그(0번 인덱스) 메모리 해제
    for (int i = 0; i < LOG_MAX_LINES - 1; i++) {
        s_log_buffer[i] = s_log_buffer[i + 1]; // 포인터를 한 칸씩 위로 당김
    }
    s_log_buffer[LOG_MAX_LINES - 1] = NULL; // 마지막 줄은 비움
    s_current_line_index--;
}

static void s_log_add_message(const char* fmt, ...)
{
    char tmp[LOG_BUFFER_SIZE];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(tmp, sizeof(tmp), fmt, ap);
    va_end(ap);

    if (s_current_line_index >= LOG_MAX_LINES) {
        s_log_push_line();
        s_current_line_index = LOG_MAX_LINES - 1;
    }

    s_log_buffer[s_current_line_index] = _strdup(tmp);
    
    s_log_print_buffer();
    s_current_line_index++;
}

void log_buffer_clear(void) {
    for (int i = 0; i < LOG_MAX_LINES; i++) {
        if (s_log_buffer[i]) {
            free(s_log_buffer[i]);
            s_log_buffer[i] = NULL;
        }
        utils_gotoxy(LOG_START_X, LOG_START_Y + i);
        printf("%-*s", LOG_BUFFER_SIZE - 1, "");
    }

    s_current_line_index = 0;
}

void log_player_focus(player_t* player) {
    s_log_add_message("%s이(가) 다음 행동을 위해 집중합니다.", player->name);
}

void log_player_attack(player_t* player, monster_t* monster, int damage, int break_damage) {
    s_log_add_message("%s이(가) %s에게 [피해 %d], [강인도 피해 %d]!",
        player->name, monster->name, damage, break_damage);
}

void log_monster_attack(player_t* player, monster_t* monster, int damage) {
    s_log_add_message("%s이(가) %s에게 %d의 피해를 입혔습니다.",
        monster->name, player->name, damage);
}

void log_evaded(const char* defender_name, const char* attacker_name) {
    s_log_add_message("%s이(가) %s의 공격을 회피했습니다!",
        defender_name, attacker_name);
}

// 몬스터가 그로기 상태에 빠졌을 때 호출
void log_monster_groggy(const char* monster_name) {
    s_log_add_message("%s의 강인도가 모두 파괴되어 그로기 상태에 빠집니다!", monster_name);
}

// 몬스터가 기절해서 턴을 넘길 때 호출
void log_monster_stunned(const char* monster_name) {
    s_log_add_message("%s은(는) 기절해서 움직일 수 없습니다.", monster_name);
}

// 몬스터가 기절에서 회복될 때 호출
void log_monster_recovers(const char* monster_name) {
    s_log_add_message("%s이(가) 기절에서 회복하며 자세를 가다듬습니다.", monster_name);
}