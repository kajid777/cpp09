#include "PmergeMe.hpp"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <stdexcept>
#include <cctype>

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe& other)
	: _vec(other._vec), _deq(other._deq) {}

PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
	if (this != &other) {
		_vec = other._vec;
		_deq = other._deq;
	}
	return *this;
}

PmergeMe::~PmergeMe() {}

// 引数を正の整数として検証し _vec と _deq に格納する
void PmergeMe::parseInput(int argc, char** argv) {
	for (int i = 1; i < argc; i++) {
		std::string arg(argv[i]);

		// 空文字列チェック
		if (arg.empty())
			throw std::runtime_error("Error");

		// 全ての文字が数字(0-9)であることを確認
		for (std::size_t j = 0; j < arg.size(); j++) {
			if (!std::isdigit(static_cast<unsigned char>(arg[j])))
				throw std::runtime_error("Error");
		}

		// 文字列を long に変換し、範囲チェック
		char* end;
		long val = std::strtol(argv[i], &end, 10);
		if (*end != '\0' || val <= 0 || val > INT_MAX)
			throw std::runtime_error("Error");

		// vector と deque の両方に同じ値を格納
		int num = static_cast<int>(val);
		_vec.push_back(num);
		_deq.push_back(num);
	}

	// 引数が1つもなければエラー
	if (_vec.empty())
		throw std::runtime_error("Error");
}

// ラベル付きで整数列を1行表示する
void PmergeMe::display(const std::string& label, const std::vector<int>& c) const {
	std::cout << label;
	for (std::size_t i = 0; i < c.size(); i++)
		std::cout << " " << c[i];
	std::cout << std::endl;
}

// Jacobsthal 数に基づいて pend[1]〜 の挿入順インデックス列を返す
std::vector<std::size_t> PmergeMe::generateInsertionOrder(std::size_t pendSize) {
	std::vector<std::size_t> order;

	// pend が1個以下なら挿入すべき要素がない（pend[0] は既に挿入済み）
	if (pendSize <= 1)
		return order;

	// 境界列: jn[k] = jn[k-1] + 2 * jn[k-2] （1, 3, 5, 11, 21, ...）
	std::vector<std::size_t> jn;
	jn.push_back(1);   // jn[0] = 1
	jn.push_back(3);   // jn[1] = 3
	while (jn.back() < pendSize) {
		std::size_t next = jn[jn.size() - 1] + 2 * jn[jn.size() - 2];
		jn.push_back(next);
	}

	// 各グループ内で降順にインデックスを追加
	// 例: pendSize=5 の場合
	//   k=1: high=min(3,5)=3, low=1 → インデックス 2, 1 を追加
	//   k=2: high=min(5,5)=5, low=3 → インデックス 4, 3 を追加
	//   合計4個 = pendSize-1 なので終了
	for (std::size_t k = 1; k < jn.size(); k++) {
		std::size_t high = jn[k];
		if (high > pendSize)
			high = pendSize;
		std::size_t low = jn[k - 1];

		// high-1 から low まで降順にインデックスを追加
		for (std::size_t i = high; i > low; ) {
			--i;
			order.push_back(i);
		}

		// 全ての pend 要素（pend[0] を除く）の挿入順が決まったら終了
		if (order.size() >= pendSize - 1)
			break;
	}

	return order;
}

// vector と deque それぞれをソートして処理時間を表示する
void PmergeMe::run(int argc, char** argv) {
	parseInput(argc, argv);

	std::vector<int> before(_vec);

	clock_t startVec = clock();
	mergeInsertSort(_vec);
	clock_t endVec = clock();
	double timeVec = static_cast<double>(endVec - startVec) / CLOCKS_PER_SEC * 1000000.0;

	clock_t startDeq = clock();
	mergeInsertSort(_deq);
	clock_t endDeq = clock();
	double timeDeq = static_cast<double>(endDeq - startDeq) / CLOCKS_PER_SEC * 1000000.0;

	display("Before:", before);
	std::vector<int> after(_vec.begin(), _vec.end());
	display("After: ", after);
	std::cout << "Time to process a range of " << _vec.size()
			  << " elements with std::vector : " << std::fixed
			  << std::setprecision(5) << timeVec << " us" << std::endl;
	std::cout << "Time to process a range of " << _deq.size()
			  << " elements with std::deque  : " << std::fixed
			  << std::setprecision(5) << timeDeq << " us" << std::endl;
}
