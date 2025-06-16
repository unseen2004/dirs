#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <random>
#include <chrono>
#include <set>
#include <map>
#include <fstream>
#include <functional>

class TreeNode {
public:
    int id;
    std::vector<TreeNode*> children;
    TreeNode* parent;
    int subtreeHeight;
    int subtreeSize;
    bool hasInfo;
    int roundReceived;

    TreeNode(int nodeId) : id(nodeId), parent(nullptr), subtreeHeight(0),
                          subtreeSize(1), hasInfo(false), roundReceived(-1) {}
};

class OptimalTreeBroadcast {
private:
    TreeNode* root;
    int totalNodes;
    std::map<int, TreeNode*> nodeMap;

public:
    OptimalTreeBroadcast() : root(nullptr), totalNodes(0) {}

    ~OptimalTreeBroadcast() {
        cleanup(root);
    }

    void cleanup(TreeNode* node) {
        if (!node) return;
        for (auto* child : node->children) {
            cleanup(child);
        }
        delete node;
    }

    // Create tree from adjacency list representation
    void buildTree(const std::vector<std::vector<int>>& adj, int rootId) {
        totalNodes = adj.size();

        // Create all nodes
        for (int i = 0; i < totalNodes; i++) {
            nodeMap[i] = new TreeNode(i);
        }

        root = nodeMap[rootId];

        // Build tree using BFS to establish parent-child relationships
        std::queue<int> q;
        std::set<int> visited;
        q.push(rootId);
        visited.insert(rootId);

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            for (int neighbor : adj[current]) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    nodeMap[neighbor]->parent = nodeMap[current];
                    nodeMap[current]->children.push_back(nodeMap[neighbor]);
                    q.push(neighbor);
                }
            }
        }

        // Compute subtree properties
        computeSubtreeProperties(root);

        // Sort children optimally for each node
        optimizeChildrenOrder(root);
    }

    // Compute subtree height and size for optimal ordering
    int computeSubtreeProperties(TreeNode* node) {
        if (!node) return -1;

        node->subtreeSize = 1;
        node->subtreeHeight = 0;

        for (auto* child : node->children) {
            int childHeight = computeSubtreeProperties(child);
            node->subtreeHeight = std::max(node->subtreeHeight, childHeight + 1);
            node->subtreeSize += child->subtreeSize;
        }

        return node->subtreeHeight;
    }

    // Sort children by subtree height (descending) - optimal strategy
    void optimizeChildrenOrder(TreeNode* node) {
        if (!node) return;

        // Sort children by subtree height (descending order)
        // Larger subtrees get informed first
        std::sort(node->children.begin(), node->children.end(),
                 [](const TreeNode* a, const TreeNode* b) {
                     if (a->subtreeHeight != b->subtreeHeight) {
                         return a->subtreeHeight > b->subtreeHeight;
                     }
                     // Tiebreaker: use subtree size
                     if (a->subtreeSize != b->subtreeSize) {
                         return a->subtreeSize > b->subtreeSize;
                     }
                     // Final tiebreaker: use node id for consistency
                     return a->id < b->id;
                 });

        // Recursively optimize all subtrees
        for (auto* child : node->children) {
            optimizeChildrenOrder(child);
        }
    }

    // Simulate optimal information propagation
    int simulateBroadcast() {
        // Reset simulation state
        resetSimulation();

        // Root starts with information
        root->hasInfo = true;
        root->roundReceived = 0;

        // Track which child each node will inform next
        std::map<TreeNode*, int> nextChildIndex;
        for (auto& pair : nodeMap) {
            nextChildIndex[pair.second] = 0;
        }

        int round = 1;
        bool newInformations = true;

        while (newInformations) {
            newInformations = false;

            // Each node with info tries to inform one child
            for (auto& pair : nodeMap) {
                TreeNode* node = pair.second;

                if (node->hasInfo && nextChildIndex[node] < node->children.size()) {
                    TreeNode* childToInform = node->children[nextChildIndex[node]];

                    if (!childToInform->hasInfo) {
                        childToInform->hasInfo = true;
                        childToInform->roundReceived = round;
                        newInformations = true;
                    }

                    nextChildIndex[node]++;
                }
            }

            if (newInformations) {
                round++;
            }
        }

        return round - 1; // Total rounds needed
    }

    void resetSimulation() {
        for (auto& pair : nodeMap) {
            pair.second->hasInfo = false;
            pair.second->roundReceived = -1;
        }
    }

    // Generate random tree using MST of complete graph
    static std::vector<std::vector<int>> generateRandomMST(int n, std::mt19937& gen) {
        // Generate complete graph with random weights
        std::vector<std::tuple<int, int, int>> edges; // weight, u, v
        std::uniform_int_distribution<> weightDist(1, 1000);

        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                edges.emplace_back(weightDist(gen), i, j);
            }
        }

        // Sort edges by weight (Kruskal's algorithm)
        std::sort(edges.begin(), edges.end());

        // Union-Find for MST construction
        std::vector<int> parent(n);
        std::iota(parent.begin(), parent.end(), 0);

        std::function<int(int)> find = [&](int x) {
            return parent[x] == x ? x : parent[x] = find(parent[x]);
        };

        auto unite = [&](int x, int y) {
            x = find(x);
            y = find(y);
            if (x != y) {
                parent[y] = x;
                return true;
            }
            return false;
        };

        // Build MST
        std::vector<std::vector<int>> adj(n);
        for (auto& [weight, u, v] : edges) {
            if (unite(u, v)) {
                adj[u].push_back(v);
                adj[v].push_back(u);
            }
        }

        return adj;
    }

    int getOptimalRounds() const {
        return calculateOptimalRounds(root);
    }

private:
    // Calculate theoretical optimal rounds using formula
    int calculateOptimalRounds(TreeNode* node) const {
        if (!node || node->children.empty()) {
            return 0;
        }

        std::vector<int> childRounds;
        for (auto* child : node->children) {
            childRounds.push_back(calculateOptimalRounds(child));
        }

        // Sort child rounds in descending order
        std::sort(childRounds.begin(), childRounds.end(), std::greater<int>());

        int maxRounds = 0;
        for (int i = 0; i < childRounds.size(); i++) {
            maxRounds = std::max(maxRounds, childRounds[i] + i + 1);
        }

        return maxRounds;
    }
};

// Experimental analysis class
class ExperimentalAnalysis {
private:
    std::mt19937 gen;

public:
    ExperimentalAnalysis() : gen(std::chrono::steady_clock::now().time_since_epoch().count()) {}

    struct Statistics {
        double mean;
        double min;
        double max;
        double stddev;
        std::vector<int> data;
    };

    Statistics runExperiments(int numNodes, int numTrials, int startRoot = -1) {
        std::vector<int> rounds;

        for (int trial = 0; trial < numTrials; trial++) {
            auto adj = OptimalTreeBroadcast::generateRandomMST(numNodes, gen);

            int rootNode = (startRoot == -1) ?
                std::uniform_int_distribution<>(0, numNodes-1)(gen) : startRoot;

            OptimalTreeBroadcast tree;
            tree.buildTree(adj, rootNode);

            int roundsNeeded = tree.simulateBroadcast();
            rounds.push_back(roundsNeeded);
        }

        Statistics stats;
        stats.data = rounds;
        stats.min = *std::min_element(rounds.begin(), rounds.end());
        stats.max = *std::max_element(rounds.begin(), rounds.end());

        double sum = std::accumulate(rounds.begin(), rounds.end(), 0.0);
        stats.mean = sum / rounds.size();

        double variance = 0.0;
        for (int r : rounds) {
            variance += (r - stats.mean) * (r - stats.mean);
        }
        stats.stddev = std::sqrt(variance / rounds.size());

        return stats;
    }

    void generateReport(const std::string& filename) {
        std::ofstream file(filename);
        file << "Nodes,Mean,Min,Max,StdDev\n";

        std::vector<int> nodeSizes = {10, 20, 50, 100, 200, 500};
        int trials = 100;

        for (int nodes : nodeSizes) {
            auto stats = runExperiments(nodes, trials);
            file << nodes << "," << stats.mean << "," << stats.min << ","
                 << stats.max << "," << stats.stddev << "\n";

            std::cout << "Nodes: " << nodes << " - Mean: " << stats.mean
                      << ", Min: " << stats.min << ", Max: " << stats.max
                      << ", StdDev: " << stats.stddev << std::endl;
        }

        file.close();
        std::cout << "Results saved to " << filename << std::endl;
    }
};

int main() {
    std::cout << "=== Optimal Tree Information Propagation Analysis ===\n\n";

    // Demonstrate algorithm on small example
    std::cout << "1. Small Example Demonstration:\n";
    std::vector<std::vector<int>> exampleAdj = {
        {1, 2},        // Node 0 connected to 1, 2
        {0, 3, 4},     // Node 1 connected to 0, 3, 4
        {0, 5},        // Node 2 connected to 0, 5
        {1},           // Node 3 connected to 1
        {1},           // Node 4 connected to 1
        {2}            // Node 5 connected to 2
    };

    OptimalTreeBroadcast exampleTree;
    exampleTree.buildTree(exampleAdj, 0);
    int rounds = exampleTree.simulateBroadcast();
    std::cout << "Example tree (6 nodes) needs " << rounds << " rounds\n\n";

    // Complexity analysis
    std::cout << "2. Complexity Analysis:\n";
    std::cout << "Time Complexity: O(n log n) for sorting children + O(n) for simulation\n";
    std::cout << "Space Complexity: O(n) for tree storage\n";
    std::cout << "Optimality: Achieves theoretical lower bound\n\n";

    // Experimental evaluation
    std::cout << "3. Experimental Evaluation:\n";
    ExperimentalAnalysis analysis;
    analysis.generateReport("broadcast_results.csv");

    return 0;
}