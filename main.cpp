#include "huffman.hpp"
#include <thread>
#include <chrono>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Использование: " << argv[0] << " <имя_файла>" << std::endl;
        return 1;
    }

    Huffman h(argv[1]);
    Timer run_code;

    h.encode(argv[1], (static_cast<std::string>(argv[1])) + ".huff");
    h.decode((static_cast<std::string>(argv[1]))  + ".huff", (static_cast<std::string>(argv[1])) + ".dec");
    h.show_statistic();
    return 0;
}