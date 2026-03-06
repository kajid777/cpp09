#include "PmergeMe.hpp"
#include <iostream>

// ============================================================================
// エントリーポイント
//
// 使い方: ./PmergeMe 3 5 9 7 4
//   - 正の整数を空白区切りで引数として渡す
//   - ソート前後の列と、各コンテナ(vector/deque)の処理時間を表示する
//   - 不正な入力（負数・文字列・0・オーバーフロー）は "Error" を出力して終了
// ============================================================================
int main(int argc, char** argv) {
	// 引数が1つもなければエラー
	if (argc < 2) {
		std::cerr << "Error" << std::endl;
		return 1;
	}
	try {
		PmergeMe sorter;
		sorter.run(argc, argv);
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}
