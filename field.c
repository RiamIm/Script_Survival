// field.c
#define _CRT_SECURE_NO_WARNINGS
#include "field.h"
#include "utils.h"

#include "log.h"

static int player_evasion = 0;
static int monster_evasion = 0;

static int player_add_attack = 200;
static int monster_add_attack = 100;

static bool is_add_attack_player = true;
static int sum_attack = 0;

void field_effect_on(player_t* player, monster_t* monster, int type)
{
	log_field_effect_on();

	if (type == 0) { // 행동서열 바꾸기
		utils_swap_double(&player->action_value, &monster->action_value);
		log_field_effect_swap_action_value();
	}
	else if (type == 1) { // 모든 생명체 현재 체력 대미지
		int player_damage = get_player_hp(player) / 10;
		int monster_damage = monster->current_hp / 10;
		set_player_hp(player, player_damage);
		monster->current_hp -= monster_damage;
		log_field_effect_player_damage(player, player_damage);
		log_field_effect_monster_damage(monster, monster_damage);
	}
	else if (type == 2) { // 1턴 동안 서로 회피 못함
		player_evasion = (int)(get_player_evasion_rate(player) * 100); // 현재 회피율 저장
		monster_evasion = (int)(monster->evasion_rate * 100); // 현재 회피율 저장
		set_player_evasion_rate(player, 0.0, 1); // 회피율 0으로 설정
		monster->evasion_rate = 0.0; // 회피율 0으로 설정
		log_field_effect_zero_evasion();
	}
	else if (type == 3) { // 1턴 동안 서로 공격력 증가
		set_player_attack(player, player_add_attack, 1); // 플레이어 공격력 증가
		monster->attack += monster_add_attack; // 몬스터 공격력 증가
		log_field_effect_attack_increase();
	}
	else if (type == 4) { // 광역힐
		set_player_hp(player, (int)(get_player_max_hp(player) * 0.1)); // 플레이어 체력 회복
		monster->current_hp += monster->max_hp / 10; // 몬스터 체력 회복
		if (monster->current_hp > monster->max_hp) {
			monster->current_hp = monster->max_hp; // 최대 체력 초과 방지
		}

		log_field_effect_heal();
	}
	else if (type == 5) { // 서로 피를 깎고 그 값만큼 다음 행동 캐릭터의 공격력 증가
		sum_attack += get_player_hp(player) / 5;
		set_player_hp(player, get_player_hp(player) / 5); // 플레이어 체력 감소
		sum_attack += monster->current_hp / 5;
		monster->current_hp -= monster->current_hp / 5; // 몬스터 체력 감소

		if(player->action_value >= monster->action_value) {
			set_player_attack(player, sum_attack, 1); // 플레이어 공격력 증가
			is_add_attack_player = true; // 플레이어가 공격력 증가
			log_field_effect_blood_for_power(player->name, sum_attack);
		}
		else {
			monster->attack += sum_attack; // 몬스터 공격력 증가
			is_add_attack_player = false; // 몬스터가 공격력 증가
			log_field_effect_blood_for_power(monster->name, sum_attack);
		}
	}
	else if (type == 6) { // 체력 비율이 높은 캐릭터에게 강력한 피해 
		double player_per = (double)get_player_hp(player) / get_player_max_hp(player);
		double monster_per = (double)monster->current_hp / monster->max_hp;
		
		if (player_per <= monster_per)
		{
			monster->current_hp -= monster->max_hp / 5; // 몬스터에게 강력한 피해
			log_field_effect_power_attack(monster->name);
		}
		else
		{
			set_player_hp(player, get_player_hp(player) / 5); // 플레이어에게 강력한 피해
			log_field_effect_power_attack(player->name);
		}
	}
}

void field_effect_off(player_t* player, monster_t* monster, int type)
{
	if (type == 0) { // 행동서열 바꾸기
		return;
	}
	else if (type == 1) { // 모든 생명체 현재 체력 대미지
		return; // 이 효과는 지속적이지 않으므로 해제할 필요 없음
	}
	else if (type == 2) { // 1턴 동안 서로 회피 못함
		set_player_evasion_rate(player, player_evasion / 100.0, 1); // 회피율 복원
		monster->evasion_rate = monster_evasion / 100.0; // 회피율 복원
	}
	else if(type == 3) { // 1턴 동안 서로 공격력 증가
		set_player_attack(player, -player_add_attack, 1); // 플레이어 공격력 복원
		monster->attack -= monster_add_attack; // 몬스터 공격력 복원
	}
	else if (type == 4) { // 광역힐
		return; // 이 효과는 지속적이지 않으므로 해제할 필요 없음
	}
	else if (type == 5) { // 서로 피를 깎고 그 값만큼 다음 행동 캐릭터의 공격력 증가
		if(is_add_attack_player) {
			set_player_attack(player, -sum_attack, 1); // 플레이어 공격력 복원
		}
		else {
			monster->attack -= sum_attack; // 몬스터 공격력 복원
		}
		sum_attack = 0; // 합계 초기화
	}
	else if(type == 6) { 
		return; // 이 효과는 지속적이지 않으므로 해제할 필요 없음
	}

	log_field_effect_off();
}