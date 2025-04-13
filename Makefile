main:
	g++ main.cpp

run: main
	./a.out test1

make test: main
	./test.sh

memory:
	valgrind --leak-check=full ./a.out test1

clean:
	rm -rf *_dec *.bin a.out tests/*.dec tests/*.bin