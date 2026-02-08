#include "BitcoinExchange.hpp"
#include <fstream>
#include <iostream>
#include <cstdlib>

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) : _database(other._database), _input(other._input) {}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other) {
	if (this != &other) {
		_database = other._database;
		_input = other._input;
	}
	return *this;
}

BitcoinExchange::~BitcoinExchange() {}

void BitcoinExchange::loadDatabase(const std::string &filename) {
	std::ifstream file(filename.c_str());
	if (!file.is_open()) {
		std::cerr << "Error: could not open file." << std::endl;
		return;
	}

	std::string line;
	// ヘッダー行をスキップ
	std::getline(file, line);

	while (std::getline(file, line)) {
		std::string::size_type pos = line.find(',');
		// カンマがない行は不正なのでスキップ
		if (pos == std::string::npos)
			continue;

		std::string date = line.substr(0, pos);
		std::string valueStr = line.substr(pos + 1);
		float value = static_cast<float>(std::atof(valueStr.c_str()));

		_database[date] = value;
	}
	file.close();
}

void BitcoinExchange::loadInput(const std::string &filename) {
	std::ifstream file(filename.c_str());
	if (!file.is_open()) {
		std::cerr << "Error: could not open file." << std::endl;
		return;
	}

	std::string line;
	// ヘッダー行をスキップ
	std::getline(file, line);

	while (std::getline(file, line)) {
		std::string::size_type pos = line.find(" | ");
		// " | " がない行は不正なのでスキップ
		if (pos == std::string::npos)
			continue;

		std::string date = line.substr(0, pos);
		std::string valueStr = line.substr(pos + 3);
		float value = static_cast<float>(std::atof(valueStr.c_str()));

		_input.push_back(std::make_pair(date, value));
	}
	file.close();
}

const std::map<std::string, float> &BitcoinExchange::getDatabase() const {
	return _database;
}

const std::vector<std::pair<std::string, float> > &BitcoinExchange::getInput() const {
	return _input;
}

void BitcoinExchange::execute() const {
	for (size_t i = 0; i < _input.size(); ++i) {
		std::string date_for_search = _input[i].first;
		float value = _input[i].second;

		// _inputのdateで_databaseを検索（その日付以前で最も近い日付を使う）
		std::map<std::string, float>::const_iterator it = _database.lower_bound(date_for_search);
		//lower_bound は std::map の検索関数で、指定したキー以上で最初の要素を返すのでイテレーターを後で一個戻している
		if (it != _database.end() && it->first == date_for_search) {
			// 日付完全一致したとき
			std::cout << date_for_search << ", value:" << value << ", exchange_rate:" << it->second << ", result:" << value * it->second << std::endl;
		} else if (it != _database.begin()) {
			//日付完全一致しなかったとき
			// イテレーターを戻している
			--it;
			std::cout << date_for_search << ", value:" << value << ", exchange_rate:" << it->second << ", result:" << value * it->second << std::endl;
		} else {
			std::cerr << "Error: date not found in database => " << date_for_search << std::endl;
		}
	}
}
