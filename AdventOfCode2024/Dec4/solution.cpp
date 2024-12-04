#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <string_view>

void get_data(std::vector<std::string>& out_vec, const std::string& file_name) {
    std::ifstream file(file_name);
    std::string str;
    while (std::getline(file, str)) {
        out_vec.push_back(str);
    }
}


void word_found(const std::string& word, const std::vector<std::string>& v, int w, int h, int row, int col, int& out) {
    int len = word.length();

    // Left
    if (col - (len - 1) >= 0) {
        std::string tmp;
        for (int i = 0; i < len; ++i) {
            tmp += v[row][col - i];
        }
        if (tmp == word) ++out;
    }

    // Right
    if (col + (len - 1) < w) {
        std::string tmp;
        for (int i = 0; i < len; ++i) {
            tmp += v[row][col + i];
        }
        if (tmp == word) ++out;
    }

    // Up
    if (row - (len - 1) >= 0) {
        std::string tmp;
        for (int i = 0; i < len; ++i) {
            tmp += v[row - i][col];
        }
        if (tmp == word) ++out;
    }

    // Down
    if (row + (len - 1) < h) {
        std::string tmp;
        for (int i = 0; i < len; ++i) {
            tmp += v[row + i][col];
        }
        if (tmp == word) ++out;
    }

    // Left-Up 
    if (col - (len - 1) >= 0 && row - (len - 1) >= 0) {
        std::string tmp;
        for (int i = 0; i < len; ++i) {
            tmp += v[row - i][col - i];
        }
        if (tmp == word) ++out;
    }

    // Right-Up 
    if (col + (len - 1) < w && row - (len - 1) >= 0) {
        std::string tmp;
        for (int i = 0; i < len; ++i) {
            tmp += v[row - i][col + i];
        }
        if (tmp == word) ++out;
    }

    // Left-Down 
    if (col - (len - 1) >= 0 && row + (len - 1) < h) {
        std::string tmp;
        for (int i = 0; i < len; ++i) {
            tmp += v[row + i][col - i];
        }
        if (tmp == word) ++out;
    }

    // Right-Down 
    if (col + (len - 1) < w && row + (len - 1) < h) {
        std::string tmp;
        for (int i = 0; i < len; ++i) {
            tmp += v[row + i][col + i];
        }
        if (tmp == word) ++out;
    }
}


bool check_mas(const std::vector<std::string>& grid, int row, int col, int d_row1, int d_col1, int d_row2, int d_col2, int height, int width) {
    char m1 = grid[row + d_row1][col + d_col1];
    char s1 = grid[row + d_row2][col + d_col2];
    std::string word1 = {m1, grid[row][col], s1};

    if (word1 == "MAS" || word1 == "SAM") {
        return true;
    }
    return false;
}

int count_x_mas(const std::vector<std::string>& grid) {
    int count = 0;
    int height = grid.size();
    int width = grid[0].size();

    for (int row = 1; row < height - 1; ++row) {
        for (int col = 1; col < width - 1; ++col) {
            if (grid[row][col] == 'A') {
                bool diag1 = false, diag2 = false;

                if (row - 1 >= 0 && row + 1 < height && col - 1 >= 0 && col + 1 < width) {
                    char m1 = grid[row - 1][col - 1];
                    char s1 = grid[row + 1][col + 1];
                    std::string word1 = { m1, grid[row][col], s1 };

                    if (word1 == "MAS" || word1 == "SAM") {
                        diag1 = true;
                    }
                }

                if (row - 1 >= 0 && row + 1 < height && col + 1 < width && col - 1 >= 0) {
                    char m2 = grid[row - 1][col + 1];
                    char s2 = grid[row + 1][col - 1];
                    std::string word2 = { m2, grid[row][col], s2 };

                    if (word2 == "MAS" || word2 == "SAM") {
                        diag2 = true;
                    }
                }

                if (diag1 && diag2) {
                    ++count;
                }
            }
        }
    }

    return count;
}

int main() {
   std::vector<std::string> input;
   get_data(input, "input.txt");
 

    int height = input.size();
    int width = input[0].size();

    int out = 0;
    const std::string word = "XMAS";

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            word_found(word, input, width, height, i, j, out);
        }
    }

    int out2 = count_x_mas(input);
        
    std::cout << out<<"\n";
    std::cout << out2;

}
