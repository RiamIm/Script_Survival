// log.c

#include "log.h"


// 로그창 시작 좌표 (UI_info.h 에 정의된 매크로가 있으면 그걸로 교체하세요)
#define LOG_START_X   42
#define LOG_START_Y   21

// -----------------------------------------------------------------
// 전역 로그 버퍼 (char* 배열, NULL 가능)
// -----------------------------------------------------------------
char* log_buffer[LOG_MAX_LINES] = { NULL };

// -----------------------------------------------------------------
// 내부: 한 줄 밀어내기 → 맨 위 로그 제거, 위로 당기고 맨 아래 빈 칸
// 그리고 화면에도 그대로 다시 그려줌
// -----------------------------------------------------------------
void s_log_push_line(void)
{
    // 1) 가장 오래된 로그 해제
    if (log_buffer[0]) {
        free(log_buffer[0]);
    }
    // 2) 한 칸씩 위로 당기기
    for (int i = 0; i < LOG_MAX_LINES - 1; i++) {
        log_buffer[i] = log_buffer[i + 1];
    }
    // 3) 맨 아래는 비워두기
    log_buffer[LOG_MAX_LINES - 1] = NULL;

    // 4) 화면에 5줄 모두 다시 그리기
    for (int i = 0; i < LOG_MAX_LINES; i++) {
        utils_gotoxy(LOG_START_X, LOG_START_Y + i);
        if (log_buffer[i]) {
            // 메시지 폭만큼 출력 (나머지 공백)
            printf("%-*s", LOG_BUFFER_SIZE - 1, log_buffer[i]);
        }
        else {
            // 빈 줄 클리어
            printf("%-*s", LOG_BUFFER_SIZE - 1, "");
        }
    }
}

// -----------------------------------------------------------------
// 로그 전체 초기화 (버퍼 해제 + 화면 클리어)
// -----------------------------------------------------------------
void log_buffer_clear(void)
{
    for (int i = 0; i < LOG_MAX_LINES; i++) {
        if (log_buffer[i]) {
            free(log_buffer[i]);
            log_buffer[i] = NULL;
        }
        utils_gotoxy(LOG_START_X, LOG_START_Y + i);
        printf("%-*s", LOG_BUFFER_SIZE - 1, "");
    }
}

// -----------------------------------------------------------------
// 내부 공통: 가변인자로 메시지 포맷 → 푸시 → 맨 아래 출력
// -----------------------------------------------------------------
static void log_common(const char* fmt, ...)
{
    char tmp[LOG_BUFFER_SIZE];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(tmp, sizeof(tmp), fmt, ap);
    va_end(ap);

    // 로그 한 칸 밀어내고
    s_log_push_line();

    // 맨 아래에 새 메시지 저장
    log_buffer[LOG_MAX_LINES - 1] = _strdup(tmp);

    // 그리고 화면 맨 아래 줄에 출력
    utils_gotoxy(LOG_START_X, LOG_START_Y + (LOG_MAX_LINES - 1));
    printf("%s", tmp);
}

// -----------------------------------------------------------------
// 이벤트별 로그 함수들
// -----------------------------------------------------------------
void log_player_turn(player_t* player)
{
    log_common("%s의 턴입니다.", player->name);
}

void log_player_attack(player_t* player, monster_t* monster, int attack)
{
    log_common("%s가 %s에게 %d의 피해를 입혔습니다.",
        player->name, monster->name, attack);
}

void log_player_skill(player_t* player, monster_t* monster, int attack)
{
    log_common("%s가 %s에게 스킬을 사용하여 %d의 피해를 입혔습니다.",
        player->name, monster->name, attack);
}

void log_player_evade(player_t* player, monster_t* monster)
{
    log_common("%s가 %s의 공격을 회피했습니다.",
        player->name, monster->name);
}

void log_monster_turn(monster_t* monster)
{
    log_common("%s의 턴입니다.", monster->name);
}

void log_monster_attack(player_t* player, monster_t* monster, int attack)
{
    log_common("%s가 %s에게 %d의 피해를 입혔습니다.",
        monster->name, player->name, attack);
}

void log_monster_skill(player_t* player, monster_t* monster, int attack)
{
    log_common("%s가 %s에게 스킬을 사용하여 %d의 피해를 입혔습니다.",
        monster->name, player->name, attack);
}

void log_monster_evade(player_t* player, monster_t* monster)
{
    log_common("%s가 %s의 공격을 회피했습니다.",
        monster->name, player->name);
}

void log_deal_damage(player_t* player,
    monster_t* monster,
    int attack,
    bool is_player_attack)
{
    if (is_player_attack)
        log_player_attack(player, monster, attack);
    else
        log_monster_attack(player, monster, attack);
}

void log_take_damage(player_t* player,
    monster_t* monster,
    int attack,
    bool is_monster_attack)
{
    // 플레이어가 피해를 받으면 is_monster_attack == true
    if (is_monster_attack)
        log_player_attack(player, monster, attack);
    else
        log_monster_attack(player, monster, attack);
}

void log_evade(player_t* player, monster_t* monster)
{
    log_common("%s와 %s가 서로의 공격을 피했습니다.",
        player->name, monster->name);
}
