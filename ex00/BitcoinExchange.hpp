#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <vector>
#include <string>
#include <utility>

class BitcoinExchange {
public:
	BitcoinExchange();
	BitcoinExchange(const BitcoinExchange &other);
	BitcoinExchange &operator=(const BitcoinExchange &other);
	~BitcoinExchange();

	void loadDatabase(const std::string &filename);
	void loadInput(const std::string &filename);
	const std::map<std::string, float> &getDatabase() const;
	const std::vector<std::pair<std::string, float> > &getInput() const;
	void execute() const;

private:
	std::map<std::string, float> _database;
	std::vector<std::pair<std::string, float> > _input;
};

#endif
