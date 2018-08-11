#include "cpu/instr.h"
#include "cpu/intr.h"

extern void raise_intr(uint8_t intr_no);

make_instr_func(int_i_b) {
	uint32_t intr_no = instr_fetch(cpu.eip + 1, 1);
	cpu.eip += 2;
	// react to exception & interruption
	raise_intr(intr_no);

	// it will not reach here
	assert(0);
	return 0;
}




