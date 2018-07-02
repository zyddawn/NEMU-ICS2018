#include "memory/memory.h"
#include "memory/cache.h"
#include <memory.h>
#include <stdio.h>

BLOCK cache[CACHE_LINES/8][8];

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

void move_to_cache(paddr_t paddr) {


}


uint32_t get_cache_data(uint32_t set_index, uint32_t inner_set_index, uint32_t tag, bool *hit) {
	// Not Implemented Yet	


}


// be careful when some of the data is on another block
uint32_t cache_read(paddr_t paddr, size_t len, BLOCK cache[][8]) {
	if (len == 0)
		return 0;

	bool cache_hit = false;
	uint32_t res = 0, data_tag, data_set_index, data_block_index;

	for(paddr_t cur_addr = paddr+len-1; cur_addr>=paddr; --cur_addr) {
		cache_hit = false;
		data_tag = get_tag(cur_addr);
		data_set_index = get_set_index(cur_addr);
		data_block_index = get_block_index(cur_addr);
		
		if (data_tag > 0x4000 || data_set_index > 128 || data_block_index > 64) {
			Log("Parsing error! Wrong paddr.");
			return 0;
		}
		
		for(int i=0; i<8; ++i) {
			// found data in cache
			if (cache[data_set_index][i].tag == data_tag && cache[data_set_index][i].valid_bit) {
				cache_hit = true;	
				res = (res << 8) + cache[data_set_index][i].block_data[data_block_index];
				break;
			}
		}
		if (!cache_hit)
	
	}




}















