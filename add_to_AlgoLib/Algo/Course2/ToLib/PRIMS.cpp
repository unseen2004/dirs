#include <iostream>
#include <vector>
#include <queue>
#include <limits>

template <typename T>
class PrimMST {
private:
    std::vector<std::vector<std::pair<int, T>>> m_graph;
    std::vector<T> m_key;
    std::vector<int> m_parent;
    std::vector<bool> m_inMST;

public:
    PrimMST(const std::vector<std::vector<std::pair<int, T>>>& graph) : m_graph(graph) {
        int n = static_cast<int>(m_graph.size());
        m_key.resize(n, std::numeric_limits<T>::max());
        m_parent.resize(n, -1);
        m_inMST.resize(n, false);
    }

    void run() {
        std::priority_queue<std::pair<T, int>, std::vector<std::pair<T, int>>, std::greater<std::pair<T, int>>> pq;
        m_key[0] = 0;
        pq.push({0, 0});
        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();
            if (m_inMST[u]) continue;
            m_inMST[u] = true;
            for (auto& edge : m_graph[u]) {
                int v = edge.first;
                T weight = edge.second;
                if (!m_inMST[v] && weight < m_key[v]) {
                    m_key[v] = weight;
                    m_parent[v] = u;
                    pq.push({m_key[v], v});
                }
            }
        }
    }

    void printMST(std::ostream& os) {
        for (int i = 1; i < static_cast<int>(m_graph.size()); i++) {
            os << "Edge: " << m_parent[i] << " - " << i << " | Weight: " << m_key[i] << "\n";
        }
    }
};
