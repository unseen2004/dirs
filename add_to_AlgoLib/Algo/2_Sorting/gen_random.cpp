#include <iostream>
#include <vector>
#include <random>
using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: gen_random n\n";
        return 1;
    }
    int n = atoi(argv[1]);
    vector<int> arr(n);
    for (int i = 0; i < n; ++i) arr[i] = i;
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<> dist(0, 2 * n - 1);
    for (int i = 0; i < n; ++i)
        arr[i] = dist(rng);
    cout << n << endl;
    for (int i = 0; i < n; ++i)
        cout << arr[i] << (i + 1 == n ? '\n' : ' ');
    return 0;
}