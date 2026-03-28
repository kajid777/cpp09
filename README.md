# CPP09 — C++ STL 演習課題

42 School の C++ モジュール09 課題です。C++98 標準の STL コンテナ・アルゴリズムを活用した3つのプログラムを実装しています。

---

## 目的

- STL コンテナ（`std::map`, `std::list`, `std::vector`, `std::deque`, `std::stack`）を適切な場面で選択・活用する能力を養う
- 実用的なアルゴリズム（逆ポーランド記法の評価、Ford-Johnson ソート）を C++98 で実装する
- 入力バリデーション・エラー処理の実装パターンを習得する

---

## 構成

```
cpp09_00/
├── ex00/   Bitcoin Exchange — CSV レート DB を用いた BTC 価値計算
├── ex01/   RPN Calculator   — 逆ポーランド記法の式評価
└── ex02/   PmergeMe         — Ford-Johnson ソートと処理時間比較
```

---

## 各演習の概要

### ex00 — Bitcoin Exchange (`btc`)

**目的**: 過去の Bitcoin レートデータベースと入力ファイルを照合し、指定日のBTC価値を計算する。

**使い方**:
```bash
cd ex00
make
./btc input.txt
```

**動作**:
1. `data.csv` を読み込み、`std::map<string, float>` に日付→レートのデータベースを構築する
2. 引数で渡された入力ファイルを読み込み、`date | value` 形式の各行を処理する
3. 各行の日付に最も近い過去の日付のレートを `lower_bound` で検索し、`value × rate` の結果を出力する

**バリデーション**:
- 日付形式 `YYYY-MM-DD`（うるう年対応）
- 値は `0` 以上 `1000` 以下の数値

**主要ファイル**:

| ファイル | 役割 |
|---|---|
| `BitcoinExchange.hpp` | クラス定義（`std::map` DB + `std::list` 入力） |
| `BitcoinExchange.cpp` | DB 読み込み・バリデーション・レート検索・出力 |
| `main.cpp` | 引数チェックとクラス呼び出し |
| `data.csv` | Bitcoin 日次レートデータベース |
| `input.txt` | 計算対象の日付・保有量リスト |

---

### ex01 — RPN Calculator (`RPN`)

**目的**: スペース区切りで渡された逆ポーランド記法（後置記法）の数式を `std::stack` を使って評価する。

**使い方**:
```bash
cd ex01
make
./RPN "3 4 + 2 *"
# => 14
```

**動作**:
- トークンを左から順に処理し、数字ならスタックに積み、演算子なら上位2要素を取り出して演算しスタックに戻す
- 全トークン処理後にスタックに1要素だけ残っていれば、それが計算結果

**制約**:
- 入力数値は0〜9の1桁整数のみ
- 演算子は `+`, `-`, `*`, `/` のみ
- ゼロ除算・スタック不足・オーバーフローはエラーとして処理

**主要ファイル**:

| ファイル | 役割 |
|---|---|
| `RPN.hpp` | クラス定義（`std::stack<int>` 保持） |
| `RPN.cpp` | トークン解析・演算・エラー処理 |
| `main.cpp` | コマンドライン引数受け取りと実行 |

---

### ex02 — PmergeMe (`PmergeMe`)

**目的**: Ford-Johnson アルゴリズム（マージ挿入ソート）を `std::vector` と `std::deque` の両方で実装し、処理時間を比較する。

**使い方**:
```bash
cd ex02
make
./PmergeMe 5 3 8 1 9 2
```

**出力例**:
```
Before:  5 3 8 1 9 2
After:   1 2 3 5 8 9
Time to process a range of 6 elements with std::vector :  4.00000 us
Time to process a range of 6 elements with std::deque  :  5.00000 us
```

**Ford-Johnson アルゴリズムの流れ**:
1. 隣接ペアを作り、大きい方（winner）と小さい方（pend）に分離する
2. winner 列を再帰的にソートする
3. pend を Jacobsthal 数に基づく挿入順で winner 列へ二分探索挿入する

**主要ファイル**:

| ファイル | 役割 |
|---|---|
| `PmergeMe.hpp` | クラス定義 + `mergeInsertSort<T>` テンプレート実装 |
| `PmergeMe.cpp` | 入力パース・Jacobsthal 順生成・計時・表示 |
| `main.cpp` | エントリポイント |

---

## ビルド方法

各演習ディレクトリで `make` を実行します。

```bash
# ex00
cd ex00 && make

# ex01
cd ex01 && make

# ex02
cd ex02 && make
```

**ビルドオプション**: `-Wall -Wextra -Werror -std=c++98`

クリーンビルドは `make re`、オブジェクトファイルの削除は `make clean`、実行ファイルも含む削除は `make fclean`。
