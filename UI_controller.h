#pragma once

#include <stdbool.h>

typedef enum UI_state {
	UI_STATE_TITLE,
	UI_STATE_SETTING,
	UI_STATE_BATTLE,
	UI_STATE_INVENTORY,
	UI_STATE_STORE
};

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77