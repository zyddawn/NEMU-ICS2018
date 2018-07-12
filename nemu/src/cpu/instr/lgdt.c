#include "cpu/instr.h"


static void instr_execute_1op() {
	laddr_t mem_addr = opr_src.val;
	cpu.gdtr.limit = laddr_read(mem_addr, 2) & 0xFFFF;
	cpu.gdtr.base =  laddr_read(mem_addr + 2, 4) & 0xFFFFFFFF;
	if (data_size == 16)
		cpu.gdtr.base &= 0x00FFFFFF;
		// not sure if it's correct
}


make_instr_impl_1op(lgdt, rm, v)








