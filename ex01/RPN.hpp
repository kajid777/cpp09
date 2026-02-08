#ifndef RPN_HPP
#define RPN_HPP

#include <stack>
#include <string>

class RPN {
public:
	RPN();
	RPN(const RPN &other);
	RPN &operator=(const RPN &other);
	~RPN();

	void execute(const std::string &expression);
	const std::stack<int> &getStack() const;

private:
	std::stack<int> _stack;
	bool isOperator(const std::string &token) const;
	bool calcOperator(const std::string &token);
	bool pushNumber(const std::string &token);
};

#endif
