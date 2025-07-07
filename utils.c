#include <windows.h>
#include <imm.h>
#include <stdbool.h>

#include "utils.h"

#pragma comment(lib, "imm32.lib")

void gotoxy(int x, int y)
{
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void hide_cursor(void)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void set_color(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void force_disable_ime(void) {
    HWND hWnd = GetConsoleWindow();
    HIMC hIMC = ImmGetContext(hWnd);
    ImmSetOpenStatus(hIMC, FALSE); // IME를 닫음 (한글 입력 종료)
    ImmReleaseContext(hWnd, hIMC);
}

void force_english_input(void) 
{
    HWND hWnd = GetConsoleWindow();
    SetForegroundWindow(hWnd); // 콘솔에 포커스 줘야 활성화됨
    HKL hkl = LoadKeyboardLayout("00000409", KLF_ACTIVATE);
    ActivateKeyboardLayout(hkl, KLF_SETFORPROCESS);
}