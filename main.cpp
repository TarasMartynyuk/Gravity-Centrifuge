#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>

using namespace std;

class Landscape {
public:
    

    int width() { return tile}

private:
    // row : # blocks in that row
    vector<char>* row_block_counts;
    // col : # blocks in that col
    vector<char>* col_block_counts;
};

int main()
{
    int width;
    int height;
    cin >> width >> height; cin.ignore();
    int count;
    cin >> count; cin.ignore();
    for (int i = 0; i < height; i++) {
        string tile_row;
        getline(cin, tile_row);
    }

}