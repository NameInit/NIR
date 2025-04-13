#include "huffman.hpp"
#include "lz77.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Использование: " << argv[0] << " <имя_файла>" << std::endl;
        return 1;
    }

    LZ77 lz(100, 100);
    lz.encode(argv[1], (static_cast<std::string>(argv[1])) + ".lz77");

    // Huffman h(argv[1]);
    // h.encode(argv[1], (static_cast<std::string>(argv[1])) + ".huff");
    // h.decode((static_cast<std::string>(argv[1]))  + ".huff", (static_cast<std::string>(argv[1])) + ".dec");
    // h.show_statistic();
    return 0;
}