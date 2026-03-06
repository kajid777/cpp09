#ifndef BINARY_INSERTION_SORT_HPP
#define BINARY_INSERTION_SORT_HPP

#include <vector>
#include <iterator>

/**
 * 二分挿入ソート
 *
 * 2つのグループを持つアルゴリズム:
 * - グループ1: ソート済み
 * - グループ2: ソートされていない
 *
 * グループ2から1つずつ要素を取り出し、
 * 二分探索でグループ1内の挿入位置を求め、
 * グループ1に挿入していく
 */
template <typename T>
class BinaryInsertionSort {
public:
	// 二分探索で挿入位置（イテレータ）を返す
	// [first, last) はソート済み、value の挿入位置を探す
	template <typename Iterator>
	static Iterator binarySearchInsertPos(Iterator first, Iterator last, const T &value) {
		Iterator left = first;
		Iterator right = last;

		while (left != right) {
			Iterator mid = left;
			std::advance(mid, std::distance(left, right) / 2);

			if (*mid < value) {
				left = mid;
				++left;
			} else {
				right = mid;
			}
		}
		return left;
	}

	// 未ソートのベクターを、ソート済みグループに1つずつ二分探索で挿入
	static void sort(std::vector<T> &sorted, std::vector<T> &unsorted) {
		for (typename std::vector<T>::iterator it = unsorted.begin(); it != unsorted.end(); ++it) {
			T value = *it;
			typename std::vector<T>::iterator pos = binarySearchInsertPos(sorted.begin(), sorted.end(), value);
			sorted.insert(pos, value);
		}
	}

	// 1つのベクターに対して実行（最初の1要素がソート済み、残りを順次挿入）
	static void sort(std::vector<T> &vec) {
		if (vec.size() <= 1)
			return;

		std::vector<T> sorted;
		sorted.push_back(vec[0]);

		std::vector<T> unsorted(vec.begin() + 1, vec.end());
		sort(sorted, unsorted);
		vec = sorted;
	}
};

#endif
