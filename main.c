// main.c 
#define _CRT_SECURE_NO_WARNINGS
#include "utils.h"
#include "game_manager.h"

int main(void)
{
    // 커서 지우기
    utils_hide_cursor();

    // 랜덤 시드 및 콘솔 크기 설정
    struct timespec ts;
    /* TIME_UTC 플래그로 현재 시각(초+나노초)을 얻어옵니다 */
    if (timespec_get(&ts, TIME_UTC) == 0) {
        fprintf(stderr, "timespec_get 실패\n");
        return 1;
    }
    uint32_t seed = (uint32_t)(ts.tv_nsec ^ ts.tv_sec);

    init_genrand(seed);

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT windowSize = { 0, 0, 176, 30 };
    SetConsoleWindowInfo(hOut, TRUE, &windowSize);
    COORD bufferSize = { 177, 300 };
    SetConsoleScreenBufferSize(hOut, bufferSize);

    GameManager_Run();

    return 0;
}