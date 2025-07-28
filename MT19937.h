#pragma once

#include <stdio.h>
#include <stdint.h> // C99 표준 헤더

#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

void init_genrand(uint32_t s);

uint32_t genrand_int32(void);

double genrand_real1(void);
