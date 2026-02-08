#include "BitcoinExchange.hpp"
#include <iostream>
#include <vector>
#include <utility>

int main(void) {
	BitcoinExchange btc;

	btc.loadDatabase("data.csv");

	// ロードしたデータベースの一部を表示
	std::cout << "ロードしたデータベースの内容を表示" << std::endl;
	const std::map<std::string, float> &db = btc.getDatabase();
	std::map<std::string, float>::const_iterator it = db.begin();
	for (int i = 0; i < 5 && it != db.end(); ++i, ++it) {
		std::cout << it->first << " => " << it->second << std::endl;
	}
	std::cout << "... (" << db.size() << " entries total)" << std::endl;

	btc.loadInput("input.txt");

	// ロードしたインプットの内容を表示
	std::cout << "ロードしたインプットの内容を表示" << std::endl;
	const std::vector<std::pair<std::string, float> > &input = btc.getInput();
	for (size_t i = 0; i < input.size(); ++i) {
		std::cout << input[i].first << " | " << input[i].second << std::endl;
	}

	// _inputのdateで_databaseを検索して value と exchange_rate を出力
	btc.execute();

	return 0;
}
