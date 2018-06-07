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
void set_ZF(uint32_t res) {
	cpu.eflags.ZF = (res==0);
}
uint32_t Sign_64(uint64_t res, size_t data_size) {
	return ((res>>(data_size-1)) & 0x1);
}
uint32_t Sign(uint32_t res, size_t data_size) {
	return ((res>>(data_size-1)) & 0x1);
}
void set_PF(uint32_t res) {
	uint32_t cnt1 = cnt_one_in_digits(res);
	cpu.eflags.PF = (cnt1 & 0x1)==0;
}
void set_SF(uint32_t res, size_t data_size) {	// data_size in {8, 16, 32, 64}
	cpu.eflags.SF = Sign(res, data_size);
}

void set_OF_add(uint32_t src, uint32_t dest, uint32_t res) {
	cpu.eflags.OF = (Sign(src, 32)==Sign(dest, 32)) && (Sign(src, 32)^Sign(res, 32));
}

void set_CF_add(uint32_t src, uint32_t res) {
	cpu.eflags.CF = (res < src);
}

uint32_t alu_add(uint32_t src, uint32_t dest) {
	uint32_t res = src + dest;
	set_ZF(res);
	set_PF(res);
	set_SF(res, 32);
	set_OF_add(src, dest, res);
	set_CF_add(src, res);
	// printf("After add, eflags: \n");
	// print_flags();
	return res;
}

uint32_t alu_adc(uint32_t src, uint32_t dest) {
	uint32_t res, prev_CF = cpu.eflags.CF,
		 cur_CF, cur_OF;
	if (src <= 0) {
		res = alu_add(pref_CF, src);
		res = alu_add(res, dest); }
	else if (dest <= 0) {
		res = alu_add(pref_CF, dest);
		res = alu_add(res, src); }
	else {
		res = alu_add(src, dest);
		cur_CF = cpu.eflags.CF;
		cur_OF = cpu.eflags.OF;
		if(prev_CF)
       			res = alu_add(res, 1);
		// src+dest+CF should be done within one step, but here we separate it into 2 steps;
		// thus any of these steps caused CF or OF to be 1, we should set CF or OF to be 1
		cpu.eflags.CF = cur_CF | cpu.eflags.CF;
		cpu.eflags.OF = cur_OF | cpu.eflags.OF;
	}
	return res;
	
	/* uint32_t prev_CF = cpu.eflags.CF, res, cur_CF, cur_OF;
	res = alu_add(src, dest);
	cur_CF = cpu.eflags.CF;
	cur_OF = cpu.eflags.OF;
	if(prev_CF)
       		res = alu_add(res, 1);
	// src+dest+CF should be done within one step, but here we separate it into 2 steps;
	// thus any of these steps caused CF or OF to be 1, we should set CF or OF to be 1
	cpu.eflags.CF = cur_CF | cpu.eflags.CF;
	cpu.eflags.OF = cur_OF | cpu.eflags.OF;
	return res; */
}

uint32_t alu_sub(uint32_t src, uint32_t dest) {
	uint32_t res, compl_src = 0xFFFFFFFF - src + 1;
	if (compl_src == 0x80000000) {
		cpu.eflags.CF = 1;
		res = alu_adc(0x7FFFFFFF, dest);}
	else
		res = alu_add(compl_src, dest);
	return res;
	/* uint32_t res, compl_src = 0xFFFFFFFF-src+1;
	res = alu_add(compl_src, dest);
	cpu.eflags.CF = (dest<src);
	return res; */
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

void set_ZF_64(uint64_t res) {
	cpu.eflags.ZF = (res==0);
}
void set_SF_64(uint64_t res, size_t data_size) {
	cpu.eflags.SF = Sign_64(res, data_size);
}

uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size) {
	uint64_t res, temp_src = src, temp_dest = dest, high_bits;
	uint32_t low_bits;
	res = temp_src * temp_dest;
	size_t lower_bound = data_size-1, upper_bound = 2*data_size-1;
	low_bits = ((1<<lower_bound)-1+(1<<lower_bound)) & res;
	high_bits = ((1<<upper_bound)-1+(1<<upper_bound)) & res;
	high_bits -= low_bits;
	cpu.eflags.CF = (high_bits != 0);
	cpu.eflags.OF = cpu.eflags.CF;
	set_PF(low_bits);
	set_ZF_64(high_bits|low_bits);
	set_SF_64(high_bits, 64);
	return res;
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size) {
	int64_t res = src * dest;
	return res;
}

uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size) {
	uint32_t res = dest / src;
	return res;
}

int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size) {
	int32_t res = dest / src;
	return res;
}

uint32_t alu_mod(uint64_t src, uint64_t dest) {
	uint32_t res = dest % src;
	return res;
}

int32_t alu_imod(int64_t src, int64_t dest) {
	int32_t res = dest % src;
	return res;
}

uint32_t alu_and(uint32_t src, uint32_t dest) {
	uint32_t res = src & dest;
	cpu.eflags.OF = cpu.eflags.CF = 0;
	set_PF(res);
	set_ZF(res);
	set_SF(res, 32);
	return res;
}

uint32_t alu_xor(uint32_t src, uint32_t dest) {
	uint32_t res = src ^ dest;
	// LIKE IN alu_and
	cpu.eflags.OF = cpu.eflags.CF = 0;
	set_PF(res);
        set_ZF(res);
	set_SF(res, 32);
	return res;
}

uint32_t alu_or(uint32_t src, uint32_t dest) {
	uint32_t res = src | dest;
	// LIKE IN alu_and
	cpu.eflags.OF = cpu.eflags.CF = 0;
	set_PF(res);
	set_ZF(res);
	set_SF(res, 32);
	return res;
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size) {
	size_t upper_bound = data_size-1;
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
	set_PF(low_bits);
	set_ZF(low_bits);
	set_SF(low_bits, data_size);
	return (high_bits | low_bits);
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size) {
	size_t upper_bound = data_size - 1;
	uint32_t low_bits = (1<<upper_bound) - 1 + (1<<upper_bound);
	low_bits &= dest;
	uint32_t high_bits = dest - low_bits;
	for(int i=0; i<src; ++i) {
		if(i == src-1) {
			cpu.eflags.CF = low_bits & 0x1;
			cpu.eflags.OF = (low_bits & (low_bits>>1)) & 0x1;
		}
		low_bits >>= 1;
	}
	set_PF(low_bits);
	set_ZF(low_bits);
	set_SF(low_bits, data_size);
	return (high_bits | low_bits);
}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size) {
	size_t upper_bound = data_size - 1;
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
	set_PF(low_bits);
	set_ZF(low_bits);
	set_SF(low_bits, data_size);
	return (high_bits | low_bits);
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size) {
	// THE SAME AS alu_shl
	return alu_shl(src, dest, data_size);
}

// GOODBYE PA 1-2
