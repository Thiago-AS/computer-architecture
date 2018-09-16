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
            pc = reg[rs];
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

            case 4:
                uint32_t address = reg[A0]/4;

                char *caracter;

                caracter = (char *)&(mem[endereco]);

                caracter += reg[4] % 4;
                do{
                    printf("%c", *caracter);
                    caracter++;
                }while(*caracter != '\0');
                break;

            case 10:
                std::cout << "Program finished running" << std::endl;
                DumpReg();
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
            pc = pc + k16;
        }
        break;

    case BNE:
        if(reg[rs] != reg[rt]) {
            pc = pc + k16;
        }
        break;

    case BLEZ:
        if(reg[rs] <= 0) {
            pc = pc + k16;
        }
        break;

    case BGTZ:
        if(reg[rs] > 0){
            pc = pc + k16;
        }
        break;

    case ADDI:
        reg[rt] = reg[rs] + k16;
        break;

    case SLTI:
        reg[rt] = (reg[rs] < k16) ? 1 : 0;
        break;

    case SLTIU:
        reg[rt] = (reg[rs] < (uint32_t)k16) ? 1 : 0;
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
        pc = k26;
        break;

    case JAL:
        reg[RA] = pc;
        pc = k26;
        break;

    default:
        break;
    }
}

void Mips::Step() {
    Fetch(pc);
    Decode();
    Execute();
}

void run() {
    uint32_t cnt = 0;
    pc = 0;
    while(cnt < MEM_SIZE) {
        reg[ZERO] = 0;
        step();
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
