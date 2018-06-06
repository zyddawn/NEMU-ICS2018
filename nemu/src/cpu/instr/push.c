#include "cpu/instr.h"

static void instr_execute() {
	cpu.esp -= 4;
}

push_REG_helper(eax, AX)
push_REG_helper(ecx, CX)
push_REG_helper(edx, DX)
push_REG_helper(ebx, BX)
push_REG_helper(esp, SP)
push_REG_helper(ebp, BP)
push_REG_helper(esi, SI)
push_REG_helper(edi, DI)


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

make_instr_func(push_i_b) {
	int len = 1;
	OPERAND opr_src, opr_dest;
	opr_src.data_size = 8;
	opr_dest.data_size = 32;
	decode_operand_i
	operand_read(&opr_src);
	instr_execute();
	opr_dest.type = OPR_MEM;
	opr_dest.addr = cpu.esp;
	opr_dest.val = sign_ext(opr_src.val, 8);
	operand_write(&opr_dest);
	return len;
}


make_instr_func(push_i_v) {
	int len = 1;
	OPERAND opr_src, opr_dest;
	opr_src.data_size = data_size;
	opr_dest.data_size = 32;
	decode_operand_i
	operand_read(&opr_src);
	instr_execute();
	opr_dest.type = OPR_MEM;
	opr_dest.addr = cpu.esp;
	opr_dest.val = sign_ext(opr_src.val, data_size);
	operand_write(&opr_dest);
	return len;



}



