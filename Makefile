COMPILE=g++ -std=c++11
DEBUG=-DDEBUG
WITH_STEP=-DDEBUG_STEP

main: main.cpp list.hpp timer.hpp huffman.hpp lz77.hpp data_io.hpp
	$(COMPILE) main.cpp

run:
	./a.out test1

test:
	./test.sh

debug: main.cpp list.hpp timer.hpp huffman.hpp lz77.hpp data_io.hpp
	$(COMPILE) $(DEBUG) main.cpp

debug_step:	main.cpp list.hpp timer.hpp huffman.hpp lz77.hpp data_io.hpp
	$(COMPILE) $(DEBUG) $(WITH_STEP) main.cpp

memory: main
	valgrind --leak-check=full ./a.out test1

clean:
	rm -rf *.dec *.huff *.lz77 a.out tests/*.dec tests/*.huff tests/*.lz77