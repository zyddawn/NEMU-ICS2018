#include "cpu/instr.h"

static void instr_execute_2op() {
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val = alu_and(opr_src.val, opr_dest.val);
	operand_write(&opr_dest);
}

make_instr_impl_2op(and, i, rm, b)
make_instr_impl_2op(and, i, rm, v)
make_instr_impl_2op(and, i, a, b)
make_instr_impl_2op(and, i, a, v)
make_instr_impl_2op(and, r, rm, b)
make_instr_impl_2op(and, r, rm, v)
make_instr_impl_2op(and, rm, r, b)
make_instr_impl_2op(and, rm, r, v)


make_instr_func(and_si2rm_bv) {
	int len = 1;
	OPERAND opr_src, opr_dest;
	decode_data_size_bv
	decode_operand_i2rm
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val = alu_and(sign_ext(opr_src.val, 8), opr_dest.val);
	operand_write(&opr_dest);
	return len;
}




