#include "BinarySearch.hpp"

bool binarySearch(const std::vector<int>& A, int left, int right, int v, Counter& cnt, bool verbose) {
    if (left > right) {
        if (verbose) {
            std::cout << "Value " << v << " not found in range [" << left << "," << right << "]\n";
        }
        return false;
    }
    int mid = left + (right - left) / 2;
    cnt.comparisons++;
    if (verbose) {
        std::cout << "Compare v=" << v << " with A[" << mid << "]=" << A[mid] << "\n";
    }
    if (A[mid] == v) {
        if (verbose) {
            std::cout << "Found at index " << mid << "\n";
        }
        return true;
    }
    else if (A[mid] < v) {
        return binarySearch(A, mid + 1, right, v, cnt, verbose);
    }
    else {
        return binarySearch(A, left, mid - 1, v, cnt, verbose);
    }
}
