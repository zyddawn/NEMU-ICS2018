#include "cpu/cpu.h"
#include "memory/memory.h"

// return the linear address from the virtual address and segment selector
uint32_t segment_translate(uint32_t offset, uint8_t sreg) {
	/* TODO: perform segment translation from virtual address to linear address
	 * by reading the invisible part of the segment register 'sreg'
	 */
	Assert(sreg < 6, "No such sreg.");
	Assert(cpu.segReg[sreg].ti == 0, "Selecting LDT (not implemented).");
	Assert((cpu.segReg[sreg].index<<3) < cpu.gdtr.limit, "GDTR range exceeded.");  // seg_desc is 64-bit, thus use 8*index
	Assert(offset + len < cpu.segReg[sreg].limit, "Segment range exceeded.");
	return cpu.segReg[sreg].base + offset;
}

// load the invisible part of a segment register
void load_sreg(uint8_t sreg) {
	/* TODO: load the invisibile part of the segment register 'sreg' by reading the GDT.
	 * The visible part of 'sreg' should be assigned by mov or ljmp already.
	 */

	SegDesc cur_seg = *(cpu.gdtr.base + (cpu.segReg[sreg].index << 3));
	uint32_t base = (base_31_24 << 24) | (base_23_16<< 16) | base_15_0, 
		 limit = (limit_19_16 << 16) | limit_15_0;
	cpu.segReg[sreg].base = base;
	cpu.segReg[sreg].limit = limit;
	
	// do some checking


}	
