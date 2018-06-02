#include "cpu/instr.h"

make_instr_func(leave) {
	printf("before leave: \n");
	print_reg();
	// esp = ebp
	cpu.esp = cpu.ebp;

	OPERAND old_ebp;
	old_ebp.data_size = data_size;
	old_ebp.type = OPR_MEM;
	old_ebp.addr = REG_ESP;
	operand_read(&old_ebp);
	// ebp = pop()
	cpu.ebp = old_ebp.val;
	cpu.esp += data_size / 8;
	
	printf("After leave: \n");
	print_reg();
	return 1;
}






