#include "cpu/instr.h"

make_instr_func(ret_near) {
	// printf("ret...\n");
	OPERAND pop_eip;
	pop_eip.data_size = data_size;
	pop_eip.type = OPR_MEM;
	pop_eip.sreg = SREG_SS;
	pop_eip.addr = cpu.esp;

	/*if (data_size == 16) {
		operand_read(&pop_eip);
		cpu.esp += 2;
		cpu.eip = pop_eip.val;
	}
	else {*/
	operand_read(&pop_eip);
	cpu.esp += 4;
	
	/* printf("ret before pop eip = 0x%x, current eip = 0x%x\n", cpu.eip, pop_eip.val);
	print_reg(); */
	cpu.eip = pop_eip.val;
	//}
	return 0;
}


