#include "mips.hpp"

/*
void menu (){
    int choice;
    uint32_t endereco = 0;
    uint32_t inicio, fim;
    char formato;

    do{
        std::cout << "-----------------------------MIPS SIMULATOR-----------------------------" << std::endl();
        std::cout << "(1) - Dump Memory" << std::endl();
        std::cout << "(2) - Dump All Memory" << std::endl();
        std::cout << "(3) - Step by Step" << std::endl();
        std::cout << "(4) - Dump Registers" << std::endl();
        std::cout << "(5) - Run" << std::endl();
        std::cout << "(0) - Exit" << std::endl();
        std::cout << "------------------------------------------------------------------------" << std::endl();

        std::cout <<("Type your choice: ")
        std::cin >> choice;

        if(choice == 1){
            std::cout <<("\n");

            std::cout <<("\nDigite o endereco inicial em hexadecimal: ex:(00000000)");
            scanf("%" SCNd32, &inicio);

            std::cout <<("\nDigite o endereco final em hexadecimal: ex: (00000001)");
            scanf("%" SCNd32, &fim);

            getchar();
            std::cout <<("\nDigite o formato da exibicao: d-para decimal e h para hexadecimal ");
            scanf("%c", &formato);

            dump_mem((inicio/4), (fim/4), formato);
        }

        else if(choice == 2){
             std::cout <<("\n");

            for(endereco = 0; endereco < MEM_SIZE; endereco++){
                if(mem[endereco] != 0){
                    printf("Address [0x%.8x] = 0x%.8x\n", endereco*4, mem[endereco]);
                }
            }
        }

        else if(choice == 3){
            printf("\nDump Step:");

            printf("\nSaida no terminal = ");
            step();

            printf("\nInstrução atual = 0x%.8x\n", ri);
            printf("opcode = 0x%.8x\n", opcode);
            printf("rs = 0x%.8x\n", rs);
            printf("rt = 0x%.8x\n", rt);
            printf("rd = 0x%.8x\n", rd);
            printf("shamt = 0x%.8x\n", shamt);
            printf("funct = 0x%.8x\n", funct);
            printf("k16 = 0x%.8x\n", k16);
            printf("k26 = 0x%.8x\n", k26);

            dump_reg('x');
        }


        else if(choice == 4){
            getchar();
            printf("\nDigite o formato da exibicao: ");
            scanf("%c", &formato);

            dump_reg(formato);
        }


        else if(choice == 5){
            pc = 0;
            run();
        }

    }while(choice != 0);
}
*/

void LoadData(FILE *file) {
    uint32_t address = 2048;

    while(!feof(file)){
        fread(&mem[address], sizeof(int32_t), 1, file);
        address++;
    }
}

void LoadText(FILE * file) {
    uint32_t address = 0;

    while(!feof(file)){
        fread(&mem[address], sizeof(int32_t), 1, file);
        address++;
    }
}

int main() {
    Mips* processor = new Mips;

    FILE *data_file;
    FILE *data_text;

    for(uint32_t address = 0; address < MEM_SIZE; address++){
        mem[address] = 0;
    }

    for(int i = 0; i < 32; i++){
        processor->reg[i] = 0;
    }

    data_file = fopen("data.bin", "rb");
    if(data_file == NULL){
        std::cout << "Error opening data file" << std::endl;
        exit(-1);
    }

    data_text = fopen("text.bin", "rb");
    if(data_text == NULL){
        std::cout << "Error opening text file" << std::endl;
        exit(-1);
    }

    LoadData(data_file);
    LoadText(data_text);

    processor->Run();
    processor->DumpReg('x');
    processor->DumpMem(0x2000,0x204c,'x');

    fclose(data_file);
    fclose(data_text);



    return 0;
}
