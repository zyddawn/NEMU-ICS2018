#include "nemu.h"
#include "cpu/cpu.h"
#include "memory/memory.h"
#include "device/mm_io.h"
#include <memory.h>
#include <stdio.h>


uint8_t hw_mem[MEM_SIZE_B];

uint32_t hw_mem_read(paddr_t paddr, size_t len) {
	uint32_t ret = 0;
	memcpy(&ret, hw_mem + paddr, len);

	uint32_t temp = 0;
	memcpy(&temp, hw_mem + 0x7ffffe8, 4);
	printf("paddr (read) = 0x%x, and 0x7ffffe8 store = 0x%x\n", paddr, temp);	
	return ret;
}

void hw_mem_write(paddr_t paddr, size_t len, uint32_t data) {
	memcpy(hw_mem + paddr, &data, len);
	printf("before write: \n");
	for(int i = paddr; i < paddr + 16; i += 4) {
		printf("0x%x: 0x%x 0x%x 0x%x 0x%x\n", i, hw_mem[i+3], hw_mem[i+2], hw_mem[i+1], hw_mem[i]);
	}
	printf("paddr (write) = 0x%x, data = 0x%x\n", paddr, data);
	printf("after write: \n");
	for(int i = paddr; i < paddr + 16; i += 4) {
		printf("0x%x: 0x%x 0x%x 0x%x 0x%x\n", i, hw_mem[i+3], hw_mem[i+2], hw_mem[i+1], hw_mem[i]);
	}
	printf("\n");
}

uint32_t paddr_read(paddr_t paddr, size_t len) {
	uint32_t ret = 0;
	ret = hw_mem_read(paddr, len);
	return ret;
}

void paddr_write(paddr_t paddr, size_t len, uint32_t data) {
	hw_mem_write(paddr, len, data);
}


uint32_t laddr_read(laddr_t laddr, size_t len) {
	return paddr_read(laddr, len);
}

void laddr_write(laddr_t laddr, size_t len, uint32_t data) {
	paddr_write(laddr, len, data);
}


uint32_t vaddr_read(vaddr_t vaddr, uint8_t sreg, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	return laddr_read(vaddr, len);
}

void vaddr_write(vaddr_t vaddr, uint8_t sreg, size_t len, uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
	laddr_write(vaddr, len, data);
}

void init_mem() {
	// clear the memory on initiation
	memset(hw_mem, 0, MEM_SIZE_B);

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
