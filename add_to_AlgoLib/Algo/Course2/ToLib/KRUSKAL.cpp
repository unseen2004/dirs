#include <vector>
#include <algorithm>

template <typename T>
class UnionFind {
private:
    std::vector<T> m_parent;
    std::vector<T> m_rank;

public:
    UnionFind(T n) {
        m_parent.resize(n);
        m_rank.resize(n, 0);
        for (T i = 0; i < n; ++i) m_parent[i] = i;
    }

    T find(T u) {
        if (m_parent[u] != u) m_parent[u] = find(m_parent[u]);
        return m_parent[u];
    }

    void unionSets(T u, T v) {
        T rootU = find(u);
        T rootV = find(v);
        if (rootU != rootV) {
            if (m_rank[rootU] > m_rank[rootV]) m_parent[rootV] = rootU;
            else if (m_rank[rootU] < m_rank[rootV]) m_parent[rootU] = rootV;
            else {
                m_parent[rootV] = rootU;
                m_rank[rootU]++;
            }
        }
    }
};

template <typename W, typename T>
std::vector<std::pair<T, T>> kruskalMST(std::vector<std::pair<W, std::pair<T, T>>>& edges, T n) {
    std::sort(edges.begin(), edges.end());
    UnionFind<T> uf(n);
    std::vector<std::pair<T, T>> mst;
    for (auto& [weight, edge] : edges) {
        T u = edge.first;
        T v = edge.second;
        if (uf.find(u) != uf.find(v)) {
            uf.unionSets(u, v);
            mst.push_back({u, v});
        }
    }
    return mst;
}
