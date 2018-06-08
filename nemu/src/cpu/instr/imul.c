#include "cpu/instr.h"

make_instr_func(imul_rm2a_b) {
	int len = 1;
	OPERAND opr_src, ax;
	opr_src.data_size = 8;
	decode_operand_rm
	operand_read(&opr_src); 
	uint32_t al = cpu.eax & 0xFF;
	ax.data_size = 16;
	ax.type = OPR_REG;
       	ax.addr = REG_AX;	
	uint64_t res = alu_imul(sign_ext(opr_src.val, 8), sign_ext(al, 8), 8); 
	ax.val = res & 0xFFFF;
	operand_write(&ax);
	if (((res >> 8) & 0xFF) == 0)
		cpu.eflags.CF = cpu.eflags.OF = 0;
	else
		cpu.eflags.CF = cpu.eflags.OF = 1;
	return len;
}


make_instr_func(imul_rm2a_v) {
	int len = 1;
	OPERAND opr_src, eax, edx;
	decode_data_size_v
	decode_operand_rm
	operand_read(&opr_src); 
	int32_t num;
	int64_t res;
	eax.data_size = edx.data_size = data_size;
	eax.type = edx.type = OPR_REG;
	eax.addr = REG_EAX;
	edx.addr = REG_EDX;
	num = cpu.eax;
       	if (data_size == 16) {
		num &= 0xFFFF;
		res = alu_imul(sign_ext(opr_src.val, 16), sign_ext(num, 16), 16);
		eax.val = res & 0xFFFF;
		edx.val = (res >> 16) & 0xFFFF; }
	else {
		res = opr_src.val * num;
		eax.val = res & 0xFFFFFFFF;
		edx.val = (res >> 32) & 0xFFFFFFFF;
	}
	printf("imul: 0x%x * 0x%x = 0x%llx, eax = 0x%x, edx = 0x%x\n", opr_src.val, num, res, eax.val, edx.val);
	operand_write(&eax);
	operand_write(&edx);
	if (data_size == 16 && ((res >> 16) & 0xFFFF) == 0)
		cpu.eflags.CF = cpu.eflags.OF = 0;
	else if (data_size == 32 && (res >> 32) == 0)
		cpu.eflags.CF = cpu.eflags.OF = 0;
	else
		cpu.eflags.CF = cpu.eflags.OF = 1;
	return len;
}



make_instr_func(imul_rm2r_v) {
        int len = 1; // in group
        decode_data_size_v
	decode_operand_rm2r

        operand_read(&opr_src);
        operand_read(&opr_dest);
	// zero extent
        uint64_t res = alu_imul(opr_src.val, opr_dest.val, data_size);

	// printf("imul_rm2r: %d * %d = %lld\n", opr_src.val, opr_dest.val, res);
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
	print_asm_2("imul", "v", len, &opr_src, &opr_dest);
        return len;

}

