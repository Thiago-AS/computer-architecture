#include "mips.hpp"

void LoadData(FILE *file){
    uint32_t address = 2048;

    while(!feof(file)){
        fread(&mem[address], sizeof(int32_t), 1, file);
        address++;
    }
}

void LoadText(FILE * file){
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

    return 0;
}
