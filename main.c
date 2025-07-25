// main.c 
#define _CRT_SECURE_NO_WARNINGS
#include "utils.h"
#include "game_manager.h"

int main(void)
{
    // 커서 지우기
    utils_hide_cursor();

    // 랜덤 시드 및 콘솔 크기 설정
    srand((unsigned int)time(NULL));
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT windowSize = { 0, 0, 176, 30 };
    SetConsoleWindowInfo(hOut, TRUE, &windowSize);
    COORD bufferSize = { 177, 300 };
    SetConsoleScreenBufferSize(hOut, bufferSize);

    GameManager_Run();

    return 0;
}