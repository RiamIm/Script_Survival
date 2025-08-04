// utils.c
#define _CRT_SECURE_NO_WARNINGS
#include "utils.h"
#include "UI_info.h"
#include "inout.h"

//#include <imm.h>
//#pragma comment(lib, "imm32.lib") 

static bool s_sound_enabled = true;
static int  s_sound_volume = 50;

int utils_min(int x, int y)
{
    return (x < y) ? x : y;
}

int utils_max(int x, int y)
{
    return (x > y) ? x : y;
}

void utils_gotoxy(int x, int y)
{
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void utils_hide_cursor(void)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void utils_set_color(int new_color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), new_color);
}

void utils_swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void utils_swap_double(double* a, double* b)
{
    double temp = *a;
    *a = *b;
    *b = temp;
}

void utils_set_volume_from_percentage(int percent)
{
	if (percent < 0) percent = 0;
	if (percent > 100) percent = 100;

    // percent를 0~65535 범위로 변환
    DWORD volume = (DWORD)((percent * 65535 / 100) & 0xFFFF);
    volume |= (volume << 16); // 좌우 채널 동일하게

    waveOutSetVolume(0, volume);
}

int utils_getch(void)
{
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

    int key = _getch();
    if (key == EXTENDED_KEY) key = _getch();
    while (_kbhit()) _getch();
    return key;
}

void utils_clear_input_buffer(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        // discard remaining characters
    }
    while (_kbhit()) _getch();
}

void utils_sound_init(void) {
    // 초기 볼륨 적용
    utils_set_volume_from_percentage(s_sound_volume);
}

void utils_sound_set_enabled(bool enabled) {
    s_sound_enabled = enabled;
    if (!enabled) {
        // 재생 중인 모든 사운드 중지
        PlaySound(NULL, NULL, 0);
    }
}

bool utils_sound_is_enabled(void) {
    return s_sound_enabled;
}

void utils_sound_play(const wchar_t* filepath) {
    if (!s_sound_enabled) return;
    PlaySound(filepath, NULL, SND_ASYNC | SND_FILENAME);
}