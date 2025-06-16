#include <vector>
#include <queue>
#include <algorithm> 

std::vector<int> bfs(const std::vector<std::vector<int>>& graph, int source, int target) {
    std::vector<bool> seen(graph.size(), false);
    std::vector<int> prev(graph.size(), -1);

    seen[source] = true;
    std::queue<int> q;
    q.push(source);

    while (!q.empty()) {
        int curr = q.front();
        q.pop();
        
        if (curr == target) {
            break;
        }

        const std::vector<int>& adjs = graph[curr];
        for (int i = 0; i < adjs.size(); ++i) {
            if (adjs[i] == 0) {
                continue;
            }

            if (seen[i]) {
                continue;
            }

            seen[i] = true;
            prev[i] = curr;
            q.push(i);
        }
    }

    std::vector<int> out;

    if (prev[target] == -1) {
        return out; 
    }

    for (int at = target; at != -1; at = prev[at]) {
        out.push_back(at);
    }
    std::reverse(out.begin(), out.end());
    return out;
}

