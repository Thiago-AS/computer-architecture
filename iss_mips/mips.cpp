#include "mips.hpp"

void Mips::Fetch(uint32_t PC) {
    ri = mem[PC];
    PC++;
}

void Mips::Decode() {
    opcode = ri >> 26;
    rs = (ri << 6) >> 27;
    rt = (ri << 11) >> 27;
    rd = (ri << 16) >> 27;
    shamt = (ri << 21) >> 27;
    funct = ri & 0x3F;
    k16 = (ri << 16) >> 16;
    k26 = (ri << 6 ) >> 6;
}
