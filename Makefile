COMPILE=g++ -std=c++11
DEBUG=-DDEBUG
WITH_STEP=-DDEBUG_STEP
DUMP=*.huffman *.lz* *.deflate *.dec tests/*.huffman tests/*.lz* tests/*.deflate tests/*.dec a.out

main: main.cpp list.hpp timer.hpp huffman.hpp lz77.hpp data_io.hpp
	$(COMPILE) main.cpp

rebuild: clean main
	

run:
	./a.out test1

test:
	./test.sh

debug: main.cpp list.hpp timer.hpp huffman.hpp lz77.hpp data_io.hpp
	$(COMPILE) $(DEBUG) main.cpp

debug_step:	main.cpp list.hpp timer.hpp huffman.hpp lz77.hpp data_io.hpp
	$(COMPILE) $(DEBUG) $(WITH_STEP) main.cpp

memory: main
	valgrind --leak-check=full make test

clean:
	rm -rf $(DUMP)