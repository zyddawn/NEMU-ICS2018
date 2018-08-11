#include "cpu/instr.h"

static void instr_execute_1op() {
	laddr_t mem_addr = opr_src.val;
	cpu.idtr.limit = laddr_read(mem_addr, 2) & 0xffff;
	cpu.idtr.base = laddr_read(mem_addr + 2, 4) & 0xffffffff;
	if (data_size == 16)
		cpu.idtr.base &= 0x00ffffff;
}

make_instr_impl_1op(lidt, rm, v)

