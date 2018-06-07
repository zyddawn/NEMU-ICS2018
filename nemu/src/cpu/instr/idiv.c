#include "cpu/instr.h"

#define idiv_helper(suffix) \
	make_instr_func(concat(idiv_rm2a_, suffix)) { \
        	int len = 1; \
        	concat(decode_data_size_, suffix) \
		decode_operand_rm \
        	operand_read(&opr_src); \
        	uint32_t res = alu_idiv(opr_src.val, cpu.eax, data_size); \
		cpu.eax = res; \
		print_asm_1("idiv", "v", len, &opr_src); \
        	return len; \
	} \

idiv_helper(v)
idiv_helper(b)




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
