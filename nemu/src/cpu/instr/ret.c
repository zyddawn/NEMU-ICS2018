#include "cpu/instr.h"

make_instr_func(ret_near) {
	OPERAND pop_eip;
	pop_eip.data_size = data_size;
	pop_eip.type = OPR_MEM;
#ifdef DEBUG
	
	printf("before ret esp = 0x%x\n", cpu.esp);
#endif
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
#ifdef DEBUG	
	printf("after ret eip = 0x%x, cpu.esp = 0x%x\n", cpu.eip, cpu.esp);
	OPERAND temp1, temp2;
	temp1.data_size = temp2.data_size = data_size;
	temp1.addr = REG_ESP;
	temp2.addr = REG_EBP;
	temp1.type = temp2.type = OPR_MEM;
	operand_read(&temp1);
	operand_read(&temp2);
	while(1)
		printf("ret ebp = 0x%x, esp = 0x%x\n", temp2.val, temp1.val);
#endif	
	return 0;
}


