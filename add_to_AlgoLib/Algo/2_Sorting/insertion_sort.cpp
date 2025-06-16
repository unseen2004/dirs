#include <iostream>
#include <vector>
#include <iomanip>
#include <cassert>

using namespace std;

long long cmp_count = 0, swap_count = 0;

bool cmp(const int &a, const int &b) {
    ++cmp_count;
    return a < b;
}
void swap_keys(int &a, int &b) {
    ++swap_count;
    std::swap(a, b);
}
void print_array(const vector<int> &arr) {
    for (auto v : arr) cout << setw(2) << v << " ";
    cout << endl;
}
bool is_sorted(const vector<int> &a) {
    for (size_t i = 1; i < a.size(); ++i)
        if (a[i - 1] > a[i]) return false;
    return true;
}

int main() {
    int n;
    cin >> n;
    vector<int> arr(n);
    for (auto &x : arr) cin >> x;

    if (n < 40) {
        cout << "Input:        "; print_array(arr);
    }
    vector<int> input = arr;
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && cmp(key, arr[j])) {
            swap_keys(arr[j + 1], arr[j]);
            --j;
            if (n < 40) {
                cout << "Step i=" << setw(2) << i << ":   ";
                print_array(arr);
            }
        }
        arr[j + 1] = key;
    }
    if (n < 40) {
        cout << "Input again:  "; print_array(input);
        cout << "Sorted:       "; print_array(arr);
    }
    cout << "Comparisons: " << cmp_count << endl;
    cout << "Swaps:       " << swap_count << endl;

    assert(is_sorted(arr));
    return 0;
}