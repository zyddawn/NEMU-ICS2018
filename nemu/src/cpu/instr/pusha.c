#include "cpu/instr.h"
#include "memory/memory.h"

static void push_reg(uint32_t reg_data) {
	cpu.esp -= 4;
	vaddr_write(cpu.esp, SREG_SS, 4, reg_data);
}

make_instr_func(pusha_r_v) {
	uint32_t origin_esp = cpu.esp;
	push_reg(cpu.eax);
	push_reg(cpu.edx);
	push_reg(cpu.ecx);
	push_reg(cpu.ebx);
	push_reg(origin_esp);
	push_reg(cpu.ebp);
	push_reg(cpu.esi);
	push_reg(cpu.edi);
	return 1;
}


