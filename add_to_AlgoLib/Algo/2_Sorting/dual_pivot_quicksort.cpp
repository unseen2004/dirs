#include <iostream>
#include <vector>
#include <iomanip>
#include <cassert>
using namespace std;

long long cmp_count = 0, swap_count = 0;

bool cmp(const int& a, const int& b) {
    ++cmp_count;
    return a < b;
}
void swap_keys(int& a, int& b) {
    ++swap_count;
    std::swap(a, b);
}
void print_array(const vector<int>& arr) {
    for (auto v : arr) cout << setw(2) << v << " ";
    cout << endl;
}
bool is_sorted(const vector<int>& a) {
    for (size_t i = 1; i < a.size(); ++i)
        if (a[i - 1] > a[i]) return false;
    return true;
}

// Dual-pivot partition as per COUNT strategy
void dual_pivot_quicksort(vector<int>& arr, int left, int right, int n, bool print_steps = false) {
    if (left >= right) return;
    // Choose 2 pivots: p = arr[left], q = arr[right]
    int p = arr[left], q = arr[right];
    if (!cmp(p, q)) swap_keys(arr[left], arr[right]), p = arr[left], q = arr[right];
    int lt = left + 1, gt = right - 1, i = left + 1;
    int s = 0, l = 0; // counts of small (less than p) and large (greater than q) seen so far
    while (i <= gt) {
        // COUNT strategy
        if (l > s) {
            // Compare with q first
            if (cmp(q, arr[i])) { // arr[i] > q
                swap_keys(arr[i], arr[gt]);
                gt--; l++;
            } else {
                // arr[i] <= q
                if (cmp(arr[i], p)) { // arr[i] < p
                    swap_keys(arr[i], arr[lt]);
                    lt++; s++;
                }
                i++;
            }
        } else {
            // Compare with p first
            if (cmp(arr[i], p)) { // arr[i] < p
                swap_keys(arr[i], arr[lt]);
                lt++; s++;
                i++;
            } else {
                // arr[i] >= p
                if (cmp(q, arr[i])) { // arr[i] > q
                    swap_keys(arr[i], arr[gt]);
                    gt--; l++;
                } else {
                    // p <= arr[i] <= q
                    i++;
                }
            }
        }
        if (print_steps && n < 40) {
            cout << "Step: "; print_array(arr);
        }
    }
    lt--; gt++;
    swap_keys(arr[left], arr[lt]);
    swap_keys(arr[right], arr[gt]);
    if (print_steps && n < 40) {
        cout << "Pivots placed: "; print_array(arr);
    }
    dual_pivot_quicksort(arr, left, lt - 1, n, print_steps);
    dual_pivot_quicksort(arr, lt + 1, gt - 1, n, print_steps);
    dual_pivot_quicksort(arr, gt + 1, right, n, print_steps);
}

int main() {
    int n;
    cin >> n;
    vector<int> arr(n);
    for (auto& x : arr) cin >> x;

    bool print_steps = n < 40;
    if (print_steps) {
        cout << "Input:        "; print_array(arr);
    }
    vector<int> input = arr;

    dual_pivot_quicksort(arr, 0, n - 1, n, print_steps);

    if (print_steps) {
        cout << "Input again:  "; print_array(input);
        cout << "Sorted:       "; print_array(arr);
    }
    cout << "Comparisons: " << cmp_count << endl;
    cout << "Swaps:       " << swap_count << endl;
    assert(is_sorted(arr));
    return 0;
}