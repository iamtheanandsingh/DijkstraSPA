//Only Necessary Header Files
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

//Declaring BenchFiles Globally
vector<string> benchFiles = { 
        "c17.bench", "c432.bench", "c499.bench", "c880.bench",
        "c1355.bench", "c1908.bench", "c2670.bench", "c3540.bench", 
        "c5315.bench", "c6288.bench", "c7552.bench" 
        };

int main() {
    //Declaring the variables
    string input, output, filename;

    cin>>filename>>input>>output;
    
    ifstream bench;
    ifstream inputFile();

    if (!bench.is_open()) {
        cerr << "Error opening file: " << filePath << endl;
        return 1;
    }

    //Taking in the input, output and filename
    cin>>filename>>input>>output;

    //Opening the file
    bench.open(filename);
    bench
    //Checking if the filename is an actual Benchfile or not
    if(bench.fail()) {
        cout<<"Wrong file name";
        exit(0);
    }
}