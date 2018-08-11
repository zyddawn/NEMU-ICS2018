#include "cpu/instr.h"

make_instr_func(sti) {
	printf("sti...\n");
	cpu.eflags.IF = 1;
	return 1;
}
	



