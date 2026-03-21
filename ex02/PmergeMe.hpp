#ifndef PMERGE_ME_HPP
#define PMERGE_ME_HPP

#include <vector>
#include <deque>
#include <map>
#include <string>
#include <algorithm>
#include <utility>
#include <cstddef>

// Ford-Johnson アルゴリズムで正整数列をソートし、vector/deque それぞれの処理時間を比較する
class PmergeMe {
public:
	PmergeMe();
	PmergeMe(const PmergeMe& other);
	PmergeMe& operator=(const PmergeMe& other);
	~PmergeMe();

	void run(int argc, char** argv);

private:
	std::vector<int> _vec;
	std::deque<int> _deq;

	void parseInput(int argc, char** argv);
	void display(const std::string& label, const std::vector<int>& c) const;
	static std::vector<std::size_t> generateInsertionOrder(std::size_t pendSize);

	template <typename Container>
	static void mergeInsertSort(Container& c);
};

// Ford-Johnson (merge-insertion sort) 本体
// ペア化→勝者を再帰ソート→pend を Jacobsthal 順で二分探索挿入する
template <typename Container>
void PmergeMe::mergeInsertSort(Container& c) {
	std::size_t n = c.size();

	if (n <= 1)
		return;

	// 奇数個なら末尾を退避
	bool hasStraggler = (n % 2 != 0);
	int straggler = 0;
	if (hasStraggler)
		straggler = c[n - 1];

	// 隣接ペアを作り、大きい方を winner とする
	std::size_t numPairs = n / 2;
	std::vector<std::pair<int, int> > pairs(numPairs);
	for (std::size_t i = 0; i < numPairs; i++) {
		if (c[2 * i] >= c[2 * i + 1])
			pairs[i] = std::make_pair(c[2 * i], c[2 * i + 1]);
		else
			pairs[i] = std::make_pair(c[2 * i + 1], c[2 * i]);
	}

	// winner だけを再帰的にソート
	Container winners;
	for (std::size_t i = 0; i < numPairs; i++)
		winners.push_back(pairs[i].first);
	mergeInsertSort(winners);

	// ソート済み winner に対応する pend を紐付ける
	// pend[i] < winners[i] が保証されているので二分探索の上界を絞れる
	std::multimap<int, int> pairMap;
	for (std::size_t i = 0; i < numPairs; i++)
		pairMap.insert(std::make_pair(pairs[i].first, pairs[i].second));

	std::vector<int> pend(numPairs);
	for (std::size_t i = 0; i < numPairs; i++) {
		typename std::multimap<int, int>::iterator it = pairMap.find(winners[i]);
		pend[i] = it->second;
		pairMap.erase(it);
	}

	if (hasStraggler)
		pend.push_back(straggler);
	std::size_t totalPend = pend.size();

	// メインチェーン: pend[0] を先頭に置いてから winner を並べる
	Container mainChain;
	mainChain.push_back(pend[0]);
	for (std::size_t i = 0; i < numPairs; i++)
		mainChain.push_back(winners[i]);

	// 各 winner のチェーン内位置を追跡しながら pend を Jacobsthal 順で挿入する
	std::vector<std::size_t> winnerPos(totalPend);
	for (std::size_t i = 0; i < numPairs; i++)
		winnerPos[i] = i + 1;
	if (hasStraggler)
		winnerPos[numPairs] = mainChain.size();

	if (totalPend > 1) {
		std::vector<std::size_t> order = generateInsertionOrder(totalPend);
		for (std::size_t i = 0; i < order.size(); i++) {
			std::size_t pendIdx = order[i];
			int val = pend[pendIdx];
			std::size_t bound = winnerPos[pendIdx];
			typename Container::iterator pos =
				std::lower_bound(mainChain.begin(), mainChain.begin() + bound, val);
			std::size_t insertPos = static_cast<std::size_t>(
				pos - mainChain.begin());
			mainChain.insert(pos, val);
			for (std::size_t j = 0; j < totalPend; j++) {
				if (winnerPos[j] >= insertPos)
					winnerPos[j]++;
			}
		}
	}

	c = mainChain;
}

#endif
