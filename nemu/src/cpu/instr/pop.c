#include "cpu/instr.h"

static void instr_execute_1op() {
	opr_dest.data_size = 32;
	opr_dest.type = OPR_MEM;
	opr_dest.addr = cpu.esp;
	operand_read(&opr_dest);
	if(data_size == 8)
		opr_dest.val &= 0xFF;
	else if(data_size == 16)
		opr_dest.val &= 0xFFFF;
	opr_src.val = opr_dest.val;
	operand_write(&opr_src);
	cpu.esp += 4;
}

make_instr_impl_1op(pop, r, v)
make_instr_impl_1op(pop, rm, v)

/*
static void instr_execute() {
	cpu.esp += 4;
}

pop_REG_helper(eax, AX)
pop_REG_helper(ecx, CX)
pop_REG_helper(edx, DX)
pop_REG_helper(ebx, BX)
pop_REG_helper(esp, SP)
pop_REG_helper(ebp, BP)
pop_REG_helper(esi, SI)
pop_REG_helper(edi, DI) */


