// #ifndef __CACHE_H__
// #define __CACHE_H__

#include "nemu.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


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
extern CacheLine L1_dcache[][SET_SIZE];


void init_cache(CacheLine cache[][SET_SIZE]);
uint32_t cache_read(paddr_t paddr, size_t len, CacheLine cache[][SET_SIZE]);
void cache_write(paddr_t paddr, size_t len, uint32_t data, CacheLine cache[][SET_SIZE]);

