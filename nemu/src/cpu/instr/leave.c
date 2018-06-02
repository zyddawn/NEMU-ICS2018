#include "cpu/instr.h"

make_instr_func(leave) {
	// esp = ebp
	// cpu.esp = cpu.ebp;
	print_reg();
	
	int n=5;
	OPERAND temp;
	temp.data_size = data_size;
	temp.type = OPR_MEM;
	temp.addr = REG_ESP;
	temp.sreg = SREG_SS;
	while(n--) {
		printf("n = %d\n", n);
		operand_read(&temp);
		printf("esp = 0x%x, esp store = 0x%x\n", cpu.esp, temp.val);
		cpu.esp -= 4;
	}

	OPERAND old_ebp;
	old_ebp.data_size = data_size;
	old_ebp.type = OPR_MEM;
	old_ebp.addr = REG_ESP;
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






