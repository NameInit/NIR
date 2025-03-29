main:
	g++ main.cpp


memory:
	valgrind --leak-check=full ./a.out