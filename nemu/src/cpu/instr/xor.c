#include "cpu/instr.h"

static void instr_execute_2op() {
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val = alu_xor(opr_src.val, opr_dest.val);
	operand_write(&opr_dest);
}

static void instr_execute_si2rm_bv() {
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val = alu_xor(sign_ext(opr_src.val, 8), opr_dest.val);
	operand_write(&opr_dest);
}

make_instr_impl_2op(xor, i, rm, b)
make_instr_impl_2op(xor, i, rm, v)
make_instr_impl_2op(xor, i, a, b)
make_instr_impl_2op(xor, i, a, v)
make_instr_impl_2op(xor, r, rm, b)
make_instr_impl_2op(xor, r, rm, v)
make_instr_impl_2op(xor, rm, r, b)
make_instr_impl_2op(xor, rm, r, v)
make_SignExt_impl_si2rm_bv(xor)


