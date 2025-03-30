main:
	g++ main.cpp

run:
	./a.out test1

make test: main
	./test.sh

memory:
	valgrind --leak-check=full ./a.out