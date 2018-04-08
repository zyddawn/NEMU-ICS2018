#include "cpu/cpu.h"

uint32_t alu_add(uint32_t src, uint32_t dest) {
	uint32_t res = 0;
	res = src + dest;
	//printf("my_src = 0x%08x, my_dest = 0x%08x\n", src, dest);
	//printf("my_res = 0x%08x\n", res);
	// ZF
	cpu.eflags.ZF = (res==0)? 1 : 0;
	// PF
	uint32_t cnt1 = 0, temp = res;
	for(int cnt_bit=0; cnt_bit<8; ++cnt_bit) {
		if(temp & 0x1)   // last digit is 1
			++cnt1;
		temp >>= 1;
	}
	cpu.eflags.PF = (cnt1 & 0x1)==0;  // cnt1 is odd
	// SF
	cpu.eflags.SF = (res >> 31) & 0x1;
	// CF
	uint32_t Cin, A, B, Cout=0;
	for(int cnt_bit=0; cnt_bit<32; ++cnt_bit) {
		A = (src>>cnt_bit) & 0x1, B = (dest>>cnt_bit) & 0x1, Cin = Cout;
		Cout = (A&B) | (A&Cin) | (B&Cin);
	}
	cpu.eflags.CF = Cout;
	// OF
	cpu.eflags.OF = ((src>>31)==(dest>>31)) && ((src>>31)^(res>>31));
	return res;
}

uint32_t alu_adc(uint32_t src, uint32_t dest) {
	uint32_t prev_CF = cpu.eflags.CF, res;
       	res = alu_add(src, dest);
	uint32_t cur_CF = cpu.eflags.CF, \
		 cur_OF = cpu.eflags.OF;
	if(prev_CF)
       		res = alu_add(res, 1);
	// src+dest+CF should be done within one step, but here we separate it into 2 steps;
	// thus any of these steps caused CF or OF to be 1, we should set CF or OF to be 1
	cpu.eflags.CF = cur_CF | cpu.eflags.CF;
	cpu.eflags.OF = cur_OF | cpu.eflags.OF;
	return res;
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
