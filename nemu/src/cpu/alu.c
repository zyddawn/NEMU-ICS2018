#include "cpu/cpu.h"

uint32_t alu_add(uint32_t src, uint32_t dest) {
	uint32_t res = 0;
	res = src + dest;
	// ZF
	cpu.eflags.ZF = (res==0)? 1 : 0;
	// PF
	int count1 = 0;
	uint32_t temp = res;
	while(temp > 0) {
		if(temp & 0x1)   // last digit is 1
			++ count1;
		temp >>= 1;
	}
	cpu.eflags.PF = (count1 & 0x1);  // count1 is odd
	// SF
	cpu.eflags.SF = (res >> 31) & 0x1;
	// CF
	
	// OF
	
	return res;
}

uint32_t alu_adc(uint32_t src, uint32_t dest) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}


uint32_t alu_sub(uint32_t src, uint32_t dest) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}

uint32_t alu_sbb(uint32_t src, uint32_t dest) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}


uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}

uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}

int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}

uint32_t alu_mod(uint64_t src, uint64_t dest) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}

int32_t alu_imod(int64_t src, int64_t dest) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}

uint32_t alu_and(uint32_t src, uint32_t dest) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}

uint32_t alu_xor(uint32_t src, uint32_t dest) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}

uint32_t alu_or(uint32_t src, uint32_t dest) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}
