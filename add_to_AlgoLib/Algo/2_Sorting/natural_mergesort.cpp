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

vector<pair<int, int>> find_runs(const vector<int>& arr) {
    vector<pair<int, int>> runs;
    int n = arr.size();
    int start = 0;
    while (start < n) {
        int end = start;
        while (end + 1 < n && arr[end] <= arr[end + 1]) ++end;
        runs.push_back({start, end});
        start = end + 1;
    }
    return runs;
}

void natural_merge_sort(vector<int>& arr, int n, bool print_steps=false) {
    while (true) {
        auto runs = find_runs(arr);
        if (runs.size() == 1) break;
        if (print_steps) {
            cout << "Runs: ";
            for (auto r : runs) cout << "[" << r.first << "," << r.second << "] ";
            cout << endl;
        }
        // Merge adjacent pairs of runs
        for (size_t i = 0; i + 1 < runs.size(); i += 2) {
            merge(arr, runs[i].first, runs[i].second, runs[i+1].second);
            if (print_steps) {
                cout << "After merging runs " << runs[i].first << "-" << runs[i].second << " and "
                     << runs[i+1].first << "-" << runs[i+1].second << ": ";
                print_array(arr);
            }
        }
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

    natural_merge_sort(arr, n, print_steps);

    if (print_steps) {
        cout << "Input again:  "; print_array(input);
        cout << "Sorted:       "; print_array(arr);
    }
    cout << "Comparisons: " << cmp_count << endl;
    cout << "Swaps:       " << swap_count << endl;
    assert(is_sorted(arr));
    return 0;
}
