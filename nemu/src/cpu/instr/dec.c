#include "cpu/instr.h"

static void instr_execute_1op() {
	operand_read(&opr_src);
	uint32_t temp_CF = cpu.eflags.CF;
	// printf("Before dec: 0x%x\n", opr_src.val);
	opr_src.val = alu_sub(1, opr_src.val);
	cpu.eflags.CF = temp_CF;
	operand_write(&opr_src);
	// printf("after dec: 0x%x\n", opr_src.val);
}

// make_instr_impl_1op(dec, rm, b)
make_instr_impl_1op(dec, r, v)
make_instr_impl_1op(dec, rm, v)




