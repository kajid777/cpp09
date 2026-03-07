#include "BitcoinExchange.hpp"
#include <iostream>

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "Error: could not open file." << std::endl;
		return 1;
	}

	BitcoinExchange btc;

	btc.loadDatabase("data.csv");
	btc.loadInput(argv[1]);
	btc.execute();

	return 0;
}
