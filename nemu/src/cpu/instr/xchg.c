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

make_instr_impl_2op(xchg, r, rm, b)
make_instr_impl_2op(xchg, r, rm, v)

/*
make_instr_func(xchg_a2r_b) {
	int len = 1;
	data_size_b
	opr_src.type = opr_dest.type = OPR_REG;
	


}

make_instr_func(xchg_a2r_v) {


}
*/
