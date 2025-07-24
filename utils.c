// utils.c
#define _CRT_SECURE_NO_WARNINGS
#include "utils.h"

//#include <imm.h>
//#pragma comment(lib, "imm32.lib") 

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

//void utils_force_disable_ime(void) {
//    HWND hWnd = GetConsoleWindow();
//    HIMC hIMC = ImmGetContext(hWnd);
//    ImmSetOpenStatus(hIMC, FALSE); // IME를 닫음 (한글 입력 종료)
//    ImmReleaseContext(hWnd, hIMC);
//}

//void utils_force_english_input(void) 
//{
//    HWND hWnd = GetConsoleWindow();
//    SetForegroundWindow(hWnd); // 콘솔에 포커스 줘야 활성화됨
//    HKL hkl = LoadKeyboardLayout("00000409", KLF_ACTIVATE);
//    ActivateKeyboardLayout(hkl, KLF_SETFORPROCESS);
//}