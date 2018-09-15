#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <cstdint>
#include <iostream>

#define MEM_SIZE 4096

int32_t mem[MEM_SIZE];

uint32_t PC, ri;

int32_t	opcode,
		rs,
		rt,
		rd,
		shamnt,
		funct,
		k16;

uint32_t k26;

#endif
