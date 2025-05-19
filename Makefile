COMPILE=g++ -std=c++11
DEBUG=-DDEBUG
WITH_STEP=-DDEBUG_STEP
DUMP=logger a.out *.huffman *.lz* *.myalgs *.optlz* *.deflate *.dec tests/*/*.huffman tests/*/*.myalgs tests/*/*.lz* tests/*/*.optlz* tests/*/*.deflate tests/*/*.dec tests/*/*/*.huffman tests/*/*/*.myalgs tests/*/*/*.lz* tests/*/*/*.optlz* tests/*/*/*.deflate tests/*/*/*.dec
FILES=main.cpp list.hpp timer.hpp huffman.hpp lz77.hpp lz78.hpp deflate.hpp data_io.hpp

main: $(FILES)
	$(COMPILE) main.cpp

rebuild: clean main
	

run: $(FILES)
	./a.out tests/repeat_text/alf1_1mb_test

log: $(FILES)
	rm -f looger
	touch logger
	./test.sh 2 alf1 >> logger

test_logic_text:
	./test.sh 1

test_repeat_text:
	./test.sh 2 alf1

debug: $(FILES)
	$(COMPILE) $(DEBUG) main.cpp

debug_step:	$(FILES)
	$(COMPILE) $(DEBUG) $(WITH_STEP) main.cpp

memory: main
	valgrind --leak-check=full make run

clean:
	rm -rf $(DUMP)