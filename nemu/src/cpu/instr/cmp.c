#include "cpu/instr.h"

static void instr_execute_2op() {
	operand_read(&opr_src);
	operand_read(&opr_dest);
	// printf("Before cmp: \n");
	// print_flags();
	// int temp = 
	int temp = alu_sub(opr_src.val, opr_dest.val);
	if (opr_src.val < opr_dest.val && temp < 0)
		cpu.eflags.OF = 1;
	// printf("%d - %d = %d\n", opr_dest.val, opr_src.val, temp);
	// printf("After cmp: \n");
	// print_flags();
}

static void instr_execute_si2rm_bv() {
	operand_read(&opr_src);
	operand_read(&opr_dest);
	alu_sub(sign_ext(opr_src.val, 8), opr_dest.val);
}

make_instr_impl_2op(cmp, i, rm, b)
make_instr_impl_2op(cmp, i, rm, v)
make_instr_impl_2op(cmp, i, a, b)
make_instr_impl_2op(cmp, i, a, v)
make_instr_impl_2op(cmp, r, rm, b)
make_instr_impl_2op(cmp, r, rm, v)
make_instr_impl_2op(cmp, rm, r, b)
make_instr_impl_2op(cmp, rm, r, v)
make_SignExt_impl_si2rm_bv(cmp, si, rm, bv)

