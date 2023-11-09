//Only Necessary Header Files
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Item
{
    string first;
    string second;
};

int checkBench(ifstream inputBench) {
    if(inputBench.fail()) {
        cout<<"Wrong file name";
        exit(0);
    }
    return 0;
}

int main()
{
    string filename, input, output;
    ifstream file(filename);

    vector<Item> items;

    std::string line;
    while (std::getline(file, line))
    {
        Item item;
        size_t p_space = line.find_first_of(' ');
        if(p_space == line.npos){
            //error: no space between two column
        }
        item.first = line.substr(0, p_space);
        item.second = line.substr(p_space + 1, 
        line.size() - p_space - 1);
        items.push_back(item);
    }

    for (const auto &item : items)
        std::cout << item.first << ": " << item.second << "\n";
}

    //Reading Bench File. 
    ifstream inputBench;
    inputBench.open(filename);
    checkBench(inputBench);

    inputBench.close();
    return 0;
}