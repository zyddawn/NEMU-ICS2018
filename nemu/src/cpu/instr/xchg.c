#include "cpu/instr.h"

static void instr_execute_2op() {
	operand_read(&opr_src);
	operand_read(&opr_dest);
	uint32_t temp = opr_src.val;
	opr_src.val = opr_dest.val;
	opr_dest.val = temp;
	operand_write(&opr_src);
	operand_write(&opr_dest);
}


make_instr_impl_2op(xchg, i, rm, v)
make_instr_impl_2op(xchg, i, rm, b)
make_instr_impl_2op(xchg, i, a, b)
make_instr_impl_2op(xchg, i, a, v)
make_instr_impl_2op(xchg, r, rm, b)
make_instr_impl_2op(xchg, r, rm, v)
make_instr_impl_2op(xchg, rm, r, b)
make_instr_impl_2op(xchg, rm, r, v)

