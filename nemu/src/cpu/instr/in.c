#include "cpu/instr.h"
#include "device/port_io.h"

uint32_t pio_read(uint16_t port, size_t len);


make_instr_func(in_b) {
	cpu.eax = (uint8_t) pio_read((uint16_t)cpu.edx, 1);
	return 1;
}


make_instr_func(in_v) {
	cpu.eax = pio_read((uint16_t)cpu.edx, 4);
	return 1;
}

