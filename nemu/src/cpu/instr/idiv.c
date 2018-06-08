#include "cpu/instr.h"

make_instr_func(idiv_rm2a_b) { 
        int len = 1; 
        OPERAND opr_src, al, ah;
	opr_src.data_size = 8;
	decode_operand_rm
        operand_read(&opr_src);
	uint32_t ax = cpu.eax & 0xFFFF; 
	al.data_size = ah.data_size = 8;
	al.type = ah.type = OPR_REG; 
	al.addr = REG_AL;
	ah.addr = REG_AH;
        uint32_t res = alu_idiv(opr_src.val, ax, opr_src.data_size); 
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
	uint64_t num, res; 
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
	printf("idiv: %lld / %d = %d...%d\n", num, opr_src.val, eax.val, edx.val);
	operand_write(&eax);
       	operand_write(&edx);	
	print_asm_1("idiv", "v", len, &opr_src); 
        return len; 
}


/* // copied from mul.c
make_instr_func(mul_rm2a_v) {
        int len = 1; // in group
        OPERAND a, rm;
        a.data_size = data_size;
        a.type = OPR_REG;
        a.addr = REG_EAX;
        rm.data_size = data_size;
        len += modrm_rm(eip + 1, &rm);
        operand_read(&a);
        operand_read(&rm);
        OPERAND ax, dx;
        dx.type = ax.type = OPR_REG;
        ax.addr = REG_AX;
        dx.addr = REG_DX;
        if(data_size == 16) {
                uint32_t res = alu_mul(rm.val, a.val, 16);
                dx.data_size = ax.data_size = 16;
                ax.val = res & 0xffff;
                dx.val = (res >> 16) & 0xffff;
		print_asm_3("mul", "w", len, &rm, &dx, &ax);
        } else { // data_size == 32
                uint64_t res = alu_mul(rm.val, a.val, 32);
                dx.data_size = ax.data_size = 32;
                ax.val = res & 0xffffffff;
                dx.val = (res >> 32) & 0xffffffff;
		print_asm_3("mul", "l", len, &rm, &dx, &ax);
        }
        operand_write(&ax);
        operand_write(&dx);
	if(dx.val == 0) {
		cpu.eflags.CF = cpu.eflags.OF = 0;
	} else {
		cpu.eflags.CF = cpu.eflags.OF = 1;
	}
        return len;

} */
