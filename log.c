// log.c
#define _CRT_SECURE_NO_WARNINGS
#include "log.h"
#include <stdarg.h>

// 로그 메시지와 치명타 여부를 함께 저장하는 구조체
typedef struct {
    char* text;
    bool is_critical;
} log_entry_t;

static log_entry_t s_log_buffer[LOG_MAX_LINES] = { { NULL, false } };
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
        if (s_log_buffer[i].text) {
            if (i == current_index) {
                // 치명타일 경우 노란색(주황색 대용), 아닐 경우 흰색으로 설정
                if (s_log_buffer[i].is_critical) {
                    utils_set_color(COLOR_YELLOW);
                }
                else {
                    utils_set_color(COLOR_CURRENT_LOG);
                }
                s_print_typing_anim(s_log_buffer[i].text, 15);
            }
            else {
                // 이전 로그들은 회색으로 표시
                if (s_log_buffer[i].is_critical) {
                    utils_set_color(COLOR_BROWN); // 이전 치명타 로그는 갈색으로
                }
                else {
                    utils_set_color(COLOR_HISTORY_LOG);
                }
                printf("%-*s", LOG_BUFFER_SIZE - 1, s_log_buffer[i].text);
            }
        }
        else {
            printf("%-*s", LOG_BUFFER_SIZE - 1, "");
        }
    }
    utils_set_color(COLOR_DEFAULT_TEXT);
}

static void s_log_push_line(void) {
    for (int i = 0; i < LOG_MAX_LINES - 1; i++) {
        if (s_log_buffer[i].text) {
            free(s_log_buffer[i].text);
            s_log_buffer[i].text = NULL;
        }
		s_log_buffer[i].text = _strdup(s_log_buffer[i + 1].text);
	}

	free(s_log_buffer[LOG_MAX_LINES - 1].text);
    s_log_buffer[LOG_MAX_LINES - 1].text = NULL;
    
    s_log_print_buffer();
}

static void s_log_add_message(bool is_critical, const char* fmt, ...)
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

    s_log_buffer[s_current_line_index].text = _strdup(tmp);
    s_log_buffer[s_current_line_index].is_critical = is_critical;

    s_log_print_buffer();
    s_current_line_index++;
}

void log_buffer_clear(void) {
    for (int i = 0; i < LOG_MAX_LINES; i++) {
        if (s_log_buffer[i].text) {
            free(s_log_buffer[i].text);
            s_log_buffer[i].text = NULL;
            s_log_buffer[i].is_critical = false;
        }
        utils_gotoxy(LOG_START_X, LOG_START_Y + i);
        printf("%-*s", LOG_BUFFER_SIZE - 1, "");
    }
    s_current_line_index = 0;
}

// [수정] 모든 로그 함수들이 is_critical=false 로 s_log_add_message를 호출하도록 수정
void log_player_counter_ready(player_t* player) {
    s_log_add_message(false, "%s이(가) 반격을 준비합니다.", player->name);
}

void log_player_counter_success(player_t* player, monster_t* monster, int final_damage, int break_damage, bool is_critical) {
    char extra_damage_str[20] = "";

    // [수정] "[치명타!]" 텍스트 생성 로직 삭제
    s_log_add_message(is_critical, "%s의 반격! %s에게 [피해 %d%s], [강인도 피해 %d]",
        player->name, monster->name, final_damage, extra_damage_str, break_damage);
}

void log_player_attack(player_t* player, monster_t* monster, int damage, int break_damage, bool is_critical, int break_extra_damage_dealt) {
    char extra_damage_str[20] = "";
    if (break_extra_damage_dealt > 0) {
        snprintf(extra_damage_str, sizeof(extra_damage_str), " (+%d)", break_extra_damage_dealt);
    }

    // [수정] "[치명타!]" 텍스트 생성 로직 삭제
    s_log_add_message(is_critical, "%s의 공격! %s에게 [피해 %d%s], [강인도 피해 %d]",
        player->name, monster->name, damage, extra_damage_str, break_damage);
}

void log_monster_attack(player_t* player, monster_t* monster, int damage) {
    s_log_add_message(false, "%s이(가) %s에게 %d의 피해를 입혔습니다.", monster->name, player->name, damage);
}

void log_evaded(const char* defender_name, const char* attacker_name) {
    s_log_add_message(false, "%s이(가) %s의 공격을 회피했습니다!", defender_name, attacker_name);
}

void log_skill_used(player_t* player, int self_damage) {
    if (player->choice_hero == HERO_BERSERKER) {
        s_log_add_message(false, "%s이(가) 스킬을 사용했습니다!", player->name);
		s_log_add_message(false, "자신에게 %d의 피해를 입히고, 가하는 피해가 증가합니다.", self_damage);
	}
    else {
		s_log_add_message(false, "%s이(가) 스킬을 사용했습니다!", player->name);
    }
}

void log_life_steal(player_t* player, int heal_point) {
    s_log_add_message(false, "%s이(가) %d의 생명력을 흡수했습니다.", player->name, heal_point);
}

void log_monster_groggy(const char* monster_name) {
    s_log_add_message(false, "%s의 강인도가 모두 파괴되어 그로기 상태에 빠집니다!", monster_name);
}

void log_monster_stunned(const char* monster_name) {
    s_log_add_message(false, "%s은(는) 기절해서 움직일 수 없습니다.", monster_name);
}

void log_monster_recovers(const char* monster_name) {
    s_log_add_message(false, "%s이(가) 기절에서 회복하며 자세를 가다듬습니다.", monster_name);
}

void log_auto_heal(player_t* player, int heal_point)
{
    s_log_add_message(false, "%s이(가) %d 만큼 체력을 회복했습니다.", player->name, heal_point);
}

void log_drop_item(player_t* player, equipment_t item[][ITEM_COUNT], int rarity, int index)
{

    s_log_add_message(false, "%s이(가) %s을(를) 획득했습니다! (%C)", player->name, item[rarity][index].name, rarity_name[rarity]);
}

void log_drop_coin(player_t* player, int coin)
{
    s_log_add_message(false, "%s이(가) %d 코인을 획득했습니다.", player->name, coin);
}

void log_goto_store(void)
{
	s_log_add_message(false, "상점으로 이동합니다.");
}

void log_dead_effect_used(void)
{
    s_log_add_message(false, "[응징자의 가시] 세트 효과 발동");
    s_log_add_message(false, "최대 체력을 회복하고 공격력이 증가합니다.");
}

void log_damage_reduction_effect_used(void)
{
    s_log_add_message(false, "[광전사] 세트 효과 발동");
    s_log_add_message(false, "받는 피해가 30% 감소합니다.");
}