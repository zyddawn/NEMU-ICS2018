#include "nemu.h"
#include "cpu/cpu.h"
#include "memory/memory.h"
#include "memory/cache.h"
#include "device/mm_io.h"
#include <memory.h>
#include <stdio.h>

uint8_t hw_mem[MEM_SIZE_B];

uint32_t hw_mem_read(paddr_t paddr, size_t len) {
	uint32_t ret = 0;
	memcpy(&ret, hw_mem + paddr, len);
	return ret;
}

void hw_mem_write(paddr_t paddr, size_t len, uint32_t data) {
	memcpy(hw_mem + paddr, &data, len);
}

uint32_t paddr_read(paddr_t paddr, size_t len) {
	uint32_t ret = 0;
#ifdef CACHE_ENABLED
	ret = cache_read(paddr, len, L1_dcache);
#else
	ret = hw_mem_read(paddr, len);
#endif
	return ret;
}

void paddr_write(paddr_t paddr, size_t len, uint32_t data) {
#ifdef CACHE_ENABLED
	cache_write(paddr, len, data, L1_dcache);
#else	
	hw_mem_write(paddr, len, data);
#endif
}


uint32_t laddr_read(laddr_t laddr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	paddr_t hwaddr = laddr;
	if (cpu.cr0.pg && cpu.cr0.pe) {
		uint32_t data_start_page = hwaddr & 0xfffff000,
			 data_end_page = (hwaddr + len - 1) & 0xfffff000;
		if (data_start_page != data_end_page) {  // data not in a same page
			uint32_t res = 0;
			while(len--)
				res = (res << 8) | laddr_read(hwaddr+len-1, 1);
			return res;
		}
		else
			hwaddr = page_translate(hwaddr);
	}
	return paddr_read(hwaddr, len);
}

void laddr_write(laddr_t laddr, size_t len, uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
	paddr_t hwaddr = laddr;
	if(cpu.cr0.pg && cpu.cr0.pe) {
		uint32_t data_start_page = hwaddr & 0xfffff000,
			 data_end_page = (hwaddr + len - 1) & 0xfffff000;
 		if (data_start_page != data_end_page) {   // data not in a same page
			uint32_t temp = data;
			for(uint32_t i=0; i<len; ++i) {
				laddr_write(hwaddr+i, 1, (temp & 0xff));
				temp >>= 8;
			}
			return ;
		}
		else
			hwaddr = page_translate(hwaddr);
	}
	paddr_write(hwaddr, len, data);
}


uint32_t vaddr_read(vaddr_t vaddr, uint8_t sreg, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	uint32_t laddr = vaddr;
	if (cpu.cr0.pe) {
		laddr = segment_translate(vaddr, sreg);
		// Might need to fix many bugs in previous instructions
	}
	return laddr_read(laddr, len);
}

void vaddr_write(vaddr_t vaddr, uint8_t sreg, size_t len, uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
	uint32_t laddr = vaddr;
	if(cpu.cr0.pe) {
		laddr = segment_translate(vaddr, sreg);
		// same as above
	}
	laddr_write(laddr, len, data);
}

void init_mem() {
	// clear the memory on initiation
	memset(hw_mem, 0, MEM_SIZE_B);
#ifdef CACHE_ENABLED
	init_cache(L1_dcache);
#endif

#ifdef TLB_ENABLED
	make_all_tlb();
	init_all_tlb();
#endif
}

uint32_t instr_fetch(vaddr_t vaddr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	return vaddr_read(vaddr, SREG_CS, len);
}

uint8_t * get_mem_addr() {
	return hw_mem;
}
