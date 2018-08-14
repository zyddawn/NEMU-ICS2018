#include "cpu/instr.h"
#include "device/port_io.h"

void pio_write(uint16_t port, size_t len, uint32_t data);

make_instr_func(out_b) {
	pio_write((uint16_t)(cpu.edx & 0xffff), 1, (uint32_t)(cpu.eax & 0xff));
	return 1;
}


make_instr_func(out_v) {
	uint32_t temp = 0xffffffff;
	if(data_size == 16)
		temp &= 0xffff;
	pio_write((uint16_t)(cpu.edx & 0xffff), 4, (uint32_t)(cpu.eax & temp));
	return 1;
}

