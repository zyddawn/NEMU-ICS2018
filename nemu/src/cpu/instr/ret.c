#include "cpu/instr.h"
#include "cpu/reg.h"

#define DEBUG

make_instr_func(ret_near) {
	OPERAND pop_eip;
	pop_eip.data_size = data_size;
	
#ifdef DEBUG
	bool success=false;
	int esp = get_reg_val("%esp", &success);
	if (success)
		printf("before ret %esp = 0x%x\n", esp);
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
	esp = get_reg_val("%esp", &success);
	if (success)
		printf("after ret %esp = 0x%x\n", esp);
	return 0;
}


