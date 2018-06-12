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
	// if(paddr == cpu.ebp-4 && hw_mem[cpu.ebp-4] == 3)
	//	printf("[ebp-4] = %d\n", hw_mem[cpu.ebp-4]);
	// if(paddr == cpu.ebp + 8)
	//	printf("[ebp+8] = %d\n", hw_mem[cpu.ebp+8]+256*hw_mem[cpu.ebp+9]);
	
	unsigned * code_mem = (unsigned * )get_mem_addr() + 0x30000;
	printf("code addr = 0x%x, content = %02x %02x %02x %02x\n", (unsigned)code_mem, *code_mem, *(code_mem+1), *(code_mem+2), *(code_mem+3));
	return ret;
}

void hw_mem_write(paddr_t paddr, size_t len, uint32_t data) {
	memcpy(hw_mem + paddr, &data, len);
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
