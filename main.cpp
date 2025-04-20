#include "huffman.hpp"
#include "lz77.hpp"
#include "lz78.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Использование: " << argv[0] << " <имя_файла>" << std::endl;
        return 1;
    }

	// LZ78 lz78;
	// lz78.encode(argv[1], (static_cast<std::string>(argv[1])) + ".lz78");
    // lz78.decode((static_cast<std::string>(argv[1])) + ".lz78", (static_cast<std::string>(argv[1])) + ".dec");
	// lz78.show_statistic();
    
    // LZ77 lz77;
    // lz77.encode(argv[1], (static_cast<std::string>(argv[1])) + ".lz77");
    // lz77.decode((static_cast<std::string>(argv[1])) + ".lz77", (static_cast<std::string>(argv[1])) + ".dec");
	// lz77.show_statistic();

    Huffman h(argv[1]);
    h.encode(argv[1], (static_cast<std::string>(argv[1])) + ".huffman");
    h.decode((static_cast<std::string>(argv[1]))  + ".huffman", (static_cast<std::string>(argv[1])) + ".dec");
    h.show_statistic();
    return 0;
}