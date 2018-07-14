#include "cpu/cpu.h"
#include "memory/memory.h"

// return the linear address from the virtual address and segment selector
uint32_t segment_translate(uint32_t offset, uint8_t sreg) {
	/* TODO: perform segment translation from virtual address to linear address
	 * by reading the invisible part of the segment register 'sreg'
	 */
	assert(sreg < 6);
	// assert(cpu.segReg[sreg].ti == 0);
	// assert((cpu.segReg[sreg].index<<3) < cpu.gdtr.limit);  // seg_desc is 64-bit, thus use 8*index
	
	// load_sreg(sreg);
	return cpu.segReg[sreg].base + offset;
}

// load the invisible part of a segment register
void load_sreg(uint8_t sreg) {
	/* TODO: load the invisibile part of the segment register 'sreg' by reading the GDT.
	 * The visible part of 'sreg' should be assigned by mov or ljmp already.
	 */
	SegDesc cur_seg;
       	laddr_t addr = (laddr_t)cpu.gdtr.base + (cpu.segReg[sreg].index << 3);
	cur_seg.val[0] = laddr_read(addr, 4);
	cur_seg.val[1] = laddr_read(addr + 4, 4);
	
	uint32_t base = (cur_seg.base_31_24 << 24) | (cur_seg.base_23_16<< 16) | cur_seg.base_15_0, 
		 limit = (cur_seg.limit_19_16 << 16) | cur_seg.limit_15_0;
	// load invisible part
	cpu.segReg[sreg].base = base;
	cpu.segReg[sreg].limit = limit;
	cpu.segReg[sreg].type = cur_seg.type;
	cpu.segReg[sreg].privilege_level = cur_seg.privilege_level;
	cpu.segReg[sreg].soft_use = cur_seg.soft_use;

	// LATENT BUG: didn't check present bit

	// do some checking
	if (!cpu.cr0.pe) {
		assert(base == 0);
		assert(limit == 0xfffff);
		assert(cur_seg.granularity == 1);
	}
}	
