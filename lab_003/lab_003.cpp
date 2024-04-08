#include <utility>
#include <vector>
#include "test_runner.h"

using namespace std;

template <typename T>
class Table {
public:
    Table(size_t rows, size_t cols) : rows_(rows), cols_(cols), data_(rows, vector<T>(cols)) {}

    vector<T>& operator[](size_t row_idx) {
        return data_[row_idx];
    }

    const vector<T>& operator[](size_t row_idx) const {
        return data_[row_idx];
    }

    void Resize(size_t new_rows, size_t new_cols) {
        size_t old_rows = rows_;
        size_t old_cols = cols_;
        rows_ = new_rows;
        cols_ = new_cols;
        data_.resize(rows_);
        for (auto& row : data_) {
            row.resize(cols_);
        }
        for (size_t i = 0; i < std::min(old_rows, rows_); ++i) {
            for (size_t j = 0; j < std::min(old_cols, cols_); ++j) {
                data_[i][j] = move(data_[i][j]);
            }
        }
    }

    std::pair<size_t, size_t> Size() const {
        return {rows_ != 0 ? rows_ : 0, cols_ != 0 ? cols_ : 0};
    }

private:
    size_t rows_, cols_;
    vector<vector<T>> data_;
};

void TestTable() {
    Table<int> t(1, 1);

    ASSERT_EQUAL(t.Size().first, 1u);
    ASSERT_EQUAL(t.Size().second, 1u);

    t[0][0] = 42;
    ASSERT_EQUAL(t[0][0], 42);

    t.Resize(3, 4);
    ASSERT_EQUAL(t.Size().first, 3u);
    ASSERT_EQUAL(t.Size().second, 4u);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestTable);
    return 0;
}