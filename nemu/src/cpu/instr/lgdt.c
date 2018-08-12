#include "cpu/instr.h"


static void instr_execute_1op() {
	printf("Calling LGDT.\n");
	laddr_t mem_addr = opr_src.val;
	cpu.gdtr.limit = laddr_read(mem_addr, 2) & 0xffff;
	printf("gdtr limit = 0x%x\n", cpu.gdtr.limit);
	cpu.gdtr.base =  laddr_read(mem_addr + 2, 4) & 0xffffffff;
	printf("gdtr base = 0x%x\n", cpu.gdtr.base);
	if (data_size == 16)
		cpu.gdtr.base &= 0x00ffffff;
		// not sure if it's correct
}


make_instr_impl_1op(lgdt, rm, v)








