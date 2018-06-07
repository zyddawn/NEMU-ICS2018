#include "cpu/instr.h"

make_instr_func(imul_rm2r_v) {
        int len = 1; // in group
        decode_data_size_v
	decode_operand_rm2r

        operand_read(&opr_src);
        operand_read(&opr_dest);
	// zero extent
        uint64_t res = alu_imul(opr_src.val, opr_dest.val, data_size);
        opr_dest.val = res;
	operand_write(&opr_dest);
	if(data_size == 16 && ((res >> 16) & 0xFFFF) == 0) {
		cpu.eflags.CF = cpu.eflags.OF = 0;
	} 
	else if (data_size == 32 && ((res >> 32) & 0xFFFFFFFF) == 0) {
		cpu.eflags.CF = cpu.eflags.OF = 0;
	}
	else {
		cpu.eflags.CF = cpu.eflags.OF = 1;
	}
	print_asm_2("imul", "v", len, &rm, &al);
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
