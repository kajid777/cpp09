#ifndef PMERGE_ME_HPP
#define PMERGE_ME_HPP

#include <vector>
#include <deque>
#include <map>
#include <string>
#include <algorithm>
#include <utility>
#include <cstddef>

// ============================================================================
// PmergeMe クラス
//
// Ford-Johnson アルゴリズム（merge-insertion sort）を用いて
// 正の整数列をソートする。std::vector と std::deque の2種類の
// コンテナで同じアルゴリズムを実行し、それぞれの処理時間を計測・表示する。
// ============================================================================
class PmergeMe {
public:
	// --- 正統派カノニカルフォーム（Orthodox Canonical Form） ---
	PmergeMe();
	PmergeMe(const PmergeMe& other);
	PmergeMe& operator=(const PmergeMe& other);
	~PmergeMe();

	// メイン処理: 引数をパース → ソート → 結果表示
	void run(int argc, char** argv);

private:
	std::vector<int> _vec;  // vector 用のデータ
	std::deque<int> _deq;   // deque 用のデータ

	// コマンドライン引数を解析し、正の整数として _vec と _deq に格納する
	void parseInput(int argc, char** argv);

	// ラベル付きで整数列を1行表示する（例: "Before: 3 5 9 7 4"）
	void display(const std::string& label, const std::vector<int>& c) const;

	// Jacobsthal 数に基づいて、pend 要素の挿入順序を生成する
	// pendSize: pend 配列の全要素数（pend[0] は既に挿入済みなので、戻り値は pend[1]〜のインデックス列）
	static std::vector<std::size_t> generateInsertionOrder(std::size_t pendSize);

	// Ford-Johnson アルゴリズム本体（テンプレートで vector / deque 両対応）
	template <typename Container>
	static void mergeInsertSort(Container& c);
};

// ============================================================================
// Ford-Johnson アルゴリズム（merge-insertion sort）テンプレート実装
//
// 【アルゴリズムの全体像】
//
// 入力:  [8, 3, 5, 1, 7, 2, 9]  （7個の要素）
//
// ステップ1: ペア化 & 比較
//   隣接する2要素をペアにし、大きい方を「勝者(winner)」、小さい方を「敗者(loser)」とする。
//   要素数が奇数なら、最後の1つは「あぶれ要素(straggler)」として退避する。
//
//   (8,3) → winner=8, loser=3
//   (5,1) → winner=5, loser=1
//   (7,2) → winner=7, loser=2
//   straggler = 9
//
// ステップ2: 勝者を再帰的にソート
//   勝者だけを集めて [8, 5, 7] とし、同じアルゴリズムで再帰ソートする。
//   → [5, 7, 8]
//
// ステップ3: メインチェーン構築
//   ソート済み勝者に対応する敗者を紐付ける:
//     winner=5 → loser=1,  winner=7 → loser=2,  winner=8 → loser=3
//
//   pend[0] (=1) は winner[0] (=5) より必ず小さいので、先頭に置ける:
//     メインチェーン = [1, 5, 7, 8]
//
// ステップ4: 残りの敗者を Jacobsthal 順で二分探索挿入
//   Jacobsthal 数: 1, 3, 5, 11, 21, 43, ...（比較回数を最小化する挿入順序）
//   挿入順: pend[2]=3 → pend[1]=2 の順
//     [1, 5, 7, 8] に 3 を挿入 → [1, 3, 5, 7, 8]
//     [1, 3, 5, 7, 8] に 2 を挿入 → [1, 2, 3, 5, 7, 8]
//
// 完成: [1, 2, 3, 5, 7, 8, 9]
//
// 【比較回数の最小化】
// pend[k] < winners[k] が保証されているため、二分探索の範囲を
// winners[k] の位置までに限定する。あぶれ要素は pend 末尾に追加し、
// Jacobsthal 順で挿入することで、各グループの探索範囲が 2^g - 1 以下に
// 収まり、理論上の最小比較回数を達成する。
// ============================================================================
template <typename Container>
void PmergeMe::mergeInsertSort(Container& c) {
	std::size_t n = c.size();

	// 要素が0個または1個ならソート不要（再帰の基底条件）
	if (n <= 1)
		return;

	// ========================================================================
	// ステップ1: ペア化 & あぶれ要素の退避
	// ========================================================================

	// 要素数が奇数なら最後の1要素を「あぶれ(straggler)」として退避
	bool hasStraggler = (n % 2 != 0);
	int straggler = 0;
	if (hasStraggler)
		straggler = c[n - 1];

	// 隣接する2要素をペアにする（大きい方を first=勝者、小さい方を second=敗者）
	// 例: [8, 3, 5, 1, 7, 2] → pairs = [(8,3), (5,1), (7,2)]
	std::size_t numPairs = n / 2;
	std::vector<std::pair<int, int> > pairs(numPairs);
	for (std::size_t i = 0; i < numPairs; i++) {
		if (c[2 * i] >= c[2 * i + 1])
			pairs[i] = std::make_pair(c[2 * i], c[2 * i + 1]);
		else
			pairs[i] = std::make_pair(c[2 * i + 1], c[2 * i]);
	}

	// ========================================================================
	// ステップ2: 勝者だけを抽出して再帰的にソート
	// ========================================================================

	// 各ペアの勝者(first)を集める → [8, 5, 7]
	Container winners;
	for (std::size_t i = 0; i < numPairs; i++)
		winners.push_back(pairs[i].first);

	// 再帰呼び出し: 勝者列を Ford-Johnson でソートする → [5, 7, 8]
	mergeInsertSort(winners);

	// ========================================================================
	// ステップ3: ソート済み勝者に対応する敗者(pend)を紐付ける
	// ========================================================================

	// 勝者→敗者の対応表を作る（重複する勝者値があっても正しく動くよう multimap を使用）
	// 例: {8→3, 5→1, 7→2}
	std::multimap<int, int> pairMap;
	for (std::size_t i = 0; i < numPairs; i++)
		pairMap.insert(std::make_pair(pairs[i].first, pairs[i].second));

	// ソート済み勝者の順序で、対応する敗者を pend 配列に格納する
	// winners = [5, 7, 8] → pend = [1, 2, 3]
	// ※ pend[i] < winners[i] が保証されている（ペア化時に比較済みのため）
	std::vector<int> pend(numPairs);
	for (std::size_t i = 0; i < numPairs; i++) {
		typename std::multimap<int, int>::iterator it = pairMap.find(winners[i]);
		pend[i] = it->second;
		pairMap.erase(it);  // 使用済みのエントリを削除（重複対策）
	}

	// あぶれ要素を pend の末尾に追加（Jacobsthal 順で最適な位置に挿入するため）
	if (hasStraggler)
		pend.push_back(straggler);
	std::size_t totalPend = pend.size();

	// ========================================================================
	// ステップ4: メインチェーンを構築する
	// ========================================================================

	Container mainChain;
	mainChain.push_back(pend[0]);
	for (std::size_t i = 0; i < numPairs; i++)
		mainChain.push_back(winners[i]);

	// ========================================================================
	// ステップ5: 残りの pend 要素を Jacobsthal 順で二分探索挿入
	// ========================================================================

	// 各 winner の mainChain 内での位置を追跡する。
	// pend[k] < winners[k] が保証されているため、二分探索の上界を winners[k] の
	// 位置に制限でき、比較回数を理論上の最小値に抑えられる。
	// あぶれ要素の「winner」はチェーン末尾（= mainChain.size()）とする。
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
