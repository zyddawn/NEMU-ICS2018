#include "cpu/instr.h"
#include "cpu/reg.h"

static void instr_execute() {
	cpu.esp -= data_size / 8;
}

push_pop_reg_helper(push, eax, AX)
push_pop_reg_helper(push, ecx, CX)
push_pop_reg_helper(push, edx, DX)
push_pop_reg_helper(push, ebx, BX)
push_pop_reg_helper(push, esp, SP)
push_pop_reg_helper(push, ebp, BP)
push_pop_reg_helper(push, esi, SI)
push_pop_reg_helper(push, edi, DI)


