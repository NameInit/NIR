main:
	g++ huffman.cpp


memory:
	valgrind --leak-check=full ./a.out