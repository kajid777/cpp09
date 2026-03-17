#include "PmergeMe.hpp"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <stdexcept>
#include <cctype>

// === 正統派カノニカルフォーム ===

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

// ============================================================================
// parseInput: コマンドライン引数を解析して正の整数列として取り込む
//
// 各引数について以下をチェック:
//   - 空文字列でないこと
//   - 全ての文字が数字であること（負号やアルファベット不可）
//   - 数値が 1 以上 INT_MAX 以下であること（0 も不可）
//
// 異常があれば例外 "Error" を投げる
// ============================================================================
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

// ============================================================================
// display: ラベル付きで整数列を1行に表示する
//   例: "Before: 3 5 9 7 4"
// ============================================================================
void PmergeMe::display(const std::string& label, const std::vector<int>& c) const {
	std::cout << label;
	for (std::size_t i = 0; i < c.size(); i++)
		std::cout << " " << c[i];
	std::cout << std::endl;
}

// ============================================================================
// generateInsertionOrder: Jacobsthal 数に基づく挿入順序を生成する
//
// 【Jacobsthal 数とは】
//   J(0)=0, J(1)=1, J(n) = J(n-1) + 2*J(n-2)
//   → 0, 1, 1, 3, 5, 11, 21, 43, 85, ...
//
// 【なぜ Jacobsthal 順で挿入するのか】
//   Ford-Johnson アルゴリズムの核心は「比較回数の最小化」。
//   各 pend 要素を挿入する際、二分探索の対象範囲を 2^k - 1 個以内に
//   抑えることで、各挿入が最大 k 回の比較で済むようにする。
//   Jacobsthal 数に基づくグループ分けがこの性質を実現する。
//
// 【挿入グループの作り方】
//   Jacobsthal 境界列: jn = [1, 3, 5, 11, 21, ...]
//
//   各グループ k では、インデックス jn[k]-1 から jn[k-1] まで降順に挿入:
//     グループ0: pend[0] → 既に挿入済み（メインチェーン構築時に先頭に配置）
//     グループ1: pend[2], pend[1]         （jn[1]=3 から jn[0]=1 の間）
//     グループ2: pend[4], pend[3]         （jn[2]=5 から jn[1]=3 の間）
//     グループ3: pend[10],...,pend[5]     （jn[3]=11 から jn[2]=5 の間）
//     ...
//
//   pendSize を超えるインデックスはスキップされる。
//
// 引数: pendSize = pend 配列の全要素数
// 戻り値: pend[1]〜pend[pendSize-1] のインデックスを最適な挿入順で並べた配列
// ============================================================================
std::vector<std::size_t> PmergeMe::generateInsertionOrder(std::size_t pendSize) {
	std::vector<std::size_t> order;

	// pend が1個以下なら挿入すべき要素がない（pend[0] は既に挿入済み）
	if (pendSize <= 1)
		return order;

	// Jacobsthal 境界列を生成: [1, 3, 5, 11, 21, 43, ...]
	// pendSize 以上の値が出るまで生成する
	std::vector<std::size_t> jn;
	jn.push_back(1);   // jn[0] = 1
	jn.push_back(3);   // jn[1] = 3
	while (jn.back() < pendSize) {
		// 漸化式: jn[k] = jn[k-1] + 2 * jn[k-2]
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

// ============================================================================
// run: プログラムのメイン処理フロー
//
//   1. コマンドライン引数をパースして _vec と _deq に格納
//   2. ソート前の状態を保存
//   3. std::vector に対して Ford-Johnson ソートを実行し、処理時間を計測
//   4. std::deque  に対して Ford-Johnson ソートを実行し、処理時間を計測
//   5. ソート前 / ソート後 / 各コンテナの処理時間を表示
// ============================================================================
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
