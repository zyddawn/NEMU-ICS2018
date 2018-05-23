#include "cpu/instr.h"
#include "cpu/reg.h"

static void instr_execute() {
	if (opr_dest.data_size == 16)
		cpu.esp -= 2;
	else
		cpu.esp -= 4;
}

push_pop_reg_helper(push, eax, AX)
push_pop_reg_helper(push, ecx, CX)
push_pop_reg_helper(push, edx, DX)
push_pop_reg_helper(push, ebx, BX)
push_pop_reg_helper(push, esp, SP)
push_pop_reg_helper(push, ebp, BP)
push_pop_reg_helper(push, esi, SI)
push_pop_reg_helper(push, edi, DI)


