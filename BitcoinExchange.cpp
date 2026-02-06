#include "BitcoinExchange.hpp"
#include <fstream>
#include <iostream>
#include <cstdlib>

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) : _database(other._database) {}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other) {
	if (this != &other)
		_database = other._database;
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

const std::map<std::string, float> &BitcoinExchange::getDatabase() const {
	return _database;
}
