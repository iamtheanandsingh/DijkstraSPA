//Only Necessary Header Files
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <List>
#include <Map>

using namespace std;

//Type of Node We Come Across, Is it an Input, Output, or Intermediate.
struct nodeType {
    const int INPUT;
    const int OUTPUT;
    const int INTERMEDIATE;
};

struct Node {
    string type, name;
    list<Node*> adjacencyList;
    int delay;

    Node(string type, list<Node*> adjacencyList, int delay, string name) {
        this->type = type;
        this->adjacencyList = adjacencyList;
        this->delay = delay;
        this->name = name;
    
    }
    ~Node() {
        for (auto nextNode : adjacencyList){
            delete nextNode;
        }
    }
};

struct pairNode {
    Node* gate;
    int delay;

    pairNode(Node* gate, int delay) {
        this->gate = gate;
        this->delay = delay;
    }
};

list<string> getPath(map<string, string> &penultimate, string destination) {
    list<string> path;
    while (!destination.empty()) {
        path.push_front(destination);
        destination = penultimate[destination];
    }
    return path;
}

Node* findGateNode(vector<ptr<Node>> &nodes, string nodeName) {
    for (auto &node : nodes) {
        if (node->name == nodeName) {
            return node;
        }
    }
    return nullptr;
}

//Declaring BenchFiles Globally
vector<string> benchFiles = { 
    "c17.bench", "c432.bench", "c499.bench", "c880.bench",
    "c1355.bench", "c1908.bench", "c2670.bench", "c3540.bench", 
    "c5315.bench", "c6288.bench", "c7552.bench" 
};

int main() {
    //Declaring the variables
    string input, output, filename;

    //Taking in the input, output and filename
    cin>>filename>>input>>output;
    
    //Benchfile Checking
    ifstream bench;
    if(find(benchFiles.begin(), benchFiles.end(), filename) == benchFiles.end()) {
        
    }

    return 0;
}