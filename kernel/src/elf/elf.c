#include "common.h"
#include "memory.h"
#include "string.h"
#include "x86/memory.h"

#include <stdio.h>
#include <elf.h>

#ifdef HAS_DEVICE_IDE
#define ELF_OFFSET_IN_DISK 0
#endif

#define STACK_SIZE (1 << 20)

void ide_read(uint8_t *, uint32_t, uint32_t);
void create_video_mapping();
uint32_t get_ucr3();

uint32_t loader() {	
	Elf32_Ehdr *elf;
	Elf32_Phdr *ph, *eph;

#ifdef HAS_DEVICE_IDE
	uint8_t buf[4096];
	ide_read(buf, ELF_OFFSET_IN_DISK, 4096);
	elf = (void*)buf;
	Log("ELF loading from hard disk.");
#else
	elf = (void *)0x0;
	Log("ELF loading from ram disk.");
#endif

	/* Load each program segment */
	ph = (void *)elf + elf->e_phoff;   	// program header
	eph = ph + elf->e_phnum;		// end of program header
	for(; ph < eph; ph ++) {
		if(ph->p_type == PT_LOAD) {
			Log("PT Load:\n");
#ifndef IA32_PAGE			
			// copy the segment from the ELF file to its proper memory area
			memcpy((void *)ph->p_vaddr, (void *)ph->p_offset, ph->p_filesz);	
			// zeror the memory area [vaddr + file_sz, vaddr + mem_sz)
			memset((void *)(ph->p_vaddr + ph->p_filesz), 0, ph->p_memsz - ph->p_filesz);
#else
			uint32_t vaddr = ph->p_vaddr, 
				 offset = 0x0, 
				 paddr = 0x0,
				 total_loaded = 0;
		       	while(vaddr < (ph->p_vaddr + ph->p_memsz)) {
				offset = vaddr & 0xfff;
				vaddr &= 0xfffff000;
				paddr = mm_malloc(vaddr, PAGE_SIZE);
				printf("vaddr = 0x%x, paddr = 0x%x\n", vaddr, paddr);
				uint32_t cur_pg_load = PAGE_SIZE - offset;
				if (cur_pg_load > (ph->p_filesz - total_loaded))  // no need to load from offset to the end of the page
					cur_pg_load = ph->p_filesz - total_loaded;  // load till reaching the filesz
				memset((void *)paddr, 0, PAGE_SIZE);
				memcpy((void *)paddr, (void *)(ph->p_offset + total_loaded), cur_pg_load);
				vaddr += PAGE_SIZE;   // page size = 4096 Bytes
				total_loaded += cur_pg_load;
			}	
#endif

#ifdef IA32_PAGE
			/* Record the program break for future use */
			extern uint32_t brk;
			uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
			if(brk < new_brk) { brk = new_brk; }
#endif
		}
	}

	volatile uint32_t entry = elf->e_entry;
#ifdef IA32_PAGE
	mm_malloc(KOFFSET - STACK_SIZE, STACK_SIZE);
#ifdef HAS_DEVICE_VGA
	create_video_mapping();
#endif
	write_cr3(get_ucr3());
#endif
	return entry;
}
