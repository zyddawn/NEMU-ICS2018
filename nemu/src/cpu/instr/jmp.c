#include "cpu/instr.h"
#include "memory/mmu/segment.h"

make_instr_func(jmp_near_rm) {
	int len = 0;
	decode_data_size_v
	decode_operand_rm
	operand_read(&opr_src);
	if (data_size == 16)
		cpu.eip = opr_src.val & 0xFFFF;
	else
		cpu.eip = opr_src.val;
	return 0;
}


make_instr_func(jmp_near) {
        OPERAND rel;
        rel.type = OPR_IMM;
	rel.sreg = SREG_CS;
        rel.data_size = data_size;
        rel.addr = eip + 1;

        operand_read(&rel);

	int offset = sign_ext(rel.val, data_size);
	print_asm_1("jmp", "",  1 + data_size / 8, &rel);

	cpu.eip += offset;

        // return 1 + data_size / 8;
	return 0;
}

make_instr_func(jmp_short_) {
        OPERAND rel;
        rel.type = OPR_IMM;
	rel.sreg = SREG_CS;
        rel.data_size = 8;
        rel.addr = eip + 1;
        operand_read(&rel);

	int offset = sign_ext(rel.val, 8);
	// print_asm_1("jmp", "",  1 + data_size / 8, &rel);
	// char offset = rel.val & 0xFF;  // signed 8-bit
	// printf("Before jmp, eip = 0x%x (%d), disp = 0x%x (%d), re.val = 0x%x\n", cpu.eip, cpu.eip, offset, offset, rel.val);
	cpu.eip += offset;  
	// printf("After jmp, eip = 0x%x (%d)\n", cpu.eip, cpu.eip);
        return 2;
}



make_instr_func(ljmp) {
	printf("Calling ljmp.\n");
	uint32_t new_eip = instr_fetch(eip + 1, 4);
	printf("new_eip = 0x%x\n", new_eip);
	uint16_t new_val = instr_fetch(eip + 5, 2);
	printf("new_val = 0x%x\n", new_val);
	cpu.cs.val = new_val;
	load_sreg(1);  // cs register
	cpu.eip = new_eip;	
	return 1;
}









