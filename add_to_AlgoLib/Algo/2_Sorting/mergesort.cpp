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
void assign_key(int& a, int b) {
    ++swap_count;
    a = b;
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

void merge(vector<int>& arr, int left, int mid, int right) {
    vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;
    while (i <= mid && j <= right) {
        if (!cmp(arr[j], arr[i])) { assign_key(temp[k++], arr[i++]); }
        else { assign_key(temp[k++], arr[j++]); }
    }
    while (i <= mid) assign_key(temp[k++], arr[i++]);
    while (j <= right) assign_key(temp[k++], arr[j++]);
    for (int t = 0; t < (int)temp.size(); ++t) assign_key(arr[left + t], temp[t]);
}

void merge_sort(vector<int>& arr, int l, int r, bool print_steps=false) {
    if (l >= r) return;
    int mid = (l + r) / 2;
    merge_sort(arr, l, mid, print_steps);
    merge_sort(arr, mid + 1, r, print_steps);
    merge(arr, l, mid, r);
    if (print_steps && r - l + 1 > 1) {
        cout << "After merge [" << l << "," << r << "]: ";
        print_array(arr);
    }
}

int main() {
    int n;
    cin >> n;
    vector<int> arr(n);
    for (auto &x : arr) cin >> x;

    bool print_steps = n < 40;
    if (print_steps) { cout << "Input:        "; print_array(arr); }
    vector<int> input = arr;

    merge_sort(arr, 0, n-1, print_steps);

    if (print_steps) {
        cout << "Input again:  "; print_array(input);
        cout << "Sorted:       "; print_array(arr);
    }
    cout << "Comparisons: " << cmp_count << endl;
    cout << "Swaps:       " << swap_count << endl;
    assert(is_sorted(arr));
    return 0;
}