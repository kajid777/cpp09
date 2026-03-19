#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <list>
#include <string>

class BitcoinExchange {
public:
	BitcoinExchange();
	BitcoinExchange(const BitcoinExchange &other);
	BitcoinExchange &operator=(const BitcoinExchange &other);
	~BitcoinExchange();

	void loadDatabase(const std::string &filename);
	void loadInput(const std::string &filename);
	void execute() const;

private:
	std::map<std::string, float> _database;
	std::list<std::string> _input;

	static bool isValidDate(const std::string &date);
	static bool isValidValue(const std::string &value);
};

#endif
