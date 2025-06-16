#include <iostream>
#include <vector>
#include <iomanip>
#include <cassert>
using namespace std;

long long cmp_count = 0, swap_count = 0;
const int INSERTION_THRESHOLD = 16;

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
            if (n < 40) {
                cout << "IS [" << setw(2) << l << "," << setw(2) << r << "]: ";
                print_array(arr);
            }
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
            if (n < 40) {
                cout << "QS [" << setw(2) << l << "," << setw(2) << r << "]: ";
                print_array(arr);
            }
            if (p - l < r - p) {
                hybrid_quick_sort(arr, l, p - 1, n);
                l = p + 1;
            } else {
                hybrid_quick_sort(arr, p + 1, r, n);
                r = p - 1;
            }
        }
    }
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
    hybrid_quick_sort(arr, 0, n - 1, n);

    if (n < 40) {
        cout << "Input again:  "; print_array(input);
        cout << "Sorted:       "; print_array(arr);
    }
    cout << "Comparisons: " << cmp_count << endl;
    cout << "Swaps:       " << swap_count << endl;

    assert(is_sorted(arr));
    return 0;
}