#define _CRT_SECURE_NO_WARNINGS
#include "monster.h"
#include "inventory.h"
#include "item.h"
#include "log.h"

#define MAX_STATS  100
#define DROP_STEP  1000

#define MAX_STAGE        12

#define NORMAL_BASE      0.60    // 기본 60%
#define NORMAL_MAX       1.00    // 최대 100%

#define RARE_BASE        0.1    // 기본 10%
#define RARE_MAX         0.90    // 최대 90%

#define EPIC_BASE        0.005   // 기본 0.5%
#define EPIC_MAX         0.50    // 최대 50%

#define UNIQUE_BASE      0.001   // 기본 0.1%
#define UNIQUE_MAX       0.10    // 최대 10%

// 내부 스탯 저장소
static monster_t stats_pool[MAX_STATS];
static int       stats_count = 0;

static inline int curved_money_drop(int stage, double exponent)
{
    // 1) 기존 선형 min/max 계산
    int min = stage * DROP_STEP;
    int max = (stage + 1) * DROP_STEP;

    // 2) 0.0~1.0 균일 랜덤
    double u = genrand_real1();

    // 3) 곡선 변형: ease‑out 예) exponent=0.5
    double v = pow(u, exponent);

    // 4) 곡선 분포값을 [min, max]에 매핑
    return min + (int)((max - min) * v);
}

static inline double ease_in(double frac, double exp) {
    return pow(frac, exp);
}

// ease‑out: 초반에 빠르게 증가, 후반에 천천히 증가
static inline double ease_out(double frac, double exp) {
    return 1.0 - pow(1.0 - frac, exp);
}

static inline void try_drop_equipment(player_t* player,
                                double threshold,
                                int rarity,
                                int item_count)
{
    if (genrand_real1() >= threshold)
        return;

    int equipment_type = (int)(genrand_int32() & 1); // 0=weapon, 1=armor
    int idx = genrand_int32() % item_count;

    if (equipment_type == 0) {
        weapon_inventory[rarity][idx].is_was_having = TRUE;
        weapon_inventory[rarity][idx].count++;
        log_drop_item(player, weapons, rarity, idx);
    }
    else {
        armor_inventory[rarity][idx].is_was_having = TRUE;
        armor_inventory[rarity][idx].count++;
        log_drop_item(player, armors, rarity, idx);
    }
}

static void pad_image_lines(wchar_t* image[], int line_count) {
    // 1) 가장 긴 줄 길이 찾기
    int max_len = 0;
    for (int i = 0; i < line_count; i++) {
        if (!image[i]) break;
        int len = (int)wcslen(image[i]);
        if (len > max_len) max_len = len;
    }

    // 2) 짧은 줄은 realloc + 공백 채우기
    for (int i = 0; i < line_count; i++) {
        if (!image[i]) break;
        int len = (int)wcslen(image[i]);
        if (len < max_len) {
            // 메모리 확장
            wchar_t* buf = realloc(
                image[i],
                (max_len + 1) * sizeof(wchar_t)
            );
            if (!buf) continue; // realloc 실패 시 건너뛰기
            image[i] = buf;
            // 뒤쪽을 공백으로 채우고 널 종료
            for (int j = len; j < max_len; j++) {
                image[i][j] = L' ';
            }
            image[i][max_len] = L'\0';
        }
    }
}


// monsters.csv 를 한 번만 읽어서 stats_pool 에 저장
static void load_stats_csv(void) {
    if (stats_count > 0) return; // 이미 로드됨

    FILE* fp = fopen("data/monster_stat.csv", "r");
    if (!fp) { perror("CSV 열기 실패"); return; }

    char line[512];
    fgets(line, sizeof(line), fp); // header

    while (fgets(line, sizeof(line), fp) && stats_count < MAX_STATS) {
        line[strcspn(line, "\r\n")] = '\0';
        char* tok = strtok(line, ",");
        if (!tok) break;

        monster_t* m = &stats_pool[stats_count];
        // name
        strncpy(m->name, tok, sizeof(m->name) - 1);
        m->name[sizeof(m->name) - 1] = '\0';
        // attack
        tok = strtok(NULL, ","); m->attack = tok ? atoi(tok) : 0;
        // max_hp
        tok = strtok(NULL, ","); m->max_hp = tok ? atoi(tok) : 0;
        // toughness
        tok = strtok(NULL, ","); m->max_toughness = tok ? atoi(tok) : 0;
        // speed
        tok = strtok(NULL, ","); m->speed = tok ? atoi(tok) : 0;
        // evasion
        tok = strtok(NULL, ","); m->evasion_rate = tok ? atof(tok) : 0.0;
        // defence
        tok = strtok(NULL, ",\n"); m->defence_rate = tok ? atof(tok) : 0.0;

        // 연관 필드 초기화
        m->current_hp = m->max_hp;
        m->current_toughness = m->max_toughness;
        m->action_value = 0.0;
        m->is_groggy = false;
        m->stun_turns = 0;

        stats_count++;
    }
    fclose(fp);
}

// change.log 에서 monster_index 블록을 와이드로 읽어 m->image[] 에 저장
static bool load_image_log(monster_t* m, int monster_index) {
    // UTF-8 → wchar 로 직접 읽기
    FILE* fp = _wfopen(L"data/monster_image.log", L"r, ccs=UTF-8");
    if (!fp) { fwprintf(stderr, L"log 파일 열기 실패\n"); return false; }

    setlocale(LC_ALL, "");  // 멀티바이트 함수들이 작동하도록

    wchar_t buf[512];
    bool    in_block = false;
    int     saved = 0;
    int     curr = 0;

    // 초기화
    for (int i = 0; i < MAX_IMAGE_LINES; i++)
        m->image[i] = NULL;

    while (fgetws(buf, 512, fp)) {
        // 개행 제거
        size_t len = wcslen(buf);
        while (len > 0 && (buf[len - 1] == L'\r' || buf[len - 1] == L'\n'))
            buf[--len] = L'\0';

        // 첫 블록 첫 줄 BOM 제거
        if (curr == 0 && saved == 0 && len > 0 && buf[0] == 0xFEFF) {
            memmove(buf, buf + 1, len * sizeof(wchar_t));
            len--;
        }

        for (size_t i = 0; i < len; i++) {
            if (buf[i] == L'\t') buf[i] = L' ';
        }


        // 공백/탭만 있는 진짜 빈 줄인지
        bool is_blank = true;
        for (size_t i = 0; i < len; i++) {
            if (!iswspace(buf[i])) { is_blank = false; break; }
        }

        if (is_blank) {
            if (in_block) {
                if (curr == monster_index) {
                    // 이 블록 끝
                    pad_image_lines(m->image, saved);
                    fclose(fp);
                    return (saved > 0);
                }
                curr++;
                saved = 0;
                in_block = false;
            }
            continue;
        }

        // 비어있지 않은 줄
        in_block = true;
        if (curr == monster_index && saved < MAX_IMAGE_LINES) {
            // 메모리 할당 + 복사
            wchar_t* line = malloc((len + 1) * sizeof(wchar_t));
            wcscpy(line, buf);
            m->image[saved++] = line;
        }
    }

    fclose(fp);
    return false;
}

bool monster_init(monster_t* m, int monster_index) {
    // 1) stats 한 번만 로드
    load_stats_csv();
    if (monster_index < 0 || monster_index >= stats_count) {
        fprintf(stderr, "잘못된 몬스터 인덱스: %d\n", monster_index);
        return false;
    }
    // 2) stats 복사
    *m = stats_pool[monster_index];
    // 3) image 로드
    return load_image_log(m, monster_index);

    m->used_skill = false;
}

void monster_skill(monster_t* monster, int type) {

    if (type == 0) {
        monster->attack *= 1.2;
    }
    else if (type == 1) {
        monster->current_hp += monster->max_hp / 20;
    }
    else if (type == 2) {
        monster->defence_rate += 0.05;
    }

    log_monster_use_skill(monster, type);
}

void final_monster_skill(monster_t* monster, player_t* player, int damage)
{
    player->current_hp -= damage;
    log_roar_damage(player, damage);
}

void monster_item_drop(player_t* player, int index)
{
    //if (index < 0 || index >= MAX_STAGE) return;

    //double frac = (double)index / (MAX_STAGE - 1);

    //// 선형 보간으로 각 등급별 threshold 계산
    //double th_normal = NORMAL_BASE + (NORMAL_MAX - NORMAL_BASE) * frac;
    //double th_rare = RARE_BASE + (RARE_MAX - RARE_BASE) * frac;
    //double th_epic = EPIC_BASE + (EPIC_MAX - EPIC_BASE) * frac;
    //double th_unique = UNIQUE_BASE + (UNIQUE_MAX - UNIQUE_BASE) * frac;

    if (index < 0 || index >= MAX_STAGE) return;

    double frac = (double)index / (MAX_STAGE - 1);

    // exponent 값으로 곡선의 가파름 조정 (값이 클수록 더 꺾임)
    const double norm_exp = 2.0;
    const double rare_exp = 2.5;
    const double epic_exp = 3.0;
    const double unique_exp = 4.0;

    // ease‑in 커브 적용 예
    double f_norm = ease_out(frac, norm_exp);
    double f_rare = ease_out(frac, rare_exp);
    double f_epic = ease_out(frac, epic_exp);
    double f_unique = ease_out(frac, unique_exp);

    // 혹은 ease‑out 사용
    // double f_norm   = ease_out(frac, norm_exp);
    // …

    // 곡선 보간 공식: BASE + (MAX - BASE) * f_*
    double th_normal = NORMAL_BASE + (NORMAL_MAX - NORMAL_BASE) * f_norm;
    double th_rare = RARE_BASE + (RARE_MAX - RARE_BASE) * f_rare;
    double th_epic = EPIC_BASE + (EPIC_MAX - EPIC_BASE) * f_epic;
    double th_unique = UNIQUE_BASE + (UNIQUE_MAX - UNIQUE_BASE) * f_unique;

    // 아이템 드랍 시도
    try_drop_equipment(player, th_normal, RARITY_NORMAL, NORMAL_ITEM_COUNT);
    try_drop_equipment(player, th_rare, RARITY_RARE, RARE_ITEM_COUNT);
    try_drop_equipment(player, th_epic, RARITY_EPIC, EPIC_ITEM_COUNT);
    try_drop_equipment(player, th_unique, RARITY_UNIQUE, UNIQUE_ITEM_COUNT);

    const double money_exp = 0.5;

    int money = curved_money_drop(index, money_exp);
    player->coin += money;
    log_drop_coin(player, money);
}