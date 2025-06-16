#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <cassert>

class BinomialHeap {
private:
    struct Node {
        int key;
        int degree;
        Node* parent;
        Node* child;
        Node* sibling;
        
        Node(int k) : key(k), degree(0), parent(nullptr), child(nullptr), sibling(nullptr) {}
    };
    
    Node* head;
    static long long comparison_count;
    
    // Helper function to compare keys and count comparisons
    bool compare_keys(int a, int b) {
        comparison_count++;
        return a <= b;
    }
    
    // Link two binomial trees of the same degree
    void link(Node* y, Node* z) {
        y->parent = z;
        y->sibling = z->child;
        z->child = y;
        z->degree++;
    }
    
    // Merge two binomial heap root lists
    Node* merge_root_lists(Node* h1, Node* h2) {
        if (!h1) return h2;
        if (!h2) return h1;
        
        Node* head = nullptr;
        Node* tail = nullptr;
        
        while (h1 && h2) {
            Node* next;
            if (h1->degree <= h2->degree) {
                next = h1;
                h1 = h1->sibling;
            } else {
                next = h2;
                h2 = h2->sibling;
            }
            
            if (!head) {
                head = tail = next;
            } else {
                tail->sibling = next;
                tail = next;
            }
        }
        
        if (h1) tail->sibling = h1;
        if (h2) tail->sibling = h2;
        
        return head;
    }
    
    // Union operation helper
    Node* union_helper(Node* h1, Node* h2) {
        Node* h = merge_root_lists(h1, h2);
        if (!h) return nullptr;
        
        Node* prev_x = nullptr;
        Node* x = h;
        Node* next_x = x->sibling;
        
        while (next_x) {
            if ((x->degree != next_x->degree) || 
                (next_x->sibling && next_x->sibling->degree == x->degree)) {
                prev_x = x;
                x = next_x;
            } else {
                if (compare_keys(x->key, next_x->key)) {
                    x->sibling = next_x->sibling;
                    link(next_x, x);
                } else {
                    if (!prev_x) {
                        h = next_x;
                    } else {
                        prev_x->sibling = next_x;
                    }
                    link(x, next_x);
                    x = next_x;
                }
            }
            next_x = x->sibling;
        }
        
        return h;
    }
    
    // Find minimum node
    Node* find_min_node() {
        if (!head) return nullptr;
        
        Node* min_node = head;
        Node* current = head->sibling;
        
        while (current) {
            if (compare_keys(current->key, min_node->key)) {
                min_node = current;
            }
            current = current->sibling;
        }
        
        return min_node;
    }
    
    // Reverse the order of siblings
    Node* reverse_sibling_list(Node* node) {
        Node* prev = nullptr;
        Node* current = node;
        
        while (current) {
            Node* next = current->sibling;
            current->sibling = prev;
            current->parent = nullptr;
            prev = current;
            current = next;
        }
        
        return prev;
    }
    
public:
    BinomialHeap() : head(nullptr) {}
    
    static void reset_comparison_count() {
        comparison_count = 0;
    }
    
    static long long get_comparison_count() {
        return comparison_count;
    }
    
    // Make heap (constructor already does this)
    void make_heap() {
        head = nullptr;
    }
    
    // Insert operation
    void insert(int key) {
        BinomialHeap temp_heap;
        temp_heap.head = new Node(key);
        head = union_helper(head, temp_heap.head);
        temp_heap.head = nullptr; // Prevent destructor from deleting the node
    }
    
    // Union operation
    void union_with(BinomialHeap& other) {
        head = union_helper(head, other.head);
        other.head = nullptr; // Transfer ownership
    }
    
    // Extract minimum
    int extract_min() {
        if (!head) throw std::runtime_error("Heap is empty");
        
        Node* min_node = find_min_node();
        int min_key = min_node->key;
        
        // Remove min_node from root list
        if (min_node == head) {
            head = head->sibling;
        } else {
            Node* current = head;
            while (current->sibling != min_node) {
                current = current->sibling;
            }
            current->sibling = min_node->sibling;
        }
        
        // Create new heap from children of min_node
        BinomialHeap child_heap;
        child_heap.head = reverse_sibling_list(min_node->child);
        
        // Union with remaining heap
        head = union_helper(head, child_heap.head);
        child_heap.head = nullptr;
        
        delete min_node;
        return min_key;
    }
    
    // Check if heap is empty
    bool is_empty() {
        return head == nullptr;
    }
    
    // Get minimum without extracting
    int get_min() {
        if (!head) throw std::runtime_error("Heap is empty");
        Node* min_node = find_min_node();
        return min_node->key;
    }
    
    // Destructor
    ~BinomialHeap() {
        clear();
    }
    
private:
    void clear_node(Node* node) {
        if (!node) return;
        clear_node(node->child);
        clear_node(node->sibling);
        delete node;
    }
    
    void clear() {
        clear_node(head);
        head = nullptr;
    }
};

// Static member definition
long long BinomialHeap::comparison_count = 0;

// Experiment class
class BinomialHeapExperiment {
private:
    std::mt19937 rng;
    
public:
    BinomialHeapExperiment() : rng(std::random_device{}()) {}
    
    std::vector<int> generate_random_sequence(int n) {
        std::vector<int> sequence(n);
        std::uniform_int_distribution<int> dist(1, 10000);
        
        for (int i = 0; i < n; i++) {
            sequence[i] = dist(rng);
        }
        
        return sequence;
    }
    
    std::vector<long long> run_single_experiment(int n) {
        BinomialHeap::reset_comparison_count();
        std::vector<long long> operation_comparisons;
        
        // Step 1: Create two empty heaps
        BinomialHeap H1, H2;
        
        // Step 2: Insert random sequences
        auto seq1 = generate_random_sequence(n);
        auto seq2 = generate_random_sequence(n);
        
        for (int val : seq1) {
            long long before = BinomialHeap::get_comparison_count();
            H1.insert(val);
            long long after = BinomialHeap::get_comparison_count();
            operation_comparisons.push_back(after - before);
        }
        
        for (int val : seq2) {
            long long before = BinomialHeap::get_comparison_count();
            H2.insert(val);
            long long after = BinomialHeap::get_comparison_count();
            operation_comparisons.push_back(after - before);
        }
        
        // Step 3: Union heaps
        long long before_union = BinomialHeap::get_comparison_count();
        H1.union_with(H2);
        long long after_union = BinomialHeap::get_comparison_count();
        operation_comparisons.push_back(after_union - before_union);
        
        // Step 4: Extract all elements and verify ordering
        std::vector<int> extracted_elements;
        for (int i = 0; i < 2 * n; i++) {
            long long before = BinomialHeap::get_comparison_count();
            int min_val = H1.extract_min();
            long long after = BinomialHeap::get_comparison_count();
            operation_comparisons.push_back(after - before);
            extracted_elements.push_back(min_val);
        }
        
        // Verify that elements are in sorted order
        for (int i = 1; i < extracted_elements.size(); i++) {
            assert(extracted_elements[i-1] <= extracted_elements[i]);
        }
        
        // Verify heap is empty
        assert(H1.is_empty());
        
        return operation_comparisons;
    }
    
    void run_experiments_for_n(int n, int num_experiments = 5) {
        std::cout << "Running experiments for n = " << n << std::endl;
        
        for (int exp = 1; exp <= num_experiments; exp++) {
            auto comparisons = run_single_experiment(n);
            
            // Save to file
            std::string filename = "experiment_n" + std::to_string(n) + "_run" + std::to_string(exp) + ".csv";
            std::ofstream file(filename);
            file << "Operation,Comparisons\n";
            
            for (int i = 0; i < comparisons.size(); i++) {
                file << i + 1 << "," << comparisons[i] << "\n";
            }
            
            file.close();
            std::cout << "Experiment " << exp << " completed. Total comparisons: " 
                      << std::accumulate(comparisons.begin(), comparisons.end(), 0LL) << std::endl;
        }
    }
    
    void run_scaling_experiment() {
        std::cout << "Running scaling experiments..." << std::endl;
        
        std::ofstream scaling_file("scaling_results.csv");
        scaling_file << "n,Average_Cost_Per_Operation\n";
        
        for (int n = 100; n <= 10000; n += 100) {
            long long total_comparisons = 0;
            int num_runs = 3; // Reduced for time efficiency
            
            for (int run = 0; run < num_runs; run++) {
                auto comparisons = run_single_experiment(n);
                total_comparisons += std::accumulate(comparisons.begin(), comparisons.end(), 0LL);
            }
            
            double average_cost = static_cast<double>(total_comparisons) / (num_runs * n);
            scaling_file << n << "," << std::fixed << std::setprecision(6) << average_cost << "\n";
            
            std::cout << "n = " << n << ", Average cost per operation: " << average_cost << std::endl;
        }
        
        scaling_file.close();
    }
};

int main() {
    BinomialHeapExperiment experiment;
    
    std::cout << "=== Binomial Heap Experiment ===" << std::endl;
    
    // Run experiments for n = 500
    experiment.run_experiments_for_n(500, 5);
    
    std::cout << "\n=== Scaling Experiment ===" << std::endl;
    experiment.run_scaling_experiment();
    
    std::cout << "\nAll experiments completed!" << std::endl;
    std::cout << "Check the generated CSV files for detailed results." << std::endl;
    
    return 0;
}