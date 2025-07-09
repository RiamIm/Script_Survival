// region.h
#pragma once
#include "inout.h"
#include "utils.h"

typedef enum region {
	REGION_FOREST,
	REGION_SNOW,
	REGION_DESERT,
	REGION_NULL, // 지역이 없는 경우
	REGION_COUNT
} region_t;