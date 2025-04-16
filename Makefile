main:
	g++ -std=c++11 main.cpp

run: main
	./a.out test1

make test: main
	./test.sh

memory:
	valgrind --leak-check=full ./a.out test1

clean:
	rm -rf *.dec *.huff *.lz77 a.out tests/*.dec tests/*.huff tests/*.lz77