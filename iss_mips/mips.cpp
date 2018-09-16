#include "mips.hpp"

void Mips::Fetch(uint32_t PC) {
    ri = mem[PC];
    PC++;
}

