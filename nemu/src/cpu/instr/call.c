#include "cpu/instr.h"

make_instr_func(call_near_r_v) {
	// printf("call...\n");
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
	cpu.esp -= 4;
	push_eip.addr = cpu.esp;
	push_eip.val = eip;
	operand_write(&push_eip);
	cpu.eip = eip + disp.val;
	return 0;
}

make_instr_func(call_near_rm_v) {
	OPERAND r, push_eip;
	r.data_size = data_size;
	r.type = OPR_REG;
	r.addr = instr_fetch(eip+1, 1) & 0x7;
	printf("r.addr = 0x%x\n", r.addr);
	print_reg();
	operand_read(&r);
	push_eip.data_size = data_size;
	push_eip.type = OPR_MEM;
	push_eip.sreg = SREG_SS;
	
	// read call address
	eip += (1 + data_size / 8);
	// push eip
	cpu.esp -= 4;
	push_eip.addr = cpu.esp;
	push_eip.val = eip;
	operand_write(&push_eip);
	printf("pushed eip = 0x%x, new eip = 0x%x\n", eip, r.val);
	cpu.eip = r.val;
	return 0;
}

