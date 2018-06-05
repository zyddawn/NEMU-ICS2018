#include "cpu/instr.h"

static void instr_execute_2op() {
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val = alu_add(opr_src.val, opr_dest.val);
	operand_write(&opr_dest);
}

static void instr_execute_si2rm_bv() {
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val = alu_add(sign_ext(opr_src.val, 8), opr_dest.val);
	operand_write(&opr_dest);
}

make_instr_impl_2op(add, i, rm, v)
make_instr_impl_2op(add, i, rm, b)
make_instr_impl_2op(add, i, a, b)
make_instr_impl_2op(add, i, a, v)
make_instr_impl_2op(add, r, rm, b)
make_instr_impl_2op(add, r, rm, v)
make_instr_impl_2op(add, rm, r, b)
make_instr_impl_2op(add, rm, r, v)
make_SignExt_impl_si2rm_bv(add, si2rm_bv)

