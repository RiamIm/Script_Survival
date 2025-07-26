#pragma once

#include <stdbool.h>

#include "inout.h"

#include "game_manager.h"

typedef struct save_slot_info_t {
	bool exists;
	char timestamp[20]; // YYYY-MM-DD HH:MM:SS Çü½Ä
} save_slot_info_t;

void load_save_slot_info(save_slot_info_t* slots);

void save_slot(int slot, const game_context_t* contextr);

bool load_slot(int slot, game_context_t* context);
