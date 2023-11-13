#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>

using namespace std;

int main() {
    ifstream benchFile("c432.bench", ios::binary);

    if (!benchFile.is_open()) {
        cerr << "Error: could not open input file\n";
        return 1;
    }
    else {
        cout << "Good";
    }

    benchFile.close();
    return 0;
}
