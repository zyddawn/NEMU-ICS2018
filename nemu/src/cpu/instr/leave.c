#include "cpu/instr.h"

make_instr_func(leave) {
	OPERAND opr_esp, opr_ebp;
	opr_esp.data_size = opr_ebp.data_size = data_size;
	opr_esp.type = opr_ebp.type = OPR_REG;
	if (data_size == 16) {
		opr_esp.addr = REG_SP;
		opr_ebp.addr = REG_BP;
	}
	else {
		opr_esp.addr = REG_ESP;
		opr_ebp.addr = REG_EBP;
	}
	// esp = ebp
	operand_read(&opr_ebp);
	opr_esp.val = opr_ebp.val;

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
	operand_write(&opr_esp);
	// ebp = pop()
	opr_esp.type = OPR_MEM;
	operand_read(&opr_esp);
	cpu.ebp = opr_esp.val;
	cpu.esp += data_size / 8;
	// print_reg();
	printf("\n");
	return 1;
}






