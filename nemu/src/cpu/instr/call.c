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
	OPERAND r, rm, push_eip;
	r.data_size = data_size;
	r.type = OPR_REG;
	r.addr = modrm.val;
	cpu.edx = 1;
	operand_read(&opr_src);
	printf("opr_src.val = 0x%x, eax = 0x%x\n", opr_src.val, cpu.eax);
	print_reg();
	rm.data_size = push_eip.data_size = data_size;
	rm.type = OPR_MEM;
	rm.addr = ;
	rm.sreg = SREG_CS;
	push_eip.type = OPR_MEM;
	push_eip.sreg = SREG_SS;

	// read call address
	operand_read(&rm);
	eip += (1 + data_size / 8);
	// push eip
	cpu.esp -= 4;
	push_eip.addr = cpu.esp;
	push_eip.val = eip;
	operand_write(&push_eip);
	cpu.eip = rm.val;
	while (1)
		;
	return 0;
}

