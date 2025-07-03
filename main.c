// main.c
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

#include "pair.h"
#include "battle.h"
#include "battle_eval.h"
#include "player.h"
#include "monster.h"
#include "item.h"
#include "UI.h"

#define ESC 27

// void debugging_print_status(const Player_t* player, const Monster_t* monster, int turn);
// void debugging_print_battle_evaluation(const battle_eval_result_t* result); 

int main() {
	srand((unsigned int)time(NULL));  // 랜덤 시드 초기화
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT windowSize = { 0, 0, 176, 30 };
    SetConsoleWindowInfo(hOut, TRUE, &windowSize);

    COORD bufferSize = { 177, 300 };
    SetConsoleScreenBufferSize(hOut, bufferSize);

    int currentStage = 1;

    player_t player;
    init_player(&player, "Name");

    monster_t monster;
	init_monster(&monster, currentStage);  

    while (1) {
        DrawUI(currentStage, &player, &monster, "NULL");

        char input = _getch();  // 사용자 입력
        if (input == 'q') {
            break;
        }

        if (input == 'f')
        {
			battle_result_t result = process_battle_turn(&player, &monster); 
            if (result == BATTLE_RESULT_PLAYER_WIN) {
				DrawUI(currentStage, &player, &monster, "Player wins!");
	            // next monster
                currentStage++;
				init_monster(&monster, currentStage);
            }
			else if (result == BATTLE_RESULT_MONSTER_WIN) {
				DrawUI(currentStage, &player, &monster, "Monster wins!");
			}
            else if (result == EVASION_PLAYER_MONSTER) {
				DrawUI(currentStage, &player, &monster, "Player and monster evaded!");
            }
            else if (result == EVASION_PLAYER) {
				DrawUI(currentStage, &player, &monster, "Player evaded the attack!");
            }
            else if (result == EVASION_MONSTER) {
				DrawUI(currentStage, &player, &monster, "Monster evaded the attack!");
            }
            else {
				DrawUI(currentStage, &player, &monster, "Battle ongoing...");
			}
            Sleep(1000);
        }
    }

    return 0;
}