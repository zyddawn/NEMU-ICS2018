#include "cpu/instr.h"

make_instr_func(call_near_r_v) {
#ifdef DEBUG
	printf("\ncall start...\n");
#endif
	OPERAND disp, push_eip;
	disp.data_size = push_eip.data_size = data_size;
	disp.type = OPR_IMM;
	disp.addr = eip + 1;
	disp.sreg = SREG_CS;
	push_eip.type = OPR_MEM;
	
	// read call address
	operand_read(&disp);
	eip += (1 + data_size / 8);
	// push eip
	if (data_size == 16) {
		cpu.esp -= 2;
		push_eip.val = eip & 0x0000FFFF;
		push_eip.addr = REG_SP;
		operand_write(&push_eip);
		cpu.eip = (eip + disp.val) & 0x0000FFFF;
	}
	else {
		cpu.esp -= 4;
		push_eip.val = eip;
		push_eip.addr = REG_ESP;
		operand_write(&push_eip);
		cpu.eip = eip + disp.val;
	}

#ifdef DEBUG
	OPERAND temp;
	temp.data_size = data_size;
	temp.addr = REG_ESP;
	temp.type = OPR_MEM;
	operand_read(&temp); 
	printf("call stored eip = 0x%x\n", temp.val);
	printf("call end....\n\n");
#endif
	return 0;
}



