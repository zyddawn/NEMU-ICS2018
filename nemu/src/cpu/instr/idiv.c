#include "cpu/instr.h"

make_instr_func(idiv_rm2a_b) { 
        int len = 1; 
        OPERAND opr_src, al, ah;
	opr_src.data_size = 8;
	decode_operand_rm
        operand_read(&opr_src);
	int32_t ax = cpu.eax & 0xFFFF; 
	al.data_size = ah.data_size = 8;
	al.type = ah.type = OPR_REG; 
	al.addr = REG_AL;
	ah.addr = REG_AH;
        int32_t res = alu_idiv(opr_src.val, ax, opr_src.data_size); 
	al.val = res & 0xFF;
	ah.val = res % opr_src.val; 
	operand_write(&al);
       	operand_write(&ah);	
	print_asm_1("idiv", "v", len, &opr_src); 
        return len; 
} 


make_instr_func(idiv_rm2a_v) { 
        int len = 1; 
        OPERAND opr_src, eax, edx;
	opr_src.data_size = data_size;
	decode_operand_rm
        operand_read(&opr_src);
	int64_t num, res; 
	eax.data_size = edx.data_size = data_size;
	eax.type = edx.type = OPR_REG; 
	eax.addr = REG_EAX;
	edx.addr = REG_EDX;
	if (data_size == 16) {
		num = ((cpu.edx & 0xFFFF) << 16) | (cpu.eax & 0xFFFF);
		res = alu_idiv(opr_src.val, num, data_size);
		eax.val = res & 0xFFFF;
		edx.val = num % opr_src.val; }
	else {
		num = cpu.edx;
	       	num = (num << 32) | cpu.eax;
		res = alu_idiv(opr_src.val, num, data_size);
		eax.val = res & 0xFFFFFFFF;
		edx.val = num % opr_src.val; }
	// printf("idiv: %lld / %d = %d...%d\n", num, opr_src.val, eax.val, edx.val);
	operand_write(&eax);
       	operand_write(&edx);	
	print_asm_1("idiv", "v", len, &opr_src); 
        return len; 
}


