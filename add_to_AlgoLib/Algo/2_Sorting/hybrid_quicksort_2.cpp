#include <iostream>
#include <vector>
#include <iomanip>
#include <cassert>
using namespace std;

long long cmp_count = 0, swap_count = 0;
int INSERTION_THRESHOLD = 16;

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
void insertion_sort(vector<int> &arr, int l, int r, int n) {
    for (int i = l + 1; i <= r; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= l && cmp(key, arr[j])) {
            swap_keys(arr[j + 1], arr[j]);
            --j;
        }
        arr[j + 1] = key;
    }
}
void hybrid_quick_sort(vector<int> &arr, int l, int r, int n) {
    while (l < r) {
        if (r - l + 1 <= INSERTION_THRESHOLD) {
            insertion_sort(arr, l, r, n);
            break;
        } else {
            int pivot = arr[r];
            int i = l - 1;
            for (int j = l; j < r; ++j) {
                if (cmp(arr[j], pivot)) {
                    ++i;
                    swap_keys(arr[i], arr[j]);
                }
            }
            swap_keys(arr[i + 1], arr[r]);
            int p = i + 1;
            hybrid_quick_sort(arr, l, p - 1, n);
            l = p + 1;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc >= 2) {
        INSERTION_THRESHOLD = atoi(argv[1]);
        if (INSERTION_THRESHOLD < 1) INSERTION_THRESHOLD = 1;
    }
    int n;
    cin >> n;
    vector<int> arr(n);
    for (auto &x : arr) cin >> x;
    hybrid_quick_sort(arr, 0, n - 1, n);
    cout << "Comparisons: " << cmp_count << endl;
    cout << "Swaps:       " << swap_count << endl;
    assert(is_sorted(arr));
    return 0;
}