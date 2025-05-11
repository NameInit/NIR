#include "huffman.hpp"
#include "lz77.hpp"
#include "lz78.hpp"
#include "deflate.hpp"

int main(int argc, char** argv) {
	if (argc < 2) {
		std::cout << "Использование: " << argv[0] << " <имя_файла>" << std::endl;
		return 1;
	};

	Huffman huffman(argv[1]);
	huffman.encode(argv[1], (static_cast<std::string>(argv[1])) + ".huffman");
	huffman.decode((static_cast<std::string>(argv[1]))  + ".huffman", (static_cast<std::string>(argv[1])) + ".dec");
	huffman.show_statistic(true);

	LZ77 lz77;
	lz77.encode(argv[1], (static_cast<std::string>(argv[1])) + ".lz77");
	lz77.decode((static_cast<std::string>(argv[1])) + ".lz77", (static_cast<std::string>(argv[1])) + ".dec");
	lz77.show_statistic(true);

	LZ78 lz78;
	lz78.encode(argv[1], (static_cast<std::string>(argv[1])) + ".lz78");
	lz78.decode((static_cast<std::string>(argv[1])) + ".lz78", (static_cast<std::string>(argv[1])) + ".dec");
	lz78.show_statistic(true);

	Deflate deflate(argv[1]);
	deflate.encode(static_cast<std::string>(argv[1]), static_cast<std::string>(argv[1])+".deflate");
	deflate.decode(static_cast<std::string>(argv[1])+".deflate", static_cast<std::string>(argv[1])+".dec");
	deflate.show_statistic(true);

	std::cout << std::endl;
	return 0;
}