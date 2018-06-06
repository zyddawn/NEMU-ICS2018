#include "cpu/instr.h"

static void instr_execute_1op() {
	operand_read(&opr_src);
	print_reg();
	opr_src.val = alu_add(opr_src.val, 1);
	printf("after inc:\n");
	operand_write(&opr_src);
	print_reg();
}

/* make_instr_impl_1op(inc, rm, b)
make_instr_impl_1op(inc, rm, v)
make_instr_impl_1op(inc, r, v) */

inc_REG_helper(eax, AX);
inc_REG_helper(edx, DX);
inc_REG_helper(ecx, CX);
inc_REG_helper(ebx, BX);
inc_REG_helper(esp, SP);
inc_REG_helper(ebp, BP);
inc_REG_helper(esi, SI);
inc_REG_helper(edi, DI);











