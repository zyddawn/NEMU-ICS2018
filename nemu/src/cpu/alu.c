#include "cpu/cpu.h"

uint32_t cnt_one_in_digits(uint32_t x) {
	uint32_t cnt=0, upper_bound=8;
	for(uint32_t i=0; i<upper_bound; ++i) {
		if(x & 0x1)
			++cnt;
		x >>= 1;
	}
	return cnt;
}

uint32_t alu_add(uint32_t src, uint32_t dest) {
	uint32_t res = 0;
	res = src + dest;
	//printf("my_src = 0x%08x, my_dest = 0x%08x\n", src, dest);
	//printf("my_res = 0x%08x\n", res);
	// ZF
	cpu.eflags.ZF = (res==0)? 1 : 0;
	// PF
	uint32_t cnt1 = cnt_one_in_digits(res);
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
	uint32_t res, compl_src = 0xFFFFFFFF-src+1;
	res = alu_add(compl_src, dest);
	// CF
	cpu.eflags.CF = (dest<src)? 1 : 0;
	return res;
}

uint32_t alu_sbb(uint32_t src, uint32_t dest) {
	uint32_t res, prev_CF = cpu.eflags.CF;
	res = alu_sub(src, dest);
	uint32_t cur_CF = cpu.eflags.CF, \
		 cur_OF = cpu.eflags.OF;
	if(prev_CF)
		res = alu_sub(1, res);
	cpu.eflags.CF = cur_CF | cpu.eflags.CF;
	cpu.eflags.OF = cur_OF | cpu.eflags.OF;
	return res;
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
	uint32_t res = src & dest;
	// OF & CF
	cpu.eflags.OF = cpu.eflags.CF = 0;
	// PF
	cpu.eflags.PF = (cnt_one_in_digits(res) & 0x1)==0;
	// ZF
	cpu.eflags.ZF = (res==0);
	// SF
	cpu.eflags.SF = (res>>31) & 0x1;
	return res;
}

uint32_t alu_xor(uint32_t src, uint32_t dest) {
	uint32_t res = src ^ dest;
	// LIKE IN alu_and
	cpu.eflags.OF = cpu.eflags.CF = 0;
	cpu.eflags.PF = (cnt_one_in_digits(res) & 0x1)==0;
	cpu.eflags.ZF = (res==0);
	cpu.eflags.SF = (res>>31) & 0x1;
	return res;
}

uint32_t alu_or(uint32_t src, uint32_t dest) {
	uint32_t res = src | dest;
	// LIKE IN alu_and
	cpu.eflags.OF = cpu.eflags.CF = 0;
	cpu.eflags.PF = (cnt_one_in_digits(res) & 0x1)==0;
	cpu.eflags.ZF = (res==0);
	cpu.eflags.SF = (res>>31) & 0x1;
	return res;
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size) {
	uint32_t upper_bound = data_size-1;
	uint32_t low_bits = (1<<upper_bound) - 1 + (1<<upper_bound);
	low_bits &= dest;
	uint32_t high_bits = dest - low_bits;
	for(uint32_t i=0; i<src; ++i) {
		uint32_t sign = ((low_bits>>upper_bound) & 0x1), \
			 high_bit = ((low_bits>>(upper_bound-1)) & 0x1);
		// OF & CF
		if(i == src-1) {
			cpu.eflags.OF = sign ^ high_bit;
			cpu.eflags.CF = sign;
		}
		low_bits <<= 1;	
	}
	low_bits &= ((1<<upper_bound) -1  + (1<<upper_bound));
	// PF
	cpu.eflags.PF = (cnt_one_in_digits(low_bits) & 0x1)==0;
	// SF
	cpu.eflags.SF = (low_bits>>upper_bound);
	// ZF
	cpu.eflags.ZF = (low_bits==0);
	return (high_bits | low_bits);
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size) {
	uint32_t upper_bound = data_size - 1;
	uint32_t low_bits = (1<<upper_bound) - 1 + (1<<upper_bound);
	low_bits &= dest;
	//uint32_t sign = (low_bits>>upper_bound);
	uint32_t high_bits = dest - low_bits;
	//printf("high=0x%08x, low=0x%08x\n", high_bits, low_bits);
	for(int i=0; i<src; ++i) {
		//low_bits |= (sign<<upper_bound);
		if(i == src-1) {
			cpu.eflags.CF = low_bits & 0x1;
			cpu.eflags.OF = (low_bits & (low_bits>>1)) & 0x1;
		}
		low_bits >>= 1;
	}
	cpu.eflags.PF = (cnt_one_in_digits(low_bits) & 0x1)==0;
	cpu.eflags.SF = (low_bits>>upper_bound);
	cpu.eflags.ZF = (low_bits==0);
	return (high_bits | low_bits);
}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size) {
	uint32_t upper_bound = data_size - 1;
	uint32_t low_bits = (1<<upper_bound) - 1 + (1<<upper_bound);
       	low_bits &= dest;
	uint32_t sign = (low_bits>>upper_bound);
	uint32_t high_bits = dest - low_bits;
	for(int i=0; i<src; ++i) {
		if(i == src-1) {
			cpu.eflags.CF = low_bits & 0x1;
			cpu.eflags.OF = (low_bits & (low_bits>>1)) & 0x1;
		}
		low_bits >>= 1;
		low_bits |= (sign<<upper_bound);
	}
	cpu.eflags.PF = (cnt_one_in_digits(low_bits) & 0x1)==0;
	cpu.eflags.SF = sign;
	cpu.eflags.ZF = (low_bits==0);
	return (high_bits | low_bits);
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size) {
	// THE SAME AS alu_shl
	return alu_shl(src, dest, data_size);
}
