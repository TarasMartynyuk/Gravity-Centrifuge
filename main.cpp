#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <sstream>
#include <iterator>
#include <bitset>

using namespace std;

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
        assert(all_of(grid.begin(), grid.end(), [this](const vector<char>& row) {
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
        return accumulate(row_block_counts->begin(), row_block_counts->end(), int()) ==
               accumulate(col_block_counts->begin(), col_block_counts->end(), int());
    }
};

class CentrifugeSimulator {
    long total_tumbles = 0;
    int drive_a_momentum = 1;
    int drive_b_momentum = 1;

    const int bits_per_command_portion = 54;

    // interprets the 18 (max even number of full 3-bit pairs that fits into long - one for each oct digit)
    // lowest bits of six_bit_command
    // as a series of commands to centrifuge's drives
    // one command is 2 bits - first for drive A, second for drive B
    void feed6BytesCommandPortion(long commands_portion_number) {
        assert(hasNoMoreLowestBitsThanCommandPortion(commands_portion_number));
        bitset<18> command_portion(commands_portion_number);
    }

    void simulateDoubleCommand(bool driveACommand, bool driveBCommand) {

    };

    // true if the numbers highest set bit is not out of bounds of what
    // a number with bits_per_command_portion bits can represent
    bool hasNoMoreLowestBitsThanCommandPortion(unsigned long number) {
        bitset<54> bits_54;
        bits_54.set(0);
        cerr << bits_54 << endl;
        return bits_54.to_ulong() > number;
    }
};

int getNetTumblingsCount(const string& centrifuge_command) {
    const int oct_digits_in_command_portion = 6;
    CentrifugeSimulator centrifuge;
    if(centrifuge_command.length() <= oct_digits_in_command_portion) {
//        long long
    }

    for (int i = 0; i < 2; ++i) {
        auto three_ocd_digits =  centrifuge_command.substr();
    }
}

int main()
{
    int columns;
    int rows;
    cin >> columns >> rows; cin.ignore();
    string centrifuge_command; getline(cin, centrifuge_command);
    int tumblings_count = getNetTumblingsCount(centrifuge_command);

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
        copy(row.begin(), row.end(), os_it);
        cout << "\n";
    }
}

