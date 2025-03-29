#include <iostream>
#include <map>
#include "huffman.hpp"

int main(){
    Huffman h("test1");
    h.encode("test1","test1.bin");
    h.decode("test1.bin", "test1_dec");
    return 0;
}