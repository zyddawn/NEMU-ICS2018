#ifndef __CACHE_H__
#define __CACHE_H__

#include "nemu.h"

#define BLOCK_SIZE_B 64
#define CACHE_LINES 1024
#define TAG_BITS 19
#define SET_INDEX_BITS 7
#define BLOCK_INDEX_BITS 6

// physics addr 32 bits in total
//  19 (tag) + 7 (set index) + 6 (block index)

typedef struct {
	bool valid_bit;
	uint32_t tag;
	uint8_t block_data[BLOCK_SIZE_B];
} BLOCK;

// 8-way set associative
extern BLOCK cache[][8];



