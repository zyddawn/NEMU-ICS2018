#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"
#include <setjmp.h>

extern jmp_buf jbuf;

extern void vaddr_write(vaddr_t vaddr, uint8_t sreg, size_t len, uint32_t data);

#define push_helper(data) \
	cpu.esp -= 4; \
	vaddr_write(cpu.esp, SREG_SS, 4, data);

void raise_intr(uint8_t intr_no) {
#ifdef IA32_INTR
	// printf("Please implement raise_intr()");
	// assert(0);
	
	// push EFLAGS, CS, EIP
	push_helper(cpu.eflags.val)
	push_helper((uint32_t) cpu.cs.val)
	push_helper(cpu.eip)

	// Find IDT with "intr_no"
	laddr_t gd_addr = cpu.idtr.base + (intr_no << 3);
	GateDesc gd;
	gd.val[0] = laddr_t(gd_addr, 4);
	gd.val[1] = laddr_t(gd_addr + 4, 4);
	
	// clear IF if it's interrupt gate (type == 6)
	if (gd.type == 0x6)
		cpu.eflags.IF = 0;

	// set eip to the entry of interrupt handler
	uint32_t new_eip = (gd.offset_15_0 & 0xffff) | ((gd.offset_31_16 & 0xffff) << 16),
		 new_cs = gd.selector;
	cpu.eip = new_eip;  // update eip
	cpu.cs.val = new_cs;
	load_sreg(1);	// reload CS
	
	longjmp(jbuf, 1);
	// will not reach here
	assert(0);
#endif
}

void raise_sw_intr(uint8_t intr_no) {
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}
