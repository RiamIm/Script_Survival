#pragma once

#include <windows.h>
#include <stdio.h>
#include "item.h"
#include "player.h"
#include "monster.h"
#include "pair.h"
#include "utils.h"
#include "UI_controller.h"

#define WIDTH 151
#define HEIGHT 27

static void s_draw_title_selection(int title_state);

void clean_all_display(void);
void static_draw_main_box(void);
void static_draw_title(void);
void static_draw_battle_box(void); 
void static_draw_inventory_box(void);