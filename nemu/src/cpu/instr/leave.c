#include "cpu/instr.h"

make_instr_func(leave) {
	// esp = ebp
	// cpu.esp = cpu.ebp;
	print_reg();
	
	int n=5;
	OPERAND temp, change_esp;
	temp.data_size = change_esp.data_size = data_size;
	temp.type = OPR_MEM;
	change_esp.type = OPR_REG;
	temp.addr = REG_ESP;
	change_esp.addr = REG_ESP;
	temp.sreg = SREG_SS;
	temp.sreg = SREG_DS;
	while(n--) {
		printf("n = %d\n", n);
		operand_read(&temp);
		printf("esp = 0x%x, esp store = 0x%x\n", cpu.esp, temp.val);
		operand_read(&change_esp);
		change_esp.val -= 4;
		operand_write(&change_esp);
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






