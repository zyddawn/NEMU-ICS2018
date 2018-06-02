#include "cpu/instr.h"

make_instr_func(leave) {
#ifdef DEBUG
	printf("\nleave start...\n");
	printf("before leave cpu.eip = 0x%x, ebp = 0x%x, esp = 0x%x\n", cpu.eip, cpu.ebp, cpu.esp);
	print_reg();
#endif
	// esp = ebp
	cpu.esp = cpu.ebp;

// BUG LIES IN THE OPERAND TYPE AND OPERAND_READ !!!
#ifdef DEBUG
	OPERAND temp1, temp2;
	temp1.data_size = temp2.data_size = data_size;
	temp1.addr = REG_ESP;
	temp2.addr = REG_EBP;
	temp1.type = temp2.type = OPR_MEM;
	operand_read(&temp1);
	operand_read(&temp2);
	printf("leave ebp store = 0x%x, ebp = 0x%x, esp = 0x%x\n", temp2.val, cpu.ebp, cpu.esp);
#endif
	OPERAND old_ebp;
	old_ebp.type = OPR_MEM;
	old_ebp.addr = REG_ESP;
	operand_read(&old_ebp);
	// ebp = pop()
	cpu.ebp = old_ebp.val;
	cpu.esp += data_size / 8;
	
	return 1;
}






