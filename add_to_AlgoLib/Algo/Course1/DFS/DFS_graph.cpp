#include <vector>

bool walk(const std::vector<std::vector<std::pair<int, int>>>& graph, int curr, int target, std::vector<bool>& seen, std::vector<int>& path) {
    if (seen[curr]) {
        return false;
    }

    seen[curr] = true;
    path.push_back(curr);

    if (curr == target) {
        return true;
    }

    const std::vector<std::pair<int, int>>& list = graph[curr];
    for (const auto& edge : list) {
        if (walk(graph, edge.first, target, seen, path)) {
            return true;
        }
    }

    path.pop_back();
    return false;
}

std::vector<int> dfs(const std::vector<std::vector<std::pair<int, int>>>& graph, int source, int target) {
    std::vector<bool> seen(graph.size(), false);
    std::vector<int> path;

    walk(graph, source, target, seen, path);

    return path;
}


