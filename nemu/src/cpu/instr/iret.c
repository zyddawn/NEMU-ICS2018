#include "cpu/instr.h"
#include "memory/memory.h"

static uint32_t iret_pop() {
	uint32_t temp = vaddr_read(cpu.esp, SREG_SS, 4);
	cpu.esp -= 4;
	return temp;
}

make_instr_func(iret_v) {
	uint32_t origin_eip = iret_pop(),
		 origin_cs = iret_pop(),
		 origin_eflags = iret_pop();
	
	cpu.eip = origin_eip;	// recover eip
	cpu.cs.val = origin_cs;
	load_sreg(1);   // reload sreg
	cpu.eflags.val = origin_eflags;
	
	return 0;
}


