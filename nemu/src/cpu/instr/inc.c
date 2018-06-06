#include "cpu/instr.h"

static void instr_execute_1op() {
	operand_read(&opr_src);
	print_reg();
	uint32_t temp_CF = cpu.eflags.CF;
	opr_src.val = alu_add(opr_src.val, 1);
	cpu.eflags.CF = temp_CF;
	printf("after inc:\n");
	operand_write(&opr_src);
	print_reg();
}

// make_instr_impl_1op(inc, rm, b)
make_instr_impl_1op(inc, rm, v)
make_instr_impl_1op(inc, r, v)




