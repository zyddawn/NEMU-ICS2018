#include "cpu/instr.h"

static void instr_execute() {
	cpu.esp -= 4;
}

push_REG_helper(push, eax, AX)
push_REG_helper(push, ecx, CX)
push_REG_helper(push, edx, DX)
push_REG_helper(push, ebx, BX)
push_REG_helper(push, esp, SP)
push_REG_helper(push, ebp, BP)
push_REG_helper(push, esi, SI)
push_REG_helper(push, edi, DI)


make_instr_func(push_rm_v) {
	int len = 1;
	OPERAND opr_src, opr_dest;
	decode_data_size_v
	decode_operand_rm
	operand_read(&opr_src);
	opr_dest.type = OPR_MEM;
	instr_execute();
	opr_dest.addr = cpu.esp;
	opr_dest.val = opr_src.val;
	operand_write(&opr_dest);
	return len;
}






