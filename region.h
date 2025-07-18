// region.h
#pragma once

typedef enum region {
	REGION_FOREST,
	REGION_SNOW,
	REGION_DESERT,
	// REGION_NULL, // 지역이 없는 경우 (는 필요한지 모르겠어서 일단 주석 처리했습니다.)
} region_t;

#define REGION_COUNT 3