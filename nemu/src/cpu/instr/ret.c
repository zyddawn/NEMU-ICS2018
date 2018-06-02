#include "cpu/instr.h"

make_instr_func(ret_near) {
	OPERAND pop_eip;
	pop_eip.data_size = data_size;
	pop_eip.type = OPR_MEM;

	if (data_size == 16) {
		pop_eip.addr = REG_SP;
		operand_read(&pop_eip);
		cpu.esp += 2;
		cpu.eip = pop_eip.val;
	}
	else {
		pop_eip.addr = REG_ESP;
		operand_read(&pop_eip);
		cpu.esp += 4;
		cpu.eip = pop_eip.val;
	}
	printf("after RET eip = 0x%x\n", cpu.eip);
	
	OPERAND temp1, temp2;
	temp1.data_size = temp2.data_size = data_size;
	temp1.addr = cpu.ebp;
	temp2.addr = cpu.ebp - 4;
	temp1.type = temp2.type = OPR_MEM;
	operand_read(&temp1);
	operand_read(&temp2);
	printf("ebp store = 0x%x, ebp-4 store = 0x%x\n", temp1.val, temp2.val);

	return 0;
}


