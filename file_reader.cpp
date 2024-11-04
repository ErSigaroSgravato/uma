#include "file_reader.hpp"

std::string read_file_to_string(const char *filename){
    std::ifstream file_reader(filename, std::ios::binary);
    if(!file_reader.is_open()){
        printf("ERROR: could not open file %s\n", filename);
        return "";
    }

    std::string content((std::istreambuf_iterator<char>(file_reader)),
                        std::istreambuf_iterator<char>());
    file_reader.close();

    return content;
}
