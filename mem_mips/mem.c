#include "mem.h"

int32_t lw(uint32_t address, int16_t kte){
    if((address+kte) % 4 != 0){
        printf("Address not aligned!\n");
        return 0;
    }
    return mem[(address+kte)/4];
}

int32_t lh(uint32_t address, int16_t kte){
    int off_set;
    int32_t base_address;

    if((address+kte) % 2 != 0){
        printf("Address not aligned!\n");
        return 0;
    }

    off_set = (address+kte) % 4;
    base_address = mem[(address+kte)/4];
    base_address = (base_address << (8 * (2-off_set))) >> 16;

    return base_address;
}

uint32_t lhu(uint32_t address, int16_t kte){
    int off_set;
    uint32_t base_address;

    if((address+kte) % 2 != 0){
        printf("Address not aligned!\n");
        return 0;
    }

    off_set = (address+kte) % 4;
    base_address = mem[(address+kte)/4];
    base_address = (base_address << (8 * (2-off_set))) >> 16;

    return base_address;
}

int32_t lb(uint32_t address, int16_t kte){
    int off_set;
    int32_t base_address;
    off_set = (address+kte) % 4;
    base_address = mem[(address+kte)/4];
    base_address = (base_address << (8 * (3-off_set))) >> 24;

    return base_address;
}

uint32_t lbu(uint32_t address, int16_t kte){
    int off_set;
    uint32_t base_address;
    off_set = (address+kte) % 4;
    base_address = mem[(address+kte)/4];
    base_address = (base_address << (8 * (3-off_set))) >> 24;

    return base_address;
}

void sw(uint32_t address, int16_t kte, int32_t dado){
    if((address+kte) % 4 != 0)
        printf("Address not aligned!\n");
    else
        mem[(address+kte)/4] = dado;
}

void sh(uint32_t address, int16_t kte, int16_t dado){
    int off_set;
    int32_t data, base_address, mask;

    if((address+kte) % 2 != 0)
        printf("Address not aligned!\n");
    else{
        off_set = (address+kte) % 4;
        base_address = mem[(address+kte)/4];
        data = dado << (8 * off_set);
        mask = 0xffff << (8 * off_set);
        mem[(address+kte)/4] = (data & mask) | (base_address & (~mask));
    }
}

void sb(uint32_t address, int16_t kte, int8_t dado){
    int off_set;
    int32_t base_address, data, mask;
    off_set = (address+kte) % 4;
    base_address = mem[(address+kte)/4];
    data = dado << (8 * off_set);
    mask = 0xff << (8 * off_set);
    mem[(address+kte)/4] = (data & mask) | (base_address & (~mask));
}


void dump_mem(uint32_t address, uint32_t size){
    int i;
    if((address+size) % 4 != 0)
        printf("Address not aligned!\n");
    else{
        for(i = address/4; i < address/4+size/4; i++){
            printf("mem[%d]: = %08x \n", i, mem[i]);
        }
    }
}

