#include "cpu/instr.h"
#include "cpu/alu.h" 

static void instr_execute_2op() {
	operand_read(&opr_src);
	operand_read(&opr_dest);
	alu_sub(opr_src.val, opr_dest.val);
}


make_instr_impl_2op(cmp, i, rm, b)
make_instr_impl_2op(cmp, i, rm, v)
make_instr_impl_2op(cmp, i, a, b)
make_instr_impl_2op(cmp, i, a, v)
make_instr_impl_2op(cmp, r, rm, b)
make_instr_impl_2op(cmp, r, rm, v)
make_instr_impl_2op(cmp, rm, r, b)
make_instr_impl_2op(cmp, rm, r, v)


make_instr_func(cmp_si2rm_bv) {
	int len = 1;
	OPERAND opr_src, opr_dest;
	decode_data_size_bv
	decode_operand_i2rm
	print_asm_2("cmp", "bv", len, &opr_src, &opr_dest);
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_src.val = sign_ext(opr_src.val, 8);
	alu_sub(opr_src.val, opr_dest.val);
	return len;
}
