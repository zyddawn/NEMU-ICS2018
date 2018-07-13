#include "cpu/cpu.h"
#include "memory/memory.h"

// return the linear address from the virtual address and segment selector
uint32_t segment_translate(uint32_t offset, uint8_t sreg) {
	/* TODO: perform segment translation from virtual address to linear address
	 * by reading the invisible part of the segment register 'sreg'
	 */
	// printf("Segment translate: \n");
	// printf("sreg = 0x%x\n", sreg);
	assert(sreg < 6);
	// printf("ti = %d\n", cpu.segReg[sreg].ti);
	// assert(cpu.segReg[sreg].ti == 0);
	printf("index = 0x%x\n", cpu.segReg[sreg].index);
	assert((cpu.segReg[sreg].index<<3) < cpu.gdtr.limit);  // seg_desc is 64-bit, thus use 8*index
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
	uint32_t *mem_read = (uint32_t *)&cur_seg;
	*mem_read = laddr_read(addr, 4);
	
	// if(!cur_seg.present) { // not yet in main memory, need to load
	uint32_t base = (cur_seg.base_31_24 << 24) | (cur_seg.base_23_16<< 16) | cur_seg.base_15_0, 
		 limit = (cur_seg.limit_19_16 << 16) | cur_seg.limit_15_0;
	// load invisible part
	cpu.segReg[sreg].base = base;
	cpu.segReg[sreg].limit = limit;
	cpu.segReg[sreg].type = cur_seg.type;
	cpu.segReg[sreg].privilege_level = cur_seg.privilege_level;
	cpu.segReg[sreg].soft_use = cur_seg.soft_use;
	// }	

	// do some checking
	if (!cpu.cr0.pe) {
		assert(base == 0);
		assert(limit == 0xfffff);
		assert(cur_seg.granularity == 1);
	}
}	
