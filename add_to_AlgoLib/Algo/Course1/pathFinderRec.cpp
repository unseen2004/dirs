
#include <vector>
#include <string>
#include <utility>

const int dir[4][2] = {
    {-1, 0}, // Up
    {1, 0},  // Down
    {0, -1}, // Left
    {0, 1}   // Right
};

bool walk(const std::vector<std::vector<std::string>>& board, const std::string& wall, 
          std::pair<int, int> curr, std::pair<int, int>& end, 
          std::vector<std::vector<bool>>& seen, 
          std::vector<std::pair<int, int>>& path) {
    if (curr.first < 0 || curr.first >= board.size() || 
        curr.second < 0 || curr.second >= board[0].size()) {
        return false;
    }

    if (board[curr.first][curr.second] == wall) {
        return false;
    }

    if (seen[curr.first][curr.second]) {
        return false;
    }

    seen[curr.first][curr.second] = true;

    if (curr.first == end.first && curr.second == end.second) {
        path.push_back(curr);
        return true;
    }

    path.push_back(curr);

    for (int i = 0; i < 4; ++i) {
        const int x = dir[i][0];
        const int y = dir[i][1];
        if (walk(board, wall, {curr.first + x, curr.second + y}, end, seen, path)) {
            return true;
        }
    }

    path.pop_back();

    return false;
}

