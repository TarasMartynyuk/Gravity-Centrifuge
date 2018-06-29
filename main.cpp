#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <sstream>
#include <iterator>

using namespace std;

template<class TInputContainer, class OIter>
OIter copy(const TInputContainer& container, OIter out_it) {
    return std::copy(container.begin(), container.end(), out_it);
};

// default parameter is only for numerical types
// init value is needed to deduce TElem
template<typename TElem, class TContainer>
TElem sum(const TContainer& container, TElem init_value) {
    return std::accumulate(container.begin(), container.end(), init_value);
};

template<typename TContainer, class TPred>
bool all_of(const TContainer& container, TPred pred) {
    return all_of(container.begin(), container.end(), pred);
};

class Landscape {
public:
    Landscape(vector<int>* row_block_counts,
              vector<int>* col_block_counts)
        : row_block_counts(row_block_counts),
        col_block_counts(col_block_counts)
    { assert(columnsRowsHaveEqualBlocksCount()); }

    void tumble() {
        rollOver();
        simulateFall();
    }

    // assumes that all the blocks have already fallen to bottom,
    // thus ignores the row_block_counts values
    void toGrid(vector<vector<char>> & grid) {
        assert(grid.size() >= rows());
        assert(all_of(grid, [this](const vector<char>& row) {
                        return row.size() == columns();
        }));

        for (int j = 0; j < columns(); ++j) {
            int blocks_for_col = col_block_counts->at(j);
            for (int i = 0; i < rows(); ++i) {
                int row_from_bot = rows() - i;
                char ch = row_from_bot <= blocks_for_col ?
                          '#' : '.';
                    grid.at(i).at(j) = ch;
            }
        }
    }

    int rows() { return row_block_counts->size(); }
    int columns() { return col_block_counts->size(); }
private:
    // row : # blocks in that row
    vector<int>* row_block_counts;
    // col : # blocks in that col
    vector<int>* col_block_counts;

    // updates row blocks counts to simulate blocks falling to the bottom
    void simulateFall() {
        for (int i = rows() - 1; i >= 0; --i) {
            // how much blocks must the column have
            // so that when they fall a block will be placed in the ith row
            int thresh = rows() - i;
            int row_blocks = countColumnsWithNotLessBlocksThan(thresh);
            row_block_counts->at(i) = row_blocks;
        }
        assert(columnsRowsHaveEqualBlocksCount());
    }
    // rolls by 90 deg counter clockwise
    void rollOver() {
        auto* temp = col_block_counts;
        col_block_counts = row_block_counts;

        row_block_counts = new vector<int>(temp->size());
        for (int i = 0; i < temp->size(); ++i) {
            int reverse = temp->size() - 1 - i;
            row_block_counts->at(reverse) = temp->at(i);
        }
    }

    int countColumnsWithNotLessBlocksThan(int thresh) {
        return count_if(col_block_counts->begin(), col_block_counts->end(), [thresh](int blocks) {
            return blocks >= thresh;
        });
    }
    bool columnsRowsHaveEqualBlocksCount() {
        int row_sum = sum(*row_block_counts, int());
        int col_sum = sum(*col_block_counts, int());
        return sum(*row_block_counts, int()) == sum(*col_block_counts, int());
    }
};

int main()
{
    int columns;
    int rows;
    cin >> columns >> rows; cin.ignore();
    int tumblings_count; cin >> tumblings_count; cin.ignore();

    auto* row_block_counts = new vector<int>(rows, 0);
    auto* col_block_counts = new vector<int>(columns, 0);

    for (int i = 0; i < rows; i++) {
        string tile_row; getline(cin, tile_row);

        for (int j = 0; j < columns; ++j) {
            if(tile_row.at(j) == '#') {
                row_block_counts->at(i)++;
                col_block_counts->at(j)++;
            }
        }
    }
    //endregion

    Landscape landscape(row_block_counts, col_block_counts);

    for (int i = 0; i < tumblings_count; ++i) {
        landscape.tumble();
    }

    vector<vector<char>> landscape_char_grid(landscape.rows(),
                                             vector<char>(landscape.columns(), '%'));

    landscape.toGrid(landscape_char_grid);

    ostream_iterator<char> os_it(cout, "");
    for(auto& row : landscape_char_grid) {
        copy(row, os_it);
        cout << "\n";
    }
}

