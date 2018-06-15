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
	// printf("hw_mem[0x30000]: %02x %02x %02x %02x %02x %02x\n", hw_mem[0x30000], hw_mem[0x30001], hw_mem[0x30002], hw_mem[0x30003], hw_mem[0x30004], hw_mem[0x30005]);
	// printf("hw_mem[0x60000]: %02x %02x %02x %02x %02x %02x\n", hw_mem[0x60000], hw_mem[0x60001], hw_mem[0x60002], hw_mem[0x60003], hw_mem[0x60004], hw_mem[0x60005]);
	// printf("hw_mem[0x38504]: %02x %02x %02x %02x %02x %02x\n", hw_mem[0x38504], hw_mem[0x38505], hw_mem[0x38506], hw_mem[0x38507], hw_mem[0x38508], hw_mem[0x38509]);
	for(int i=0x30000; i<0x30100; ++i) {
		if(i % 16 == 0)
			printf("\n0x%08x: ", i);
		printf("%02x ", hw_mem[i]);
	}
	
	cpu.eip = r.val;
	return 0;
}

