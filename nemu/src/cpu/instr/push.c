#include "cpu/instr.h"
#include "cpu/reg.h"

static void instr_execute() {
	cpu.esp -= data_size / 8;
}

push_reg_helper(push, eax, AX)
push_reg_helper(push, ecx, CX)
push_reg_helper(push, edx, DX)
push_reg_helper(push, ebx, BX)
push_reg_helper(push, esp, SP)
push_reg_helper(push, ebp, BP)
push_reg_helper(push, esi, SI)
push_reg_helper(push, edi, DI)


