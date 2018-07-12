#include "cpu/instr.h"


static void instr_execute_1op() {

	// BUG EXISTS!!!

	cpu.gdtr.limit = opr_src.val & 0xFFFF;
	cpu.gdtr.base = opr_src.val >> 16;
	if (data_size == 16)
		cpu.gdtr.base &= 0x00FFFFFF;
		// not sure if it's correct
}


make_instr_impl_1op(lgdt, rm, v)








