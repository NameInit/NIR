COMPILE=g++ -std=c++11
DEBUG=-DDEBUG
WITH_STEP=-DDEBUG_STEP
DUMP=a.out *.huffman *.lz* *.deflate *.dec tests/*/*.huffman tests/*/*.lz* tests/*/*.deflate tests/*/*.dec tests/*/*/*.huffman tests/*/*/*.lz* tests/*/*/*.deflate tests/*/*/*.dec

main: main.cpp list.hpp timer.hpp huffman.hpp lz77.hpp lz78.hpp deflate.hpp data_io.hpp
	$(COMPILE) main.cpp

rebuild: clean main
	

run:
	./a.out tests/repeat_text/alf1_1mb_test

test_logic_text:
	./test.sh 1

test_repeat_text:
	./test.sh 2 alf1

debug: main.cpp list.hpp timer.hpp huffman.hpp lz77.hpp lz78.hpp deflate.hpp data_io.hpp
	$(COMPILE) $(DEBUG) main.cpp

debug_step:	main.cpp list.hpp timer.hpp huffman.hpp lz77.hpp lz78.hpp deflate.hpp data_io.hpp
	$(COMPILE) $(DEBUG) $(WITH_STEP) main.cpp

memory: main
	valgrind --leak-check=full make run

clean:
	rm -rf $(DUMP)