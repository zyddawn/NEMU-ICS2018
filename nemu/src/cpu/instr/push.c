#include "cpu/instr.h"


static void instr_execute() {
	if (opr_dest.data_size == 16)
		cpu.esp -= 2;
	else
		cpu.esp -= 4;
}


push_pop_reg_helper(push, eax, AX, v)
push_pop_reg_helper(push, ecx, CX, v)
push_pop_reg_helper(push, edx, DX, v)
push_pop_reg_helper(push, ebx, BX, v)
push_pop_reg_helper(push, esp, SP, v)
push_pop_reg_helper(push, ebp, BP, v)
push_pop_reg_helper(push, esi, SI, v)
push_pop_reg_helper(push, edi, DI, v)










