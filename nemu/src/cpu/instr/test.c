#include "cpu/instr.h"

static void instr_execute_2op() {
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val = opr_dest.val & opr_src.val;
	cpu.eflags.CF = cpu.eflags.OF = 0;
}

make_instr_impl_2op(test, i, a, b)
make_instr_impl_2op(test, i, a, v)
make_instr_impl_2op(test, i, rm, b)
make_instr_impl_2op(test, i, rm, v)
make_instr_impl_2op(test, r, rm, b)
make_instr_impl_2op(test, r, rm, v)


