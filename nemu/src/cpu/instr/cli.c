#include "cpu/instr.h"


make_instr_func(cli) {
	printf("cli...\n");
	cpu.eflags.IF = 0;
	return 1;
}

