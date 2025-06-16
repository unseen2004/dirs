#include <vector>

class Heap {
private:
    int length;
    std::vector<int> data;

public:
    Heap() : length(0), data() {}

    ~Heap() {
        while (length > 0) {
            deleteMin();
        }
    }

    void insert(int n) {
        data.push_back(n);
        heapifyUp(length);
        ++length;
    }

    int deleteMin() {
        if (length == 0) {
            return -1;
        }

        int out = data[0];

        --length;
        if (length == 0) {
            data.pop_back();
            return out;
        }

        data[0] = data[length];
        data.pop_back();
        heapifyDown(0);

        return out;
    }

private:
    int parent(int idx) {
        return (idx - 1) / 2;
    }

    int leftChild(int idx) {
        return 2 * idx + 1;
    }

    int rightChild(int idx) {
        return 2 * idx + 2;
    }

    void heapifyUp(int idx) {
        if (idx == 0) return;

        int p = parent(idx);
        if (data[p] > data[idx]) {
            std::swap(data[p], data[idx]);
            heapifyUp(p);
        }
    }

    void heapifyDown(int idx) {
        int lIdx = leftChild(idx);
        int rIdx = rightChild(idx);
        int smallest = idx;

        if (lIdx < length && data[lIdx] < data[smallest]) {
            smallest = lIdx;
        }

        if (rIdx < length && data[rIdx] < data[smallest]) {
            smallest = rIdx;
        }
        
        if (smallest != idx) {
            std::swap(data[idx], data[smallest]);
            heapifyDown(smallest);
        }
    }
};

