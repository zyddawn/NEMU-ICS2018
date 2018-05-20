#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "nemu.h"
#include "cpu/cpu.h"
#include "cpu/fpu.h"
#include "cpu/modrm.h"
#include "cpu/operand.h"
#include "cpu/instr_helper.h"
#include "memory/memory.h"

#include <stdio.h>
 
extern uint8_t data_size;

#include "cpu/instr/mov.h"
#include "cpu/instr/jmp.h"
#include "cpu/instr/shift.h"
#include "cpu/instr/flags.h"
#include "cpu/instr/group.h"
#include "cpu/instr/special.h"
#include "cpu/instr/x87.h"

/* TODO: add more instructions here */
#define mov_r2rm_b make_instr_impl_2op(mov, r, rm, b)
#define mov_r2rm_v make_instr_impl_2op(mov, r, rm ,v)
#define mov_rm2r_b make_instr_impl_2op(mov, rm, r, b)
#define mov_rm2r_v make_instr_impl_2op(mov, rm, r, v)





#endif
