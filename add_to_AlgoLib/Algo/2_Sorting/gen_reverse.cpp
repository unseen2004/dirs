#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: gen_reverse n\n";
        return 1;
    }
    int n = atoi(argv[1]);
    cout << n << endl;
    for (int i = n - 1; i >= 0; --i)
        cout << i << (i == 0 ? '\n' : ' ');
    return 0;
}