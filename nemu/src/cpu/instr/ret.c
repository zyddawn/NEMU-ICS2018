#include "cpu/instr.h"

make_instr_func(ret_near) {
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
	cpu.eip = pop_eip.val;
	//}
	return 0;
}


