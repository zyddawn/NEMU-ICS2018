#include "memory/cache.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define DEBUG

CacheLine L1_dcache[CACHE_LINES/SET_SIZE][SET_SIZE];

static void init_rand() {
	srand((unsigned)time(NULL));
}

static uint32_t get_tag(paddr_t paddr) {
	uint32_t res = (uint32_t) paddr;
	return (res >> (SET_INDEX_BITS + BLOCK_INDEX_BITS));
}

static uint32_t get_set_index(paddr_t paddr) {
	uint32_t res = (uint32_t) paddr;
	return ((res >> BLOCK_INDEX_BITS) & 0x7F);
}

static uint32_t get_block_index(paddr_t paddr) {
	uint32_t res = (uint32_t) paddr;
	return (res & 0x3F);
}

static uint32_t move_to_cache(CacheLine cache[][SET_SIZE], paddr_t cur_addr) {
	// randomly replace
	uint32_t replace_id = (uint32_t) rand() % SET_SIZE;
#ifdef DEBUG
	printf("replace_id = %d\n", replace_id);
#endif
	uint32_t set_index = get_set_index(cur_addr),
		 new_tag = get_tag(cur_addr);
	cache[set_index][replace_id].valid_bit = 1;
	cache[set_index][replace_id].tag = new_tag;
	// copy from the start of the block
	paddr_t cur_block_start = (new_tag << (SET_INDEX_BITS + BLOCK_INDEX_BITS)) | (set_index << BLOCK_INDEX_BITS);
	for(uint32_t block_index=0; block_index<BLOCK_SIZE_B; block_index+=4) {
		// in order to call hw_mem_read(), emulate real process
		uint32_t data = hw_mem_read((paddr_t)(cur_block_start+block_index), 4);
		memcpy(cache[set_index][replace_id].block_data, &data, 4);
	}
	// memcpy(cache[set_index][replace_id].block_data, hw_mem + cur_block_start, BLOCK_SIZE_B);
	return replace_id;
}


static bool found_in_cache(CacheLine cache[][SET_SIZE], uint32_t tag, uint32_t set_index, uint32_t inner_set_index) {
#ifdef DEBUG
	printf("want tag = 0x%x, cache tag = 0x%x, valid_bit = %d\n", tag, cache[set_index][inner_set_index].tag, cache[set_index][inner_set_index].valid_bit);
#endif
	return cache[set_index][inner_set_index].valid_bit && cache[set_index][inner_set_index].tag == tag;
}


static uint32_t update_res(CacheLine cache[][SET_SIZE], uint32_t res, uint32_t set_index, uint32_t inner_set_index, uint32_t block_index) {
	return (res << 8) | cache[set_index][inner_set_index].block_data[block_index];
}


uint32_t cache_read(paddr_t paddr, size_t len, CacheLine cache[][SET_SIZE]) {
	printf("CACHE READ...\n");
	if (len == 0)
		return 0;

	bool cache_hit = false;
	uint32_t res = 0, data_tag, data_set_index, data_block_index,
		 inner_set_index = 0, prev_set_index = -1;  // to judge if in the same block
	
	// len <= 4
	for(paddr_t cur_addr = paddr+len-1; cur_addr>=paddr; --cur_addr) {
		cache_hit = false;
		data_tag = get_tag(cur_addr);
		data_set_index = get_set_index(cur_addr);
		data_block_index = get_block_index(cur_addr);
		
		printf("cur_addr = 0x%x, tag = 0x%x, set = 0x%x, block = 0x%x\n", cur_addr, data_tag, data_set_index, data_block_index);

		if (data_tag > 0x4000 || data_set_index > 128 || data_block_index > 64) {
			printf("Parsing error! Wrong paddr.\n");
			return 0;
		}

		// if in the same block as before, no need to match again
		if(data_set_index == prev_set_index) {
			cache_hit = true;
			res = update_res(cache, res, data_set_index, inner_set_index, data_block_index);
			continue;
		}
		
		for(inner_set_index=0; inner_set_index<SET_SIZE; ++inner_set_index) {
			// found data in cache
			if (found_in_cache(cache, data_tag, data_set_index, inner_set_index)) {
				printf("HIT IN CACHE!!\n\n");
				cache_hit = true;	
				res = update_res(cache, res, data_set_index, inner_set_index, data_block_index);
				break;
			}
		}
		if (!cache_hit) {   // have to move from memory
			printf("Didn't hit in cache.\n");
			inner_set_index = move_to_cache(cache, cur_addr);
			res = update_res(cache, res, data_set_index, inner_set_index, data_block_index);
		}
	}
	return res;
}

static void update_cache(CacheLine cache[][SET_SIZE], uint32_t set_index, uint32_t inner_set_index, uint32_t block_index, uint32_t *cur_data) {
	cache[set_index][inner_set_index].block_data[block_index] = ((*cur_data) & 0xFF);
	*cur_data >>= 8;
}


void cache_write(paddr_t paddr, size_t len, uint32_t data, CacheLine cache[][SET_SIZE]) {
	// write to cache (if hit)
	if (len == 0)
		return ;
	bool cache_hit = false;
	uint32_t data_tag, data_set_index, data_block_index,
		 inner_set_index = 0, prev_set_index = -1, cur_data = data;  // to judge if in the same block
	paddr_t cur_addr = paddr;
	for(uint32_t i=0; i<len; ++i) {
		cur_addr = paddr + i;
		data_tag = get_tag(cur_addr);
		data_set_index = get_set_index(cur_addr);
		data_block_index = get_block_index(cur_addr);
		
		if (data_tag > 0x4000 || data_set_index > 128 || data_block_index > 64) {
			printf("Parsing error! Wrong paddr.\n");
			return ;
		}

		// if in the same block as before, no need to match again
		if (data_set_index == prev_set_index) {
		       	if (cache_hit) {
				update_cache(cache, data_set_index, inner_set_index, data_block_index, &cur_data);
			}
		}
		else {
			for(inner_set_index=0; inner_set_index<SET_SIZE; ++inner_set_index) {
				// found data in cache
				if (found_in_cache(cache, data_tag, data_set_index, inner_set_index)) {
					cache_hit = true;
					update_cache(cache, data_set_index, inner_set_index, data_block_index, &cur_data);
					break;
				}
			}
			if(inner_set_index == SET_SIZE)
				cache_hit = false;
		}
	}

	// write to memory
	hw_mem_write(paddr, len, data);
}


// set all valid bit to be 0
void init_cache(CacheLine cache[][SET_SIZE]) {
	init_rand();
	for(int i=0; i<(int)(CACHE_LINES/SET_SIZE); ++i)
		for(int j=0; j<SET_SIZE; ++j)
			cache[i][j].valid_bit = 0;
}














