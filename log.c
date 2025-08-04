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
    s_log_add_message(is_critical, "%s의 반격! %s에게 [피해 %d%s]",
        player->name, monster->name, final_damage, extra_damage_str);
}

void log_player_attack(player_t* player, monster_t* monster, int damage, int break_damage, bool is_critical, int break_extra_damage_dealt) {
    char extra_damage_str[20] = "";
    if (break_extra_damage_dealt > 0) {
        snprintf(extra_damage_str, sizeof(extra_damage_str), " (+%d)", break_extra_damage_dealt);
    }

    // [수정] "[치명타!]" 텍스트 생성 로직 삭제
    s_log_add_message(is_critical, "%s의 공격! %s에게 [피해 %d%s]",
        player->name, monster->name, damage, extra_damage_str);
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
    else if (player->choice_hero == HERO_BREAKER) {
		s_log_add_message(false, "%s이(가) 스킬을 사용했습니다!", player->name);
        s_log_add_message(false, "강인도 피해가 대폭 증가합니다.");
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

void log_field_effect_on(void)
{
    s_log_add_message(false, "필드 효과가 발동됩니다.");
}

void log_field_effect_off(void)
{
    s_log_add_message(false, "필드 효과가 사라졌습니다.");
}

void log_field_effect_swap_action_value(void) 
{
	s_log_add_message(false, "플레이어와 몬스터의 행동 서열이 서로 바뀝니다.");
}

void log_field_effect_player_damage(player_t* player, int damage)
{
    s_log_add_message(false, "%s이(가) %d의 피해를 입었습니다.", player->name, damage);
}

void log_field_effect_monster_damage(monster_t* monster, int damage)
{
    s_log_add_message(false, "%s이(가) %d의 피해를 입었습니다.", monster, damage);
}

void log_filed_effect_swap_attack(void)
{
    s_log_add_message(false, "플레이어와 몬스터의 공격력이 서로 바뀝니다.");
}

void log_field_effect_zero_evasion(void)
{
    s_log_add_message(false, "1턴동안 플레이어와 몬스터의 회피율이 0으로 설정됩니다.");
}

void log_field_effect_attack_increase(void)
{
    s_log_add_message(false, "1턴동안 플레이어와 몬스터의 공격력이 증가합니다.");
}

void log_field_effect_heal(void)
{
    s_log_add_message(false, "플레이어와 몬스터가 체력을 회복합니다.");
}

void log_field_effect_blood_for_power(const char* name, int sum_attack)
{
    s_log_add_message(false, "플레이어와 몬스터의 체력을 감소시키고");
    s_log_add_message(false, "%s가 그 값 만큼 공격력을 증가시킵니다. (+%d)", name, sum_attack);
}

void log_field_effect_power_attack(const char* name)
{
    s_log_add_message(false, "%s이(가) 강력한 데미지를 받았습니다.", name);
}


void log_run(void)
{
    s_log_add_message(false, "[전장에서 도망쳤습니다..]");
    Sleep(500);
    s_log_add_message(false, "당신은 등을 돌렸습니다.");
    Sleep(500);
    s_log_add_message(false, "피로 물든 전장의 함성은 점점 멀어지고,");
    Sleep(500);
    s_log_add_message(false, "심장은 죄책감에 무겁게 뛰고 있었습니다.");
    Sleep(1000);

    s_log_add_message(false, "");
    s_log_add_message(false, "며칠 뒤, 성문 앞에 도착한 당신.");
    Sleep(500);
    s_log_add_message(false, "기적처럼 살아 돌아왔지만, 기다리고 있던건 왕의 분노");
    Sleep(1000);

    s_log_add_message(false, "");
    s_log_add_message(false, "\"패배보다 치욕은 더 큰 법이다.\"");
    Sleep(500);
    s_log_add_message(false, "너의 두 다리는 도망에 썻지만,");
    Sleep(500);
    s_log_add_message(false, "그 책임으로 목을 치게 될 것이다.\"");
    Sleep(1000);

    s_log_add_message(false, "");
    s_log_add_message(false, "죄수복을 입은 채 왕 앞에 무릎을 꿇은 당신");
    Sleep(500);
    s_log_add_message(false, "마지막으로 고개를 들어 왕을 바라보았지만, ");
    Sleep(500);
    s_log_add_message(false, "그의 눈은 얼음처럼 차가웠습니다.");
    Sleep(1000);

    s_log_add_message(false, "");
    s_log_add_message(false, "\"목을 쳐라\"");
    Sleep(500);
    s_log_add_message(false, "찬란했던 당신의 이야기는 여기서 끝이 났습니다.");
}

void log_legacy(void)
{
    // 전 영웅의 유산이 되살아납니다...
    s_log_add_message(false, "[전 영웅의 유산이 깨어납니다...]");
    Sleep(1000);

    // 무기고의 녹슨 검이 불빛을 받아 희미하게 빛나고,
    s_log_add_message(false, "어둠 속 무기고 한켠에서 녹슨 검이");
    Sleep(500);
    s_log_add_message(false, "불빛을 받아 희미하게 빛납니다.");
    Sleep(1200);

    // 거친 손때 묻은 손잡이에는 옛 전투의 목소리가 남아 있고,
    s_log_add_message(false, "거친 손때 묻은 손잡이에는");
    Sleep(500);
    s_log_add_message(false, "옛 전투의 목소리가 남아 있습니다.");
    Sleep(1200);

    // 마지막 한 닢의 코인은 마치 아직도 운명을 기다리는 듯 반짝입니다.
    s_log_add_message(false, "작게 남은 한 닢의 코인은 마치");
    Sleep(500);
    s_log_add_message(false, "다시 쓰일 날을 기다리듯 반짝입니다.");
    Sleep(1200);

    // 그리고 공허한 성벽 너머로 한결 가벼워진 발걸음이 다가옵니다.
    s_log_add_message(false, "공허한 성벽 너머로, ");
    Sleep(500);
    s_log_add_message(false, "이름 없는 이의 발걸음이 다가옵니다.");
    Sleep(1200);

    // “이제, 네 차례다.” 바람결에 실린 목소리가 속삭입니다.
    s_log_add_message(false, "\"이제 네 차례다.\"");
    Sleep(500);
    s_log_add_message(false, "바람결에 실린 목소리가 속삭입니다.");
    Sleep(1500);

    // 새로운 전설이, 당신의 손끝에서 다시 시작됩니다.
    s_log_add_message(false, "이제 새로운 전설이 당신의 손끝에서 다시 시작됩니다.");
    Sleep(1500);
}
void log_prologue(void)
{
    // 화면을 비우고, 타이틀을 잠시 보여줍니다.
    log_buffer_clear();
    s_log_add_message(true, "[제 1장: 잿빛 하늘 아래]");
    Sleep(2000);
    log_buffer_clear();

    // 평화로운 시절에 대한 묘사
    s_log_add_message(false, "오랜 평화가 잠든 왕국, ");
    Sleep(500);
    s_log_add_message(false, "그곳엔 늘 노랫소리가 울려 퍼졌습니다.");
    Sleep(1200);
    s_log_add_message(false, "황금빛 밀밭은 바람에 춤추고,");
    Sleep(500);
    s_log_add_message(false, "아이들의 웃음소리는 하늘에 닿았습니다.");
    Sleep(1200);
    s_log_add_message(false, "모든 것이 영원할 것만 같았던,");
    Sleep(500);
    s_log_add_message(false, "축복받은 나날들이었습니다.");
    Sleep(1500);
    s_log_add_message(false, "");

    // 용의 등장과 파괴
    s_log_add_message(false, "그러나...");
    Sleep(1000);
    s_log_add_message(false, "그 모든 것을 집어삼킬 검은 재앙이 하늘을 뒤덮었습니다.");
    Sleep(1200);
    s_log_add_message(false, "거대한 날갯짓 한번에 폭풍이 일고,");
    Sleep(500);
    s_log_add_message(false, "칠흑 같은 그림자가 대지를 물들였습니다.");
    Sleep(1500);
    s_log_add_message(false, "검은 용 '칼리번'.");
    Sleep(800);
    s_log_add_message(false, "그 이름은 곧 절망이 되었습니다.");
    Sleep(1500);
    s_log_add_message(false, "");

    s_log_add_message(false, "용의 숨결이 닿은 곳마다 생명은 재가 되어 흩어졌고,");
    Sleep(1200);
    s_log_add_message(false, "사람들의 노랫소리는 처절한 비명으로 바뀌었습니다.");
    Sleep(1200);
    s_log_add_message(false, "왕국의 자랑이던 백색의 성벽은");
    Sleep(500);
    s_log_add_message(false, "검게 그을린 상처만을 드러냈습니다.");
    Sleep(1800);
    s_log_add_message(false, "");

    // 왕의 부름과 영웅의 등장
    s_log_add_message(false, "모든 희망이 사라졌다고 믿었던 바로 그때,");
    Sleep(1200);
    s_log_add_message(false, "왕은 잿더미가 된 옥좌에서 마지막 힘을 다해 외쳤습니다.");
    Sleep(1500);
    s_log_add_message(false, "");
    s_log_add_message(false, "\"누구 없느냐! 이 끔찍한 재앙을 끝낼 자는 없는가!\"");
    Sleep(1500);
    s_log_add_message(false, "\"이 땅을 구할 용사, 이 백성을 구할 영웅은 없는가!\"");
    Sleep(2000);
    s_log_add_message(false, "");

    s_log_add_message(false, "그 간절한 부름에 응답하듯,");
    Sleep(1000);
    s_log_add_message(false, "한 이름없는 이가 굳게 닫힌 성문 앞에 섰습니다.");
    Sleep(1500);
    s_log_add_message(false, "운명인가, 혹은 저주인가.");
    Sleep(1000);
    s_log_add_message(false, "왕국의 마지막 희망이... 당신의 어깨에 놓였습니다.");
    Sleep(2000);
    s_log_add_message(false, "");

    // 임무 부여
    s_log_add_message(false, "\"영웅이여. 그대만이 우리의 유일한 빛이다.\"");
    Sleep(1500);
    s_log_add_message(false, "\"저 사악한 용을 향해 나아가라.");
    Sleep(500);
    s_log_add_message(false, "열두 개의 관문을 지나 용의 심장을 꿰뚫어라!\"");
    Sleep(1800);
    s_log_add_message(false, "\"부디... 이 왕국에 다시 평화를 되찾아다오!\"");
    Sleep(2500);
    s_log_add_message(false, "");

    s_log_add_message(false, "그렇게, 위대한 서사가 시작되었습니다.");
    Sleep(1200);
    s_log_add_message(false, "당신의 손으로...");
    Sleep(2000);
}


void log_chapter_2(void)
{
    // 4스테이지 클리어 후, 중반부 돌입
    log_buffer_clear();
    s_log_add_message(true, "[제 2장: 균열의 속삭임]");
    Sleep(2000);
    log_buffer_clear();

    // 변화하는 전장과 몬스터
    s_log_add_message(false, "네 개의 관문을 지나며, ");
    Sleep(500);
    s_log_add_message(false, "당신의 검은 무뎌질 틈이 없었습니다.");
    Sleep(1500);
    s_log_add_message(false, "하지만 무언가 변하기 시작했습니다.");
    Sleep(500);
    s_log_add_message(false, "용의 하수인들의 눈빛에서 증오가 아닌, ");
    Sleep(1500);
    s_log_add_message(false, "마치 깊은 고통에 몸부림치는 듯한 처절함이 느껴집니다.");
    Sleep(1800);
    s_log_add_message(false, "");

    // 획득한 장비에 대한 묘사
    s_log_add_message(false, "새로 얻은 강철 흉갑은 이상하리만치 차가우면서도, ");
    Sleep(500);
    s_log_add_message(false, "희미한 온기를 품고 있습니다.");
    Sleep(1800);
    s_log_add_message(false, "손에 쥔 검을 휘두를 때면, "); 
    Sleep(500);
    s_log_add_message(false, "마치 누군가의 애절한 비명이 바람을 가르는 듯합니다.");
    Sleep(1800);
    s_log_add_message(false, "");

    // 피어나는 의심
    s_log_add_message(false, "단순한 마물들의 소행이라 치부하기엔, ");
    Sleep(500);
    s_log_add_message(false, " 땅에 서린 슬픔이 너무나도 깊습니다.");
    Sleep(1800);
    s_log_add_message(false, "당신은 처음으로 칼자루를 쥔 손에 깃든 무게를 느낍니다.");
    Sleep(1500);
    s_log_add_message(false, "이 길의 끝에서 마주할 진실은,");
    Sleep(500);
    s_log_add_message(false, "과연 왕이 말한 그대로일까요 ? ");
    Sleep(2500);
}

void log_chapter_3(void)
{
    // 8스테이지 클리어 후, 후반부 돌입
    log_buffer_clear();
    s_log_add_message(true, "[제 3장: 잿빛 서약]");
    Sleep(2000);
    log_buffer_clear();

    // 용의 둥지에 가까워진 풍경
    s_log_add_message(false, "여덟 개의 관문이 당신의 등 뒤에서 닫혔습니다.");
    Sleep(1500);
    s_log_add_message(false, "이제 세상은 온통 잿빛입니다."); 
    Sleep(500);
    s_log_add_message(false, "살아 숨 쉬는 모든 것이 돌처럼 굳어가는 땅.");
    Sleep(1800);
    s_log_add_message(false, "몬스터들의 울음소리는 더 이상 위협이 아닌, ");
    Sleep(500);
    s_log_add_message(false, "구원을 바라는 기도처럼 들려옵니다.");
    Sleep(1800);
    s_log_add_message(false, "");

    // 유니크 아이템과의 공명
    s_log_add_message(false, "당신이 걸친 [갑옷]이 심장처럼 고동치기 시작합니다.");
    Sleep(1800);
    s_log_add_message(false, "머리에 쓴 왕관은");
    Sleep(500);
    s_log_add_message(false, "지울 수 없는 후회의 무게로 당신을 짓누르고, ");
    Sleep(1800);
    s_log_add_message(false, "손에 쥔 칼날은 베어야 할 상대를");
    Sleep(500);
    s_log_add_message(false, "향하며 조용히 울고 있습니다.");
    Sleep(2000);
    s_log_add_message(false, "");

    // 확신이 된 의심
    s_log_add_message(false, "이 장비들은 용을 죽이기 위한 것이 아닙니다.");
    Sleep(1500);
    s_log_add_message(false, "오히려... 용을 지키기 위해 만들어진 것처럼 느껴집니다.");
    Sleep(1800);
    s_log_add_message(false, "왕이 말한 '사악한 용'은 어디에도 없습니다.");
    Sleep(1500);
    s_log_add_message(false, "오직 거대한 슬픔만이 당신의 앞길을 가로막고 있을 뿐입니다.");
    Sleep(2500);
}

void log_chapter_4(void)
{
    // 최종 보스 클리어 후, 엔딩
    log_buffer_clear();
    s_log_add_message(true, "[최종장: 영원한 속죄]");
    Sleep(2500);
    log_buffer_clear();

    // 용의 소멸과 진실
    s_log_add_message(false, "마침내 당신의 칼날이 용의 심장을 꿰뚫었습니다.");
    Sleep(1500);
    s_log_add_message(false, "분노에 찬 포효 대신, 길고 긴 고통의 끝을 알리는");
    Sleep(500);
    s_log_add_message(false, "안도의 한숨이 터져 나옵니다.");
    Sleep(2000);
    s_log_add_message(false, "거대한 용의 형체가 빛에 휩싸이며 스러지는 대신,");
    Sleep(500);
    s_log_add_message(false, "점차 작아지기 시작합니다.");
    Sleep(2500);
    s_log_add_message(false, "그리고 당신의 눈앞에 남은 것은...");
    Sleep(500);
    s_log_add_message(false, "잿빛 피부를 가진 작은 소녀였습니다.");
    Sleep(2500);
    s_log_add_message(false, "");

    // 공주의 마지막 말
    s_log_add_message(false, "소녀가 재가 되어 사라지기 직전,");
    Sleep(500);
    s_log_add_message(false, "허공을 향해 힘없이 손을 뻗습니다.");
    Sleep(2000);
    s_log_add_message(false, "\"아... 어머니... 절 데리러 오셨군요...\"");
    Sleep(2500);
    s_log_add_message(false, "\"죄송해요... 더는... 역병을 붙잡아 둘 수가... 없어요...\"");
    Sleep(3000);
    s_log_add_message(false, "");

    // 세상의 멸망
    s_log_add_message(false, "소녀가 완전히 소멸하는 순간, 억눌려 있던 '잿빛 역병'이");
    Sleep(500);
    s_log_add_message(false, "검은 안개처럼 터져 나와 온 세상을 뒤덮습니다.");
    Sleep(2500);
    s_log_add_message(false, "하늘은 피처럼 붉게 물들고, ");
    Sleep(500);
    s_log_add_message(false, "당신의 발밑에서부터 대지가 빠르게 잿빛으로 굳어갑니다.");
    Sleep(2500);
    s_log_add_message(false, "");

    // 왕의 등장과 광기
    s_log_add_message(false, "그때, 텅 빈 옥좌에서 늙은 왕이 걸어 나와 웃으면서 웁니다.");
    Sleep(2000);
    s_log_add_message(false, "\"하하... 아아... 드디어...");
    Sleep(500);
    s_log_add_message(false, "내 딸이 저 끔찍한 고통에서 해방되었구나.\"");
    Sleep(2500);
    s_log_add_message(false, "\"영웅이여, 고맙다.");
    Sleep(500);
    s_log_add_message(false, "네 덕분에 우리 모두가 '구원'받게 되었어.\"");
    Sleep(3000);

    // 무한 모드의 시작
    s_log_add_message(false, "왕의 말이 끝나자마자, ");
    Sleep(500);
    s_log_add_message(false, "세상은 역병에 잠식된 괴물들로 가득 찹니다.");
    Sleep(2000);
    s_log_add_message(false, "당신이 저지른 일의 결과이자, 당신에게 내려진 벌입니다.");
    Sleep(2000);
    s_log_add_message(false, "당신이 멸망시킨 세상 속에서, 영원한 싸움이 시작됩니다.");
    Sleep(3000);
}

void log_monster_use_skill(monster_t* monster, int type)
{
    s_log_add_message(false, "%s이(가) 스킬을 사용했습니다.", monster->name);
    if (type == 0) {
        s_log_add_message(false, "%s의 공격력이 증가합니다.", monster->name);
    }
    else if (type == 1) {
        s_log_add_message(false, "%s이(가) 체력을 회복했습니다.", monster->name);
    }
    else if (type == 2) {
        s_log_add_message(false, "%s의 방어력이 증가합니다.", monster->name);
    }
}

void log_fianl_monster_use_skill(monster_t* monster)
{
    s_log_add_message(false, "%s이(가) 포효합니다.", monster->name);
}

void log_roar_damage(player_t* player, int damage)
{
    s_log_add_message(false, "%s의 체력이 %d감소합니다.", player->name, damage);
}

void log_final_monster_after_skill(monster_t* monster)
{
    s_log_add_message(false, "%s가 체력을 회복합니다.", monster->name);
}

void log_select_rest(void)
{
    s_log_add_message(false, "왕의 축복이 당신의 여정에 함께합니다.");
    Sleep(1000);
    s_log_add_message(false, "지친 몸을 추스리고 다시 나아가십시오.");
}

void log_select_store(void)
{
    s_log_add_message(false, "어디선가 나타난 기묘한 상점입니다.");
    Sleep(1000);
    s_log_add_message(false, "진열된 물건들에서 희미한 온기와 슬픔이 느껴집니다.");
}

void log_infinite_mode_start(void)
{
    log_buffer_clear();
    s_log_add_message(true, "[ ??? 장: 끝나지 않는 속죄 ]");
    Sleep(3000);
    log_buffer_clear();

    // 멸망한 세상에 대한 묘사
    s_log_add_message(false, "당신의 손으로 구원한 것은 공주가 아닌,");
    Sleep(500);
    s_log_add_message(false, "그녀의 고통뿐이었습니다.");
    Sleep(2000);
    s_log_add_message(false, "당신이 해방시킨 것은 왕국이 아닌,");
    Sleep(500);
    s_log_add_message(false, "세상을 잠식할 역병뿐이었습니다.");
    Sleep(2000);
    s_log_add_message(false, "이제 하늘에는 별 대신 핏빛 균열이 떠 있고,");
    Sleep(500);
    s_log_add_message(false, "땅에는 침묵만이 흐릅니다.");
    Sleep(2500);
    s_log_add_message(false, "");

    // 플레이어의 상황
    s_log_add_message(false, "당신은 영웅이 아닙니다.");
    Sleep(1800);
    s_log_add_message(false, "당신은 이 잿빛 묘지 위에 홀로 남겨진, 마지막 죄인입니다.");
    Sleep(2500);
    s_log_add_message(false, "");

    // 무한 모드의 본질
    s_log_add_message(false, "저 멀리서, 역병에 잠식된 것들이 울부짖으며 다가옵니다.");
    Sleep(2000);
    s_log_add_message(false, "끝도 없는 절망의 파도입니다.");
    Sleep(1800);
    s_log_add_message(false, "여기에는 더 이상 구원도, 영광도 없습니다.");
    Sleep(2000);
    s_log_add_message(false, "오직 당신이 저지른 과오의 무게만이");
    Sleep(500);
    s_log_add_message(false, "당신의 검을 통해 기록될 뿐입니다.");
    Sleep(2800);
    s_log_add_message(false, "");

    s_log_add_message(false, "얼마나 오래 버틸 수 있겠습니까?");
    Sleep(2000);
    s_log_add_message(false, "이 멸망한 세상에서... 당신의 고통을 증명해 보이십시오.");
    Sleep(3000);
}