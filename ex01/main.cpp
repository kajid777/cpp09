#include "RPN.hpp"
#include <iostream>

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cout << "Error" << std::endl;
		return 1;
	}

	RPN rpn;
	rpn.execute(argv[1]);

	return 0;
}
