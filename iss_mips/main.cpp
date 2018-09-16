#include "mips.hpp"

void menu (){

}

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

    int choice;
    uint32_t start, end;
    char format;

    do{
        std::cout << "-----------------------------MIPS SIMULATOR-----------------------------" << std::endl;
        std::cout << "(1) - Dump Memory" << std::endl;
        std::cout << "(2) - Dump All Memory" << std::endl;
        std::cout << "(3) - Dump Registers" << std::endl;
        std::cout << "(4) - Run" << std::endl;
        std::cout << "(0) - Exit" << std::endl;
        std::cout << "------------------------------------------------------------------------" << std::endl;

        std::cout <<("Type your choice: ");
        std::cin >> choice;

        switch(choice) {
        case 1:
            std::cout << std::endl;
            std::cout << "Type the initial address in hexadecimal (ex: 2000): ";
            std::cin >> std::hex >> start;
            std::cout << std::endl;
            std::cout << "Type the end address in hexadecimal (ex: 204c): ";
            std::cin >> std::hex >> end;
            std::cout << std::endl;
            std::cout << "Type the display format (d = decimal, h = hexadecimal): ";
            std::cin >> format;
            std::cout << std::endl;
            processor->DumpMem(start, end, format);
            break;

        case 2:
            std::cout << std::endl;
            for(uint32_t i = 0; i <= MEM_SIZE; i++){
                std::cout << "Address [" << std::hex << i*4 << "] = " << std::setfill('0') << std::setw(8) << std::hex << mem[i] << std::endl;
            }
            break;

        case 3:
            std::cout << std::endl;
            std::cout << "Type the display format (d = decimal, h = hexadecimal): ";
            std::cin >> format;
            std::cout << std::endl;
            processor->DumpReg(format);
            break;

        case 4:
            std::cout << std::endl;
            processor->Run();
            std::cout << std::endl;
            break;

        case 0:
            break;

        default:
            std::cout << "Type one of the options above" << std::endl;
            break;
        }


    }while(choice != 0);

    fclose(data_file);
    fclose(data_text);

    return 0;
}
