#include "mips.hpp"

int32_t mem[MEM_SIZE];

uint32_t PC, ri, k26;

uint32_t opcode,
                rs,
                rt,
                rd,
                shamt,
                funct;

int32_t k16, hi, lo;

int32_t Mips::lw(uint32_t address, int16_t kte){
    if((address+kte) % 4 != 0){
        std::cout << "Address not aligned!" << std::endl;
        return 0;
    }
    return mem[(address+kte)/4];
}

int32_t Mips::lh(uint32_t address, int16_t kte){
    int off_set;
    int32_t base_address;

    if((address+kte) % 2 != 0){
        std::cout << "Address not aligned!" << std::endl;
        return 0;
    }

    off_set = (address+kte) % 4;
    base_address = mem[(address+kte)/4];
    base_address = (base_address << (8 * (2-off_set))) >> 16;

    return base_address;
}

uint32_t Mips::lhu(uint32_t address, int16_t kte){
    int off_set;
    uint32_t base_address;

    if((address+kte) % 2 != 0){
        std::cout << "Address not aligned!" << std::endl;
        return 0;
    }

    off_set = (address+kte) % 4;
    base_address = mem[(address+kte)/4];
    base_address = (base_address << (8 * (2-off_set))) >> 16;

    return base_address;
}

int32_t Mips::lb(uint32_t address, int16_t kte){
    int off_set;
    int32_t base_address;
    off_set = (address+kte) % 4;
    base_address = mem[(address+kte)/4];
    base_address = (base_address << (8 * (3-off_set))) >> 24;

    return base_address;
}

uint32_t Mips::lbu(uint32_t address, int16_t kte){
    int off_set;
    uint32_t base_address;
    off_set = (address+kte) % 4;
    base_address = mem[(address+kte)/4];
    base_address = (base_address << (8 * (3-off_set))) >> 24;

    return base_address;
}

void Mips::sw(uint32_t address, int16_t kte, int32_t dado){
    if((address+kte) % 4 != 0)
        std::cout << "Address not aligned!" << std::endl;
    else
        mem[(address+kte)/4] = dado;
}

void Mips::sh(uint32_t address, int16_t kte, int16_t dado){
    int off_set;
    int32_t data, base_address, mask;

    if((address+kte) % 2 != 0)
        std::cout << "Address not aligned!" << std::endl;
    else{
        off_set = (address+kte) % 4;
        base_address = mem[(address+kte)/4];
        data = dado << (8 * off_set);
        mask = 0xffff << (8 * off_set);
        mem[(address+kte)/4] = (data & mask) | (base_address & (~mask));
    }
}

void Mips::sb(uint32_t address, int16_t kte, int8_t dado){
    int off_set;
    int32_t base_address, data, mask;
    off_set = (address+kte) % 4;
    base_address = mem[(address+kte)/4];
    data = dado << (8 * off_set);
    mask = 0xff << (8 * off_set);
    mem[(address+kte)/4] = (data & mask) | (base_address & (~mask));
}

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

void Mips::Execute() {
    switch(opcode) {
    case EXT:
        switch(funct) {
        case ADD:
            reg[rd] = reg[rs] + reg[rt];
            break;

        case SUB:
            reg[rd] = reg[rs] - reg[rt];
            break;

        case MULT:
            int64_t temp;
            temp = reg[rs] * reg[rt];
            hi = (temp & 0x1111111100000000) >> 32;
            lo = temp & 0x0000000011111111;
            break;

        case DIV:
            lo = reg[rs] / reg[rt];
            hi = reg[rs] % reg[rt];
            break;

        case AND:
            reg[rd] = reg[rs] & reg[rt];
            break;

        case OR:
            reg[rd] = reg[rs] | reg[rt];
            break;

        case XOR:
            reg[rd] = reg[rs] ^ reg[rt];
            break;

        case NOR:
            reg[rd] = ~(reg[rs] ^ reg[rt]);
            break;

        case SLT:
            reg[rd] = (reg[rs] < reg[rt]) ? 1 : 0;
            break;

        case JR:
            PC = reg[rs];
            break;

        case SLL:
            reg[rd] = reg[rt] << shamt;
            break;

        case SRL:
            reg[rd] = (uint32_t)reg[rt] >> shamt;
            break;

        case SRA:
            reg[rd] = reg[rt] >> shamt;
            break;

        case SYSCALL:
            switch(reg[V0]) {
            case 1:
                std::cout << reg[A0] << std::endl;
                break;

            case 4:{
                uint32_t addr = reg[A0]/4;

                char *caracter;

                caracter = (char *)&(mem[addr]);

                caracter += reg[4] % 4;
                do{
                    printf("%c", *caracter);
                    caracter++;
                }while(*caracter != '\0');
                break;
            }

            case 10:
                std::cout << "Program finished running" << std::endl;
                DumpReg('h');
                exit(EXIT_SUCCESS);
                break;

            default:
                break;
            }

        case MFHI:
            reg[rd] = hi;
            break;

        case MFLO:
            reg[rd] = lo;
            break;

        case ADDU:
            reg[rd] = (uint32_t)reg[rs] + (uint32_t)reg[rt];
            break;

        case SLTU:
            reg[rd] = ((uint32_t)reg[rs] < (uint32_t)reg[rt]) ? 1 : 0;
            break;

        default:
            break;
        }

    case LW:
        reg[rt] = lw(reg[rs], k16);
        break;

    case LB:
        reg[rt] = lb(reg[rs], k16);
        break;

    case LBU:
        reg[rt] = lbu(reg[rs], k16);
        break;

    case LH:
        reg[rt] = lh(reg[rs], k16);
        break;

    case LHU:
        reg[rt] = lhu(reg[rs], k16);
        break;

    case LUI:
        reg[rt] = k16 << 16;
        break;

    case SW:
        sw(reg[rs],k16,reg[rt]);
        break;

    case SB:
        sb(reg[rs],k16,reg[rt]);
        break;

    case SH:
        sh(reg[rs],k16,reg[rt]);
        break;

    case BEQ:
        if(reg[rs] == reg[rt]) {
            PC = PC + k16;
        }
        break;

    case BNE:
        if(reg[rs] != reg[rt]) {
            PC = PC + k16;
        }
        break;

    case BLEZ:
        if(reg[rs] <= 0) {
            PC = PC + k16;
        }
        break;

    case BGTZ:
        if(reg[rs] > 0){
            PC = PC + k16;
        }
        break;

    case ADDI:
        reg[rt] = reg[rs] + k16;
        break;

    case SLTI:
        reg[rt] = (reg[rs] < k16) ? 1 : 0;
        break;

    case SLTIU:
        reg[rt] = ((uint32_t)reg[rs] < (uint32_t)k16) ? 1 : 0;
        break;

    case ANDI:
        reg[rd] = reg[rs] & k16;
        break;

    case ORI:
        reg[rt] = reg[rs] | k16;
        break;

    case XORI:
        reg[rt] = reg[rs] ^ k16;
        break;

    case ADDIU:
        reg[rt] = reg[rs] + k16;
        break;

    case J:
        PC = k26;
        break;

    case JAL:
        reg[RA] = PC;
        PC = k26;
        break;

    default:
        break;
    }
}

void Mips::Step() {
    Fetch(PC);
    Decode();
    Execute();
}

void Mips::Run() {
    uint32_t cnt = 0;
    PC = 0;
    while(cnt < MEM_SIZE) {
        reg[ZERO] = 0;
        Step();
        cnt++;
    }
}

void Mips::DumpReg(char format) {
    if(format == 'd'){
        std::cout << "Dump Registers: " << std::endl;

        for(int i = 0; i < 32; i++){
            if(i == 31)
                std::cout << "reg[" << i << "] = " << reg[i]*4 << std::endl;
            else
                std::cout << "reg[" << i << "] = " << reg[i] << std::endl;
        }
        std::cout << "pc = " << PC*4 << std::endl;
        std::cout << "hi = " << hi << std::endl;
        std::cout << "lo = " << lo << std::endl;
    }
    else{
        std::cout << "Dump Registers: " << std::endl;

        for(int i = 0; i < 32; i++){
            if(i == 31)
                std::cout << "reg[" << i << "] = " << std::hex << reg[i]*4 << std::endl;
            else
                std::cout << "reg[" << i << "] = " << std::hex << reg[i] << std::endl;
        }
        std::cout << "pc = " << std::hex <<  PC*4 << std::endl;
        std::cout << "hi = " << std::hex << hi << std::endl;
        std::cout << "lo = " << std::hex << lo << std::endl;
    }
}

void Mips::DumpMem(uint32_t start, uint32_t end, char format) {
    std::cout << "Dump Memory: " << std::endl;
    for(uint32_t i = start; i <= end; i++){
        if(format == 'd')
            std::cout << "Address [" << std::hex << i*4 << "] = " << mem[i] << std::endl;
        else
            std::cout << "Address [" << std::hex << i*4 << "] = " << std::hex << mem[i] << std::endl;
    }
}
