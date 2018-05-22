#include "cpu/instr.h"

make_instr_func(call_near_r_v) {
	OPERAND disp, store_eip;
	disp.data_size = store_eip.data_size = data_size;
	disp.type = OPR_IMM;
	disp.addr = eip + 1;
	disp.sreg = SREG_CS;

	// read call address
	operand_read(&disp);
	eip += (1 + data_size / 8);
	store_eip.val = eip;
	// push eip
	if (data_size == 16) {
		cpu.esp -= 2;
		store_eip.addr = REG_SP;
		operand_write(&store_eip);
	}
	cpu.eip = eip + disp.val;

	return 0;
}





















