#include "cpu/cpu.h"
#include "memory/memory.h"

#ifndef TLB_ENABLED
static uint32_t get_dir(laddr_t laddr) {
	return (laddr >> 22);  // high 10 bits
}

static uint32_t get_tb(laddr_t laddr) { 
	return (laddr >> 12) & 0x3ff;  // middle 10 bits
}

static uint32_t get_offset(laddr_t laddr) {
	return (laddr & 0xfff);   // lower 12 bits
}
#endif


// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr) {
#ifndef TLB_ENABLED
	uint32_t dir_id = get_dir(laddr),
		 tb_id = get_tb(laddr),
		 offset = get_offset(laddr);
	// fetch PageDir
	laddr_t dir_addr = (cpu.cr3.pdbr << 12) | (dir_id * 4); // PDBR + dir_index * 4
	PDE page_dir;
	page_dir.val = paddr_read(dir_addr, 4);
	if (!page_dir.present) {
		printf("PageDir present bit != 1!\n");
		assert(0);
	}
	// fetch PageTable
	laddr_t tb_addr = (page_dir.page_frame << 12) | (tb_id * 4);
	PTE page_tb;
	page_tb.val = paddr_read(tb_addr, 4);
	if (!page_tb.present) {
		printf("PageTable present bit != 1!\n");
		assert(0);
	}
	return (page_tb.page_frame << 12) | offset;
#else	
	return tlb_read(laddr) | (laddr & PAGE_MASK);;
#endif
}


