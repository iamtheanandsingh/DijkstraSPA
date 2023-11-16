#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <queue>
#include <map>
#include <functional>
#include <algorithm>
#include <iterator>

using namespace std;

struct Node {
    string type, name;
    list<Node*> adjacencyList;
    int nodeDelay;

    Node(string type, int nodeDelay, string name) : type(type), nodeDelay(nodeDelay), name(name) {}

    ~Node() {
        for (auto nextNode : adjacencyList) {
            delete nextNode;
        }
    }
};

struct pairNode {
    Node* gate;
    int weight;

    pairNode(Node* gate, int weight) : gate(gate), weight(weight) {}
};

list<string> getPath(map<string, string>& penultimate, string destination) {
    list<string> path;
    while (!destination.empty()) {
        path.push_front(destination);
        destination = penultimate[destination];
    }
    return path;
}

Node* findGateNode(vector<Node*>& nodes, string nodeName) {
    for (auto& node : nodes) {
        if (node->name == nodeName) {
            return node;
        }
    }
    return nullptr;
}

int main(int argc, char* argv[]) {

    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <filename> <input_signal> <output_signal>" << endl;
        return 1;
    }

    // Taking in the input, output, and filename
    string filename = argv[1];
    string inputN = argv[2];
    string outputN = argv[3];

    vector<string> benchFiles = {
        "c17.bench", "c432.bench", "c499.bench", "c880.bench",
        "c1355.bench", "c1908.bench", "c2670.bench", "c3540.bench",
        "c5315.bench", "c6288.bench", "c7552.bench"};

    if (find(benchFiles.begin(), benchFiles.end(), filename) == benchFiles.end()) {
        cerr << "Wrong file name." << endl;
        return 1;
    }

    vector<Node*> nodes;
    string filePath = filename;

    ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        cerr << "Error opening file: " << filePath << endl;
        return 1;
    }

    string line;
    while (getline(inputFile, line)) {
        if (!line.empty() && line[0] != '#') {
            if (line.find("INPUT") != string::npos) {
                size_t starti = line.find('(');
                size_t endi = line.find(')');
                string inputName = line.substr(starti + 1, endi - starti - 1);
                nodes.push_back(new Node("INPUT", 0, inputName));
            } else if (line.find("OUTPUT") != string::npos) {
                size_t starti = line.find('(');
                size_t endi = line.find(')');
                string name = line.substr(starti + 1, endi - starti - 1);
                nodes.push_back(new Node("OUTPUT", 0, name));
            } else {
                string input = line;
                string innerString = " = ";
                size_t index = input.find(innerString);

                string intermediateGateName = input.substr(0, index);
                string inputString = input.substr(index + innerString.length());

                Node* node;
                Node* currIntNode = findGateNode(nodes, intermediateGateName);
                if (currIntNode == nullptr) {
                    node = new Node("INTERMEDIATE", 0, intermediateGateName);
                    nodes.push_back(node);
                } else {
                    node = currIntNode;
                }

                size_t starti = inputString.find('(');
                size_t endi = inputString.find(')');

                if (starti != string::npos && endi != string::npos && starti < endi) {
                    string contentInsideBrackets = inputString.substr(starti + 1, endi - starti - 1);
                    replace(contentInsideBrackets.begin(), contentInsideBrackets.end(), ',', ' ');

                    istringstream iss(contentInsideBrackets);
                    vector<string> intermediateInputGates{istream_iterator<string>{iss},
                                                         istream_iterator<string>{}};

                    for (const auto& gates : intermediateInputGates) {
                        Node* targetNode = findGateNode(nodes, gates);
                        if (targetNode != nullptr) {
                            targetNode->adjacencyList.push_back(node);
                            if (targetNode->type == "INPUT" || targetNode->type == "OUTPUT") {
                                targetNode->nodeDelay = 0;
                            } else {
                                targetNode->nodeDelay = targetNode->adjacencyList.size();
                            }
                        }
                    }
                }
            }
            cout << line << endl;
        }
    }

    inputFile.close();

    Node* sourceGate = findGateNode(nodes, inputN);
    Node* destinationGate = findGateNode(nodes, outputN);

    if (sourceGate == nullptr) {
        cout << "Input Signal not found in " << filename << " file." << endl;
        return 1;
    } else {
        if (sourceGate->type != "INPUT") {
            cout << "Signal " << inputN << " is not an input signal" << endl;
            return 1;
        }
    }

    if (destinationGate == nullptr) {
        cout << "Output Signal not found in " << filename << " file." << endl;
        return 1;
    } else {
        if (destinationGate->type != "OUTPUT") {
            cout << "Signal " << outputN << " is not an output signal" << endl;
            return 1;
        }
    }

    // Dijkstra Algorithm
    map<string, int> nodeDelayDistance;
    map<string, string> predecessors;
    for (const auto& vertex : nodes) {
        nodeDelayDistance[vertex->name] = numeric_limits<int>::max();
        predecessors[vertex->name] = "";
    }

    priority_queue<pairNode, vector<pairNode>, function<bool(pairNode, pairNode)>> minHeap([](pairNode p1, pairNode p2) { 
        return p1.weight > p2.weight; 
        });

    nodeDelayDistance[sourceGate->name] = 0;
    minHeap.push(pairNode(sourceGate, 0));

    while (!minHeap.empty()) {
        pairNode currentNode = minHeap.top();
        minHeap.pop();
        int dis; 
        dis = nodeDelayDistance[currentNode.gate->name];
        if (currentNode.weight > dis) {
            continue;
        }

        for (auto& neighbour : currentNode.gate->adjacencyList) {
            int newDistance = dis + findGateNode(nodes, neighbour->name)->nodeDelay;
            if (newDistance < nodeDelayDistance[neighbour->name]) {
                nodeDelayDistance[neighbour->name] = newDistance;
                predecessors[neighbour->name] = currentNode.gate->name;
                minHeap.push(pairNode(neighbour, newDistance));
            }
        }
    }  

    // Print the shortest distance from INPUT to OUTPUT
    int shortestDistance = nodeDelayDistance[findGateNode(nodes, outputN)->name];
    cout << "Shortest Distance from " << inputN << " to " << outputN << ": " << shortestDistance << endl;

    // Print the path from INPUT to OUTPUT
    list<string> path = getPath(predecessors, findGateNode(nodes, outputN)->name);
    cout << "Path: ";
    for (const auto& vertex : path) {
        cout << vertex << " ";
    }
    cout << endl;

    // Clean up dynamically allocated memory
    for (auto& node : nodes) {
        delete node;
    }
    return 0;
}