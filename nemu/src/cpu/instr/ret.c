#include "cpu/instr.h"

make_instr_func(ret_near) {
	OPERAND pop_eip;
	pop_eip.data_size = data_size;
	if (data_size == 16) {
		pop_eip.addr = REG_SP;
		operand_read(&pop_eip);
		cpu.esp += 2;
		cpu.eip = pop_eip.val & 0x0000FFFF;
	}
	else {
		pop_eip.addr = REG_ESP;
		operand_read(&pop_eip);
		cpu.esp += 4;
		cpu.eip = pop_eip.val;
	}
	return 0;
}


