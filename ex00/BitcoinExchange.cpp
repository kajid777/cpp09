#include "BitcoinExchange.hpp"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cerrno>

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
		std::exit(1);
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
		std::exit(1);
	}

	std::string line;
	std::getline(file, line);

	while (std::getline(file, line))
		_input.push_back(line);
	file.close();
}

bool BitcoinExchange::isValidDate(const std::string &date) {
	if (date.length() != 10)
		return false;
	if (date[4] != '-' || date[7] != '-')
		return false;
	for (int i = 0; i < 10; ++i) {
		if (i == 4 || i == 7)
			continue;
		if (date[i] < '0' || date[i] > '9')
			return false;
	}

	int year = std::atoi(date.substr(0, 4).c_str());
	int month = std::atoi(date.substr(5, 2).c_str());
	int day = std::atoi(date.substr(8, 2).c_str());

	if (month < 1 || month > 12 || day < 1)
		return false;

	int maxDay;
	switch (month) {
		case 2:
			maxDay = 28;
			if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
				maxDay = 29;
			break;
		case 4: case 6: case 9: case 11:
			maxDay = 30;
			break;
		default:
			maxDay = 31;
			break;
	}
	return day <= maxDay;
}

bool BitcoinExchange::isValidValue(const std::string &value) {
	if (value.empty())
		return false;
	char *end;
	errno = 0;
	std::strtod(value.c_str(), &end);
	if (errno == ERANGE)
		return false;
	while (*end == ' ' || *end == '\t')
		++end;
	return *end == '\0';
}

void BitcoinExchange::execute() const {
	for (std::list<std::string>::const_iterator lit = _input.begin(); lit != _input.end(); ++lit) {
		const std::string &line = *lit;

		std::string::size_type pos = line.find(" | ");
		if (pos == std::string::npos) {
			std::cerr << "Error: bad input => " << line << std::endl;
			continue;
		}

		std::string date = line.substr(0, pos);
		std::string valueStr = line.substr(pos + 3);

		if (!isValidDate(date)) {
			std::cerr << "Error: bad input => " << date << std::endl;
			continue;
		}

		if (!isValidValue(valueStr)) {
			std::cerr << "Error: bad input => " << line << std::endl;
			continue;
		}

		float value = static_cast<float>(std::atof(valueStr.c_str()));

		if (value < 0) {
			std::cerr << "Error: not a positive number." << std::endl;
			continue;
		}
		if (value > 1000) {
			std::cerr << "Error: too large a number." << std::endl;
			continue;
		}

		std::map<std::string, float>::const_iterator it = _database.lower_bound(date);
		if (it != _database.end() && it->first == date) {
			std::cout << date << ", value:" << value << ", exchange_rate:" << it->second << ", result:" << value * it->second << std::endl;
		} else if (it != _database.begin()) {
			--it;
			std::cout << date << ", value:" << value << ", exchange_rate:" << it->second << ", result:" << value * it->second << std::endl;
		} else {
			std::cerr << "Error: bad input => " << date << std::endl;
		}
	}
}
