#include <iostream>
#include <vector>
#include <queue>
#include <limits>

template <typename FlowType>
class FordFulkerson {
private:
    int m_numVertices;
    std::vector<std::vector<FlowType>> m_capacity;
    std::vector<std::vector<FlowType>> m_residual;

public:
    FordFulkerson(int numVertices) : m_numVertices(numVertices) {
        m_capacity = std::vector<std::vector<FlowType>>(
            numVertices, std::vector<FlowType>(numVertices, static_cast<FlowType>(0))
        );
        m_residual = std::vector<std::vector<FlowType>>(
            numVertices, std::vector<FlowType>(numVertices, static_cast<FlowType>(0))
        );
    }

    void addEdge(int u, int v, FlowType capacity) {
        m_capacity[u][v] = capacity;
        m_residual[u][v] = capacity;
    }

    FlowType maxFlow(int source, int sink) {
        std::vector<int> parent(m_numVertices);
        FlowType maxFlow = static_cast<FlowType>(0);

        while (bfs(source, sink, parent)) {
            FlowType pathFlow = std::numeric_limits<FlowType>::max();

            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                pathFlow = std::min(pathFlow, m_residual[u][v]);
            }

            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                m_residual[u][v] -= pathFlow;
                m_residual[v][u] += pathFlow;
            }

            maxFlow += pathFlow;
        }

        return maxFlow;
    }

    void minCut(int source) {
        std::vector<bool> visited(m_numVertices, false);
        std::queue<int> q;
        q.push(source);
        visited[source] = true;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v = 0; v < m_numVertices; v++) {
                if (m_residual[u][v] > 0 && !visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }

        for (int u = 0; u < m_numVertices; u++) {
            for (int v = 0; v < m_numVertices; v++) {
                if (visited[u] && !visited[v] && m_capacity[u][v] > 0) {
                    std::cout << u << " - " << v << " | Capacity: " << m_capacity[u][v] << std::endl;
                }
            }
        }
    }

private:
    bool bfs(int source, int sink, std::vector<int>& parent) {
        std::fill(parent.begin(), parent.end(), -1);
        std::vector<bool> visited(m_numVertices, false);
        std::queue<int> q;
        q.push(source);
        visited[source] = true;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v = 0; v < m_numVertices; v++) {
                if (!visited[v] && m_residual[u][v] > 0) {
                    q.push(v);
                    visited[v] = true;
                    parent[v] = u;
                    if (v == sink) return true;
                }
            }
        }

        return false;
    }
};
