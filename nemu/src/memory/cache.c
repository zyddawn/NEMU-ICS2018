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


uint32_t cache_read(paddr_t paddr, size_t len, ) {
	bool cache_hit = false;
		




}















