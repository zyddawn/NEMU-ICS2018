#include "cpu/instr.h"

make_instr_func(call_near_r_v) {
	printf("call...\n");
	OPERAND disp, push_eip;
	disp.data_size = push_eip.data_size = data_size;
	disp.type = OPR_IMM;
	disp.addr = eip + 1;
	disp.sreg = SREG_CS;
	push_eip.type = OPR_MEM;
	push_eip.sreg = SREG_SS;

	// read call address
	operand_read(&disp);
	eip += (1 + data_size / 8);
	// push eip
	/*if (data_size == 16) {
		cpu.esp -= 2;
		push_eip.val = eip & 0x0000FFFF;
		operand_write(&push_eip);
		cpu.eip = (eip + disp.val) & 0x0000FFFF;
	}
	else {*/
	cpu.esp -= 4;
	push_eip.addr = cpu.esp;
	push_eip.val = eip;
	operand_write(&push_eip);
	cpu.eip = eip + disp.val;
	printf("call pushed eip = 0x%x, current eip = 0x%x\n", push_eip.val, cpu.eip);
	print_reg();
	// }
	
	OPERAND temp;
	temp.data_size = 32;
	temp.type = OPR_MEM;
	temp.addr = cpu.esp;
	temp.sreg = SREG_SS;
	operand_read(&temp);
	printf("after call check esp = 0x%x, esp store = 0x%x\n\n", cpu.esp, temp.val);
	return 0;
}



