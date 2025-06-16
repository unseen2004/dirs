#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: gen_sorted n\n";
        return 1;
    }
    int n = atoi(argv[1]);
    cout << n << endl;
    for (int i = 0; i < n; ++i)
        cout << i << (i + 1 == n ? '\n' : ' ');
    return 0;
}