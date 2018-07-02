#ifndef __CACHE_H__
#define __CACHE_H__

#include "nemu.h"

#define BLOCK_SIZE 64
#define CACHE_LINES 1024
// physics addr 32 bits in total
// 16 (tag) + 10 (line index) + 6 (block index)

typedef struct {
	bool valid_bit;
	uint32_t tag;
	uint8_t block_data[BLOCK_SIZE];
} BLOCK;

// 8-way set associative
extern BLOCK cache[CACHE_LINES/8][8];



