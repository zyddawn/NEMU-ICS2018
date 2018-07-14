#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr) {
#ifndef TLB_ENABLED
	/*TODO*/
	printf("\nPlease implement page_translate()\n");
	assert(0);
	// paddr_read()

	// assert(present_bit);  // for debugging
#else	
	return tlb_read(laddr) | (laddr & PAGE_MASK);;
#endif
}
