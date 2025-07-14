// log.c

#include "log.h"

// -----------------------------------------------------------------
// 전역 로그 버퍼 (char* 배열, NULL 가능)
// -----------------------------------------------------------------
static char* s_log_buffer[LOG_MAX_LINES] = { NULL };

static int s_log_buffer_index = 0;

static void s_print_typing_anim(const char* str, int delay_ms) {
    for (int i = 0; str[i] != '\0'; i++) {
        putchar(str[i]);
        fflush(stdout);    // 버퍼를 비워줌 -> 즉시 출력   
        Sleep(delay_ms); 
    }
}

// -----------------------------------------------------------------
// 내부 공통: 버퍼에 있는 메세지를 출력하는 함수
// -----------------------------------------------------------------
static void s_log_print_buffer(void)
{
    for (int i = 0; i < LOG_MAX_LINES; i++) {
        utils_gotoxy(LOG_START_X, LOG_START_Y + i);
        if (s_log_buffer[i]) {
            if (i == s_log_buffer_index) {
				utils_set_color(COLOR_CURRENT_LOG);
                s_print_typing_anim(s_log_buffer[i], 15); // 타이핑 효과로 출력
            }
            else {
				utils_set_color(COLOR_HISTORY_LOG);
				printf("%-*s", LOG_BUFFER_SIZE - 1, s_log_buffer[i]);
            }
        }
        else {
            // 빈 줄 클리어
            printf("%-*s", LOG_BUFFER_SIZE - 1, "");
        }
    }
}

// -----------------------------------------------------------------
// 내부: 한 줄 밀어내기 → 맨 위 로그 제거, 위에서 아래로 출력하고, 꽉 차면 맨 윗줄부터 없애기
// 그리고 화면에도 그대로 다시 그려줌
// -----------------------------------------------------------------
//static void s_log_push_line(void)
//{
//    // 한 칸씩 위로 올리기
//    for (int i = LOG_MAX_LINES - 1; i > 0; i--) {
//        if (s_log_buffer[i - 1]) {
//            s_log_buffer[i] = NULL;
//            s_log_buffer[i] = s_log_buffer[i - 1];
//            s_log_buffer[i - 1] = NULL;
//        }
//	}
//
//    s_log_print_buffer();
//}

// -----------------------------------------------------------------
// 로그 전체 초기화 (버퍼 해제 + 화면 클리어)
// -----------------------------------------------------------------
void log_buffer_clear(void)
{
    for (int i = 0; i < LOG_MAX_LINES; i++) {
        if (s_log_buffer[i]) {
            free(s_log_buffer[i]);
            s_log_buffer[i] = NULL;
        }
        utils_gotoxy(LOG_START_X, LOG_START_Y + i);
        printf("%-*s", LOG_BUFFER_SIZE - 1, "");
    }

	s_log_buffer_index = 0; // 인덱스 초기화
}

// -----------------------------------------------------------------
// 내부 공통: 가변인자로 메시지 포맷 → 푸시 → 맨 아래 출력
// -----------------------------------------------------------------
static void s_log_common(const char* fmt, ...)
{
    char tmp[LOG_BUFFER_SIZE];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(tmp, sizeof(tmp), fmt, ap);
    va_end(ap);

	// 밑으로 보내면서 기존 맨 위 로그 제거
    //s_log_push_line();

    // 맨 위에 새 메시지 저장
    s_log_buffer[s_log_buffer_index] = _strdup(tmp);

    s_log_print_buffer();
    s_log_buffer_index++;
    if (s_log_buffer_index >= LOG_MAX_LINES) {
        s_log_buffer_index = 0; // 인덱스 초기화
    }
}

// -----------------------------------------------------------------
// 이벤트별 로그 함수들
// -----------------------------------------------------------------

void log_current_turn(int turn)
{
    s_log_common("<제%d 라운드>", turn);
}

void log_player_turn(player_t* player)
{
    s_log_common("%s의 턴입니다.", player->name);
}

void log_player_attack(player_t* player, monster_t* monster, int attack)
{
    s_log_common("%s이/가 %s에게 %d의 피해를 입혔습니다.",
        player->name, monster->name, attack);
}

void log_player_skill(player_t* player, monster_t* monster, int attack)
{
    s_log_common("%s가 %s에게 스킬을 사용하여 %d의 피해를 입혔습니다.",
        player->name, monster->name, attack);
}

void log_player_evade(player_t* player, monster_t* monster)
{
    s_log_common("%s가 %s의 공격을 회피했습니다.",
        player->name, monster->name);
}

void log_monster_turn(monster_t* monster)
{
    s_log_common("%s의 턴입니다.", monster->name);
}

void log_monster_attack(player_t* player, monster_t* monster, int attack)
{
    s_log_common("%s이/가 %s에게 %d의 피해를 입혔습니다.",
        monster->name, player->name, attack);
}

void log_monster_skill(player_t* player, monster_t* monster, int attack)
{
    s_log_common("%s가 %s에게 스킬을 사용하여 %d의 피해를 입혔습니다.",
        monster->name, player->name, attack);
}

void log_monster_evade(player_t* player, monster_t* monster)
{
    s_log_common("%s가 %s의 공격을 회피했습니다.",
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
    s_log_common("%s와 %s가 서로의 공격을 피했습니다.",
        player->name, monster->name);
}
