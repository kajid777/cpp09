#include "BitcoinExchange.hpp"
#include <iostream>

int main(void) {
	BitcoinExchange btc;

	btc.loadDatabase("data.csv");

	// ロードしたデータベースの一部を表示
	const std::map<std::string, float> &db = btc.getDatabase();
	std::map<std::string, float>::const_iterator it = db.begin();
	for (int i = 0; i < 5 && it != db.end(); ++i, ++it) {
		std::cout << it->first << " => " << it->second << std::endl;
	}
	std::cout << "... (" << db.size() << " entries total)" << std::endl;

	return 0;
}
