#include "cpu/instr.h"
#include "cpu/alu.h" 


static void instr_execute_2op() {
	uint32_t dest = opr_dest.val,
		 src = opr_src.val;
	alu_sub(src, dest);
	// opr_dest // reg
	// opr_src // imm
}

make_instr_impl_2op(cmp, i, rm, v)

