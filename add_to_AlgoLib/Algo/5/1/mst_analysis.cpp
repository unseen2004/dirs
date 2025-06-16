#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <queue>
#include <fstream>
#include <iomanip>

using namespace std;
using namespace std::chrono;

struct Edge {
    int u, v;
    double weight;

    Edge(int u, int v, double weight) : u(u), v(v), weight(weight) {}

    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

struct PrimEdge {
    int vertex;
    double weight;

    PrimEdge(int vertex, double weight) : vertex(vertex), weight(weight) {}

    bool operator>(const PrimEdge& other) const {
        return weight > other.weight;
    }
};

class UnionFind {
private:
    vector<int> parent, rank;

public:
    UnionFind(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // Path compression
        }
        return parent[x];
    }

    bool unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);

        if (rootX == rootY) return false; // Already connected

        // Union by rank
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
        return true;
    }
};

class CompleteGraph {
private:
    int n; // Number of vertices
    vector<vector<double>> adjMatrix; // Adjacency matrix for weights
    vector<Edge> edges; // List of all edges

public:
    CompleteGraph(int vertices) : n(vertices) {
        adjMatrix.resize(n, vector<double>(n, 0.0));
        generateRandomWeights();
    }

    void generateRandomWeights() {
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<double> dis(0.0, 1.0);

        edges.clear();

        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                double weight = dis(gen);
                adjMatrix[i][j] = weight;
                adjMatrix[j][i] = weight; // Symmetric matrix
                edges.emplace_back(i, j, weight);
            }
        }
    }

    // Get adjacency matrix
    const vector<vector<double>>& getAdjMatrix() const {
        return adjMatrix;
    }

    // Get list of edges
    const vector<Edge>& getEdges() const {
        return edges;
    }

    int getVertexCount() const {
        return n;
    }
};

class PrimMST {
public:
    static pair<double, long long> findMST(const CompleteGraph& graph) {
        auto start = high_resolution_clock::now();

        int n = graph.getVertexCount();
        const auto& adjMatrix = graph.getAdjMatrix();

        vector<bool> inMST(n, false);
        priority_queue<PrimEdge, vector<PrimEdge>, greater<PrimEdge>> pq;
        double totalWeight = 0.0;

        // Start from vertex 0
        inMST[0] = true;

        // Add all edges from vertex 0 to priority queue
        for (int i = 1; i < n; i++) {
            pq.emplace(i, adjMatrix[0][i]);
        }

        int edgesAdded = 0;
        while (!pq.empty() && edgesAdded < n - 1) {
            PrimEdge current = pq.top();
            pq.pop();

            if (inMST[current.vertex]) continue;

            // Add vertex to MST
            inMST[current.vertex] = true;
            totalWeight += current.weight;
            edgesAdded++;

            // Add new edges to priority queue
            for (int i = 0; i < n; i++) {
                if (!inMST[i]) {
                    pq.emplace(i, adjMatrix[current.vertex][i]);
                }
            }
        }

        auto end = high_resolution_clock::now();
        long long duration = duration_cast<microseconds>(end - start).count();

        return {totalWeight, duration};
    }
};

class KruskalMST {
public:
    static pair<double, long long> findMST(const CompleteGraph& graph) {
        auto start = high_resolution_clock::now();

        int n = graph.getVertexCount();
        vector<Edge> edges = graph.getEdges();

        // Sort edges by weight
        sort(edges.begin(), edges.end());

        UnionFind uf(n);
        double totalWeight = 0.0;
        int edgesAdded = 0;

        for (const Edge& edge : edges) {
            if (uf.unite(edge.u, edge.v)) {
                totalWeight += edge.weight;
                edgesAdded++;

                if (edgesAdded == n - 1) break; // MST complete
            }
        }

        auto end = high_resolution_clock::now();
        long long duration = duration_cast<microseconds>(end - start).count();

        return {totalWeight, duration};
    }
};

class MST_Analyzer {
private:
    int nMin, nMax, step, rep;

public:
    MST_Analyzer(int nMin, int nMax, int step, int rep)
        : nMin(nMin), nMax(nMax), step(step), rep(rep) {}

    void runExperiments() {
        cout << "Starting MST Algorithm Analysis..." << endl;
        cout << "Parameters: nMin=" << nMin << ", nMax=" << nMax
             << ", step=" << step << ", rep=" << rep << endl;

        // Open file for results
        ofstream outFile("mst_results.csv");
        outFile << "n,prim_avg_time,kruskal_avg_time,prim_avg_weight,kruskal_avg_weight" << endl;

        for (int n = nMin; n <= nMax; n += step) {
            cout << "Testing n = " << n << "..." << endl;

            double primTotalTime = 0.0, kruskalTotalTime = 0.0;
            double primTotalWeight = 0.0, kruskalTotalWeight = 0.0;

            for (int r = 0; r < rep; r++) {
                // Generate random complete graph
                CompleteGraph graph(n);

                // Test Prim's algorithm
                auto primResult = PrimMST::findMST(graph);
                primTotalTime += primResult.second;
                primTotalWeight += primResult.first;

                // Test Kruskal's algorithm on same graph
                auto kruskalResult = KruskalMST::findMST(graph);
                kruskalTotalTime += kruskalResult.second;
                kruskalTotalWeight += kruskalResult.first;
            }

            // Calculate averages
            double primAvgTime = primTotalTime / rep;
            double kruskalAvgTime = kruskalTotalTime / rep;
            double primAvgWeight = primTotalWeight / rep;
            double kruskalAvgWeight = kruskalTotalWeight / rep;

            // Output results
            cout << "  Prim's avg time: " << fixed << setprecision(2)
                 << primAvgTime << " μs" << endl;
            cout << "  Kruskal's avg time: " << fixed << setprecision(2)
                 << kruskalAvgTime << " μs" << endl;
            cout << "  Prim's avg MST weight: " << fixed << setprecision(6)
                 << primAvgWeight << endl;
            cout << "  Kruskal's avg MST weight: " << fixed << setprecision(6)
                 << kruskalAvgWeight << endl;

            // Write to file
            outFile << n << "," << primAvgTime << "," << kruskalAvgTime
                    << "," << primAvgWeight << "," << kruskalAvgWeight << endl;
        }

        outFile.close();
        cout << "Results saved to mst_results.csv" << endl;
    }
};

void demonstrateAlgorithms() {
    cout << "=== Algorithm Demonstration ===" << endl;
    const int n = 5;
    CompleteGraph graph(n);

    cout << "Generated " << n << "-vertex complete graph:" << endl;
    const auto& adjMatrix = graph.getAdjMatrix();

    cout << "Adjacency matrix:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << fixed << setprecision(3) << adjMatrix[i][j] << " ";
        }
        cout << endl;
    }

    // Run Prim's algorithm
    auto primResult = PrimMST::findMST(graph);
    cout << "\nPrim's MST weight: " << fixed << setprecision(6)
         << primResult.first << endl;
    cout << "Prim's execution time: " << primResult.second << " μs" << endl;

    // Run Kruskal's algorithm
    auto kruskalResult = KruskalMST::findMST(graph);
    cout << "Kruskal's MST weight: " << fixed << setprecision(6)
         << kruskalResult.first << endl;
    cout << "Kruskal's execution time: " << kruskalResult.second << " μs" << endl;

    cout << "\n=== End Demonstration ===" << endl << endl;
}

int main() {
    cout << "MST Algorithms Implementation and Analysis" << endl;
    cout << "==========================================" << endl;

    // Demonstrate algorithms on small graph
    demonstrateAlgorithms();

    // Experimental parameters - adjust these based on your requirements
    int nMin = 10;      // Minimum graph size
    int nMax = 100;     // Maximum graph size
    int step = 10;      // Step size
    int rep = 5;        // Repetitions per graph size

    cout << "Starting experimental analysis..." << endl;
    cout << "Note: You can adjust nMin, nMax, step, and rep in main() function" << endl;
    cout << "Current settings will test graphs from " << nMin << " to " << nMax
         << " vertices with step " << step << " and " << rep << " repetitions each." << endl;

    char proceed;
    cout << "Proceed with analysis? (y/n): ";
    cin >> proceed;

    if (proceed == 'y' || proceed == 'Y') {
        MST_Analyzer analyzer(nMin, nMax, step, rep);
        analyzer.runExperiments();

        cout << "\nAnalysis complete!" << endl;
        cout << "To visualize results, you can use the generated CSV file with:" << endl;
        cout << "- Python + matplotlib" << endl;
        cout << "- R + ggplot2" << endl;
        cout << "- Excel or similar spreadsheet software" << endl;
    }

    return 0;
}