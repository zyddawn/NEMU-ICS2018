#include "cpu/instr.h"

static void instr_execute_1op_cc_pass() {
	opr_src.val = 1;
	operand_write(&opr_src);
}

static void instr_execute_1op_cc_fail() {
	opr_src.val = 0;
	operand_write(&opr_src);
}

make_instr_impl_1op_cc(set, rm, b, e)	// 94
make_instr_impl_1op_cc(set, rm, b, a) 	// 97
make_instr_impl_1op_cc(set, rm, b, b) 	// 92
make_instr_impl_1op_cc(set, rm, b, g) 	// 9F
make_instr_impl_1op_cc(set, rm, b, l) 	// 9C
make_instr_impl_1op_cc(set, rm, b, o) 	// 90
make_instr_impl_1op_cc(set, rm, b, p) 	// 9A
make_instr_impl_1op_cc(set, rm, b, s) 	// 98
make_instr_impl_1op_cc(set, rm, b, ae) 	// 93
make_instr_impl_1op_cc(set, rm, b, ge)	// 9D
make_instr_impl_1op_cc(set, rm, b, le) 	// 9E
make_instr_impl_1op_cc(set, rm, b, na) 	// 96
make_instr_impl_1op_cc(set, rm, b, ne) 	// 95
make_instr_impl_1op_cc(set, rm, b, no) 	// 91
make_instr_impl_1op_cc(set, rm, b, np) 	// 9B
make_instr_impl_1op_cc(set, rm, b, ns) 	// 99


