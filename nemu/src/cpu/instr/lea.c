#include "cpu/instr.h"

make_instr_func(lea) {
	int len = 1;
	OPERAND opr_src, opr_dest;
	decode_data_size_v
	decode_operand_rm2r
	opr_dest.val = sign_ext(opr_src.addr, data_size);
	print_reg();
	printf("m addr = 0x%x\n", opr_src.addr);
	operand_write(&opr_dest);
	print_reg();
	printf("cur eip = 0x%x, next eip = 0x%x\n", cpu.eip, len+cpu.eip);
	return len;
}





