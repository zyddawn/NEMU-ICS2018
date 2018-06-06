#include "cpu/instr.h"

make_instr_func(ret_near) {
	// printf("ret...\n");
	OPERAND pop_eip;
	pop_eip.data_size = data_size;
	pop_eip.type = OPR_MEM;
	pop_eip.sreg = SREG_SS;
	pop_eip.addr = cpu.esp;

	operand_read(&pop_eip);
	cpu.esp += 4;
	
	cpu.eip = pop_eip.val;
	return 0;
}

make_instr_func(ret_near_i) {
	int len = 1;
	OPERAND opr_src, pop_eip;
	opr_src.data_size = 16;
	decode_operand_i
	operand_read(&opr_src);
	
	pop_eip.data_size = 8 * opr_src.val;
	pop_eip.type = OPR_MEM;
	pop_eip.addr = cpu.esp;
	
	operand_read(&pop_eip);
	cpu.esp += 4;

	cpu.eip = pop_eip.val;
	return 0;
}


