#pragma once

typedef struct player {
	char name[125];
	int attack;
	int max_hp;
	int current_hp;
	int speed;

	float evasion_rate;
	float defence_rate;

	int coin;

	int forest_mastery;
	int swamp_mastery;
	int desert_mastery;
} player_t;

void init_player(player_t* player, char* name);