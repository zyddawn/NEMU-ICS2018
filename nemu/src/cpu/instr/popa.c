#include "cpu/instr.h"
#include "memory/memory.h"

#define pop_reg(reg_name) \
	reg_name = vaddr_read(cpu.esp, SREG_SS, 4); \
	cpu.esp += 4;
	
make_instr_func(popa_r_v) {
	// printf("popa...\n");
	pop_reg(cpu.edi)
	pop_reg(cpu.esi)
	pop_reg(cpu.ebp)
	cpu.esp += 4;
	pop_reg(cpu.ebx)
	pop_reg(cpu.ecx)
	pop_reg(cpu.edx)
	pop_reg(cpu.eax)
	return 1;
}



