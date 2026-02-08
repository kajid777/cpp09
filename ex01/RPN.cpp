#include "RPN.hpp"
#include <iostream>
#include <sstream>
#include <cstdlib>

RPN::RPN() {}

RPN::RPN(const RPN &other) : _stack(other._stack) {}

RPN &RPN::operator=(const RPN &other) {
	if (this != &other)
		_stack = other._stack;
	return *this;
}

RPN::~RPN() {}

bool RPN::isOperator(const std::string &token) const {
	return token == "+" || token == "-" || token == "*" || token == "/";
}

void RPN::execute(const std::string &expression) {
	std::istringstream iss(expression);
	std::string token;

	while (iss >> token) {
		if (isOperator(token)) {
			// 演算子の場合、スタックから2つ取り出して計算し結果を積む
			if (_stack.size() < 2) {
				std::cerr << "Error" << std::endl;
				return;
			}
			int b = _stack.top();
			_stack.pop();
			int a = _stack.top();
			_stack.pop();

			int result;
			if (token == "+")
				result = a + b;
			else if (token == "-")
				result = a - b;
			else if (token == "*")
				result = a * b;
			else {
				if (b == 0) {
					std::cerr << "Error" << std::endl;
					return;
				}
				result = a / b;
			}
			_stack.push(result);
			std::cout << "push " << result << " (" << a << " " << token << " " << b << ")" << std::endl;
		} else {
			// 0から9までの1桁の自然数のみ許可
			if (token.length() != 1 || !std::isdigit(token[0])) {
				std::cerr << "Error" << std::endl;
				return;
			}
			int num = token[0] - '0';
			_stack.push(num);
			std::cout << "push " << num << std::endl;
		}
	}

	if (_stack.size() != 1) {
		std::cerr << "Error" << std::endl;
		return;
	}
	std::cout << _stack.top() << std::endl;
}

const std::stack<int> &RPN::getStack() const {
	return _stack;
}
