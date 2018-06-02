#include "cpu/instr.h"

make_instr_func(leave) {
	// esp = ebp
	cpu.esp = cpu.ebp;
	print_reg();
	
	OPERAND old_ebp;
	old_ebp.data_size = data_size;
	old_ebp.type = OPR_IMM;
	old_ebp.addr = cpu.esp;
	old_ebp.sreg = SREG_SS;
	operand_read(&old_ebp);
	
	// ebp = pop()
        cpu.ebp = old_ebp.val;
        cpu.esp += data_size / 8;
	printf("\n");
	print_reg();
	while(1)
		;
	return 1;
}






