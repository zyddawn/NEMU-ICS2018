#ifndef __CACHE_H__
#define __CACHE_H__

#include "nemu.h"

#define BLOCK_SIZE_B 64
#define CACHE_LINES 1024
#define SET_SIZE 8
#define TAG_BITS 14
#define SET_INDEX_BITS 7
#define BLOCK_INDEX_BITS 6

// physics addr 27 bits in total
//  14 (tag) + 7 (set index) + 6 (block index)

typedef struct {
	bool valid_bit;
	uint32_t tag;
	uint8_t block_data[BLOCK_SIZE_B];
} CacheLine;

// 8-way set associative
extern CacheLine cache[][SET_SIZE];



