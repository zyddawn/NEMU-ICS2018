#include "cpu/instr.h"

make_instr_func(leave) {
	printf("before leave: \n");
	print_reg();
	OPERAND temp;
	temp.data_size = data_size;
	temp.addr = REG_EBP;
	temp.type = OPR_MEM;
	operand_read(&temp);
	printf("current ebp store = 0x%x\n", temp.val);

	// esp = ebp
	cpu.esp = cpu.ebp;
	print_reg();

	OPERAND old_ebp;
	old_ebp.data_size = data_size;
	old_ebp.type = OPR_MEM;
	old_ebp.addr = REG_ESP;
	old_ebp.sreg = SREG_SS;
	operand_read(&old_ebp);
	printf("esp = 0x%x, esp value = 0x%x\n", cpu.esp, old_ebp.val);
	
	OPERAND tmps, tmpb;
	tmps.data_size = tmpb.data_size = data_size;
	tmps.type = tmpb.type = OPR_MEM;
	tmps.addr = REG_ESP;
	tmpb.addr = REG_EBP;
	operand_read(&tmps);
	operand_read(&tmpb);
	printf("ebp store = 0x%x, esp store = 0x%x\n\n", tmpb.val, tmps.val);


	// ebp = pop()
        cpu.ebp = old_ebp.val;
        cpu.esp += data_size / 8;
	
	printf("After leave: \n");
	print_reg();
	while(1)
		;
	return 1;
}






