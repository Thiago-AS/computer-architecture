#include "mem.h"

int main()
{
    int i;
    /*
    Caso teste 1: Escrever bytes na posicao de memoria mem[0] com seus
    respectivos offsets
    */
    sb(0, 0, 0x04);
    sb(0, 1, 0x03);
    sb(0, 2, 0x02);
    sb(0, 3, 0x01);
    /*
    Caso teste 2: Escrever bytes na posicao de memoria mem[1] com seus
    respectivos offsets
    */
    sb(4, 0, 0xFF);
    sb(4, 1, 0xFE);
    sb(4, 2, 0xFD);
    sb(4, 3, 0xFC);
    /*
    Caso teste 3: Escrever meia palavras na posicao de memoria mem[2] com seus
    respectivos offsets
    */
    sh(8, 0, 0xFFF0);
    sh(8, 2, 0x8C);
    /*
    Caso teste 4: Escrever palavras nas posicoes de memoria mem[3], mem[4],
    mem[5] e mem[6]
    */
    sw(12, 0, 0xff);
    sw(16, 0, 0xFFFF);
    sw(20, 0, 0xFFFFFFFF);
    sw(24, 0, 0x80000000);
    /*
    Depurar casos testes 1 a 4: Imprimir memoria do 0 ao 6
    */
    printf("Casos testes 1 a 4: \n");
    dump_mem(0,28);
    printf("\n");
    /*
    Caso teste 5: Ler bytes com sinal nas posicoes de memoria mem[0] com seus
    respectivos offsets
    */
    printf("Caso teste 5: \n");
    for(i=0; i < 4; i++){
        printf("lb(0,%d) - Hex: %02x Dec: %d\n", i, 0xff & lb(0,i), lb(0,i));
    }
    printf("\n");
    /*
    Caso teste 6: Ler bytes com sinal nas posicoes de memoria mem[1] com seus
    respectivos offsets
    */
    printf("Caso teste 6: \n");
    for(i=0; i < 4; i++){
        printf("lb(0,%d) - Hex: %02x Dec: %d\n", i, 0xff & lb(4,i), lb(4,i));
    }
    printf("\n");
    /*
    Caso teste 7: Ler bytes sem sinal nas posicoes de memoria mem[1] com seus
    respectivos offsets
    */
    printf("Caso teste 7: \n");
    for(i=0; i < 4; i++){
        printf("lbu(0,%d) - Hex: %02x Dec: %d\n", i, 0xff & lbu(4,i), lbu(4,i));
    }
    printf("\n");
    /*
    Caso teste 8: Ler meias palavras com sinal nas posicoes de memoria mem[2]
    com seus respectivos offsets
    */
    printf("Caso teste 8: \n");
    for(i=0; i <= 2; i = i+2){
        printf("lh(8,%d) - Hex: %04x Dec: %d\n", i, 0xffff & lh(8,i), lh(8,i));
    }
    printf("\n");
    /*
    Caso teste 9: Ler meias palavras sem sinal nas posicoes de memoria mem[2]
    com seus respectivos offsets
    */
    printf("Caso teste 9: \n");
    for(i=0; i <= 2; i = i+2){
        printf("lhu(8,%d) - Hex: %04x Dec: %d\n", i, 0xffff & lhu(8,i), lhu(8,i));
    }
    printf("\n");
    /*
    Caso teste 10: Ler palavras com sinal nas posicoes de memoria mem[3],
    mem[4] e mem[5]
    */
    printf("Caso teste 10: \n");
    for(i=12; i <= 20; i = i+4){
        printf("lw(%d,0) - Hex: %08x Dec: %d\n", i, lw(i,0), lw(i,0));
    }
    printf("\n");
    /*
    Caso teste 11: Erro de alinhamento ao ler palavra
    */
    printf("Caso teste 11: \n");
    lw(0,2);
    printf("\n");
    /*
    Caso teste 12: Erro de alinhamento ao ler meia palavra com sinal
    */
    printf("Caso teste 12: \n");
    lh(0,1);
    printf("\n");
    /*
    Caso teste 13: Erro de alinhamento ao ler meia palavra sem sinal
    */
    printf("Caso teste 13: \n");
    lhu(0,1);
    printf("\n");
    /*
    Caso teste 14: Erro de alinhamento ao escrever palavra
    */
    printf("Caso teste 14: \n");
    sw(0,2,0x02);
    printf("\n");
    /*
    Caso teste 15: Erro de alinhamento ao escrever meia palavra
    */
    printf("Caso teste 15: \n");
    sw(0,1,0x02);
    printf("\n");
    /*
    Caso teste 16: Sobrescrever palavra na posicao mem[6]
    */
    sw(24,0,0xf040201C);
    /*
    Caso teste 17: Sobrescrever meia palavra na posicao mem[5] na metade mais
    significativa
    */
    sh(20,2,0xD430);
    /*
    Caso teste 18: Sobrescrever byte na posicao mem[4] no offset 2
    */
    sh(16,2,0x20);
    /*
    Depurar casos testes 16 a 18: Imprimir memoria do 4 ao 6
    */
    printf("Casos testes 16 a 18: \n");
    dump_mem(16,12);
    return 0;
}
