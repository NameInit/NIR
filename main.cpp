#include <iostream>
#include <map>
#include "huffman.hpp"

int main(int argc, char** argv){
    Huffman h(argv[1]);
    h.encode(argv[1],(static_cast<std::string>(argv[1]))+".bin");
    h.decode((static_cast<std::string>(argv[1]))+".bin", (static_cast<std::string>(argv[1]))+"_dec");
    return 0;
}