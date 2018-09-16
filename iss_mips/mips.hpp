#ifndef MIPS_HPP
#define MIPS_HPP

#include <cstdint>
#include <iostream>
#include <cstdlib>
#include <string>

#define MEM_SIZE 4096

extern int32_t mem[MEM_SIZE];

extern uint32_t PC, ri, k26;

extern uint32_t opcode,
                rs,
                rt,
                rd,
                shamt,
                funct;

extern int32_t k16, hi, lo;

class Mips {
 private:
    enum OPCODES {
        EXT=0x00, LW=0x23, LB=0x20, LBU=0x24,
        LH=0x21, LHU=0x25, LUI=0x0F, SW=0x2B,
        SB=0x28, SH=0x29, BEQ=0x04, BNE=0x05,
        BLEZ=0x06, BGTZ=0x07, ADDI=0x08, SLTI=0x0A,
        SLTIU=0x0B, ANDI=0x0C, ORI=0x0D, XORI=0x0E,
        J=0x02, JAL=0x03, ADDIU=0x09
    };

    enum FUNCT {
        ADD=0x20, SUB=0x22, MULT=0x18, DIV=0x1A,
        AND=0x24, OR=0x25, XOR=0x26, NOR=0x27,
        SLT=0x2A, JR=0x08, SLL=0x00, SRL=0x02,
        SRA=0x03, SYSCALL=0x0c, MFHI=0x10, MFLO=0x12,
        ADDU=0x21, SLTU=0x2b
    };

    enum REGISTERS {
		ZERO = 0, AT = 1, V0 = 2, V1 = 3,
		A0 = 4, A1 = 5, A2 = 6, A3 = 7,
		T0 = 8, T1 = 9, T2 = 10, T3 = 11,
		T4 = 12, T5 = 13, T6 = 14, T7 = 15,
        S0 = 16, S1 = 17, S2 = 18, S3 = 19,
        S4 = 20, S5 = 21, S6 = 22, S7 = 23,
        T8 = 24, T9 = 25, K0 = 26, K1 = 27,
        GP = 28, SP = 29, FP = 30, RA = 31
	};

    void Fetch(uint32_t PC);
	void Decode();
	void Execute();
	int32_t lw(uint32_t address, int16_t kte);
    int32_t lh(uint32_t address, int16_t kte);
    uint32_t lhu(uint32_t address, int16_t kte);
    int32_t lb(uint32_t address, int16_t kte);
    uint32_t lbu(uint32_t address, int16_t kte);
    void sw(uint32_t address, int16_t kte, int32_t dado);
    void sh(uint32_t address, int16_t kte, int16_t dado);
    void sb(uint32_t address, int16_t kte, int8_t dado);

 public:
	int32_t reg[32];
	void Step();
	void Run();
    void DumpMem(uint32_t address, uint32_t size, char format);
	void DumpReg(char format);
};

#endif
