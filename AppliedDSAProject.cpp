//Only Necessary Header Files
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main() {

    //Declaring the variables
    string input, output, filename;
    ifstream bench;

    //Taking in the input, output and filename
    cin>>filename>>input>>output;

    //Opening the file
    bench.open(filename);
    
    //Checking if the filename is an actual Benchfile or not
    if(bench.fail()) {
        cout<<"Wrong file name";
        exit(0);
    }
}