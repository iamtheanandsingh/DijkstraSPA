//Necessary Header Files
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <algorithm>
#include <memory>
#include <iterator>

class djikstraMinDelay {
public:
    enum NodeType {
        INPUT,
        OUTPUT,
        INTERMEDIATE
    };

    class GateNode {
    public:
        string type;
        list<GateNode*> adjacencyList;
        int gateDelay;
        string name;

        GateNode(string type, list<GateNode*> adjacencyList, int gateDelay, string name) :
            type(type), adjacencyList(adjacencyList), gateDelay(gateDelay), name(name) {}

        ~GateNode() {
            for (auto neighbor : adjacencyList) {
                delete neighbor;
            }
        }
    };

    class Pair {
    public:
        GateNode* gate;
        int delayWeight;

        Pair(GateNode* gate, int delayWeight) : gate(gate), delayWeight(delayWeight) {}
    };

    static list<string> getPath(map<string, string>& predecessors, string destination) {
        list<string> path;
        while (!destination.empty()) {
            path.push_back(destination);
            destination = predecessors[destination];
        }
        path.reverse();
        return path;
    }

    static GateNode* findGateNodeByName(vector<unique_ptr<GateNode>>& nodes, string nodeName) {
        for (const auto& node : nodes) {
            if (node->name == nodeName) {
                return node.get();
            }
        }
        return nullptr;
    }
};

vector<string> benchFiles = { "c17.bench", "c432.bench", "c499.bench", "c880.bench", "c1355.bench",
                                        "c1908.bench", "c2670.bench", "c3540.bench", "c5315.bench", "c6288.bench", "c7552.bench" };

int main(int argc, char* argv[]) {

    if (arg) {
        cout << "Incorrect number of arguments" << endl;
        return 1;
    }

    string benchFileName = argv[1];
    if (find(benchFiles.begin(), benchFiles.end(), benchFileName) == benchFiles.end()) {
        cout << "Wrong File Name. File does not Exist" << endl;
        return 1;
    }

    djikstraMinDelay outerInstance;
    vector<unique_ptr<djikstraMinDelay::GateNode>> nodes;
    string filePath = benchFileName;

    ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        cerr << "Error opening file: " << filePath << endl;
        return 1;
    }

    string line;
    while (getline(inputFile, line)) {
        if (!line.empty()) {
            if (line[0] == '#') {
                continue;
            }
            if (line.find("INPUT") != string::npos) {
                list<djikstraMinDelay::GateNode*> adjacencyList;
                size_t startIndex = line.find('(');
                size_t endIndex = line.find(')');
                string inputName = line.substr(startIndex + 1, endIndex - startIndex - 1);
                auto node1 = make_unique<djikstraMinDelay::GateNode>("INPUT", adjacencyList, 0, inputName);
                nodes.push_back(move(node1));
            } else if (line.find("OUTPUT") != string::npos) {
                list<djikstraMinDelay::GateNode*> adjacencyList;
                size_t startIndex = line.find('(');
                size_t endIndex = line.find(')');
                string name = line.substr(startIndex + 1, endIndex - startIndex - 1);
                auto node = make_unique<djikstraMinDelay::GateNode>("OUTPUT", adjacencyList, 0, name);
                nodes.push_back(move(node));
            } else {
                list<djikstraMinDelay::GateNode*> adjacencyList;
                string input = line;
                string innerString = " = ";
                size_t index = input.find(innerString);

                string intermediateGateName = input.substr(0, index);
                string inputString = input.substr(index + innerString.length());

                djikstraMinDelay::GateNode* node;
                djikstraMinDelay::GateNode* currIntNode = djikstraMinDelay::findGateNodeByName(nodes, intermediateGateName);
                if (currIntNode == nullptr) {
                    node = new djikstraMinDelay::GateNode("INTERMEDIATE", adjacencyList, 0, intermediateGateName);
                    nodes.push_back(unique_ptr<djikstraMinDelay::GateNode>(node));
                } else {
                    node = currIntNode;
                }

                size_t startIndex = inputString.find('(');
                size_t endIndex = inputString.find(')');

                if (startIndex != string::npos && endIndex != string::npos && startIndex < endIndex) {
                    string contentInsideBrackets = inputString.substr(startIndex + 1, endIndex - startIndex - 1);
                    replace(contentInsideBrackets.begin(), contentInsideBrackets.end(), ',', ' ');

                    istringstream iss(contentInsideBrackets);
                    vector<string> intermediateInputGates{ istream_iterator<string>{iss},
                                                                    istream_iterator<string>{} };

                    for (const auto& gates : intermediateInputGates) {
                        djikstraMinDelay::GateNode* targetNode = djikstraMinDelay::findGateNodeByName(nodes, gates);
                        if (targetNode != nullptr) {
                            djikstraMinDelay::GateNode* newNode = djikstraMinDelay::findGateNodeByName(nodes, node->name);
                            targetNode->adjacencyList.push_back(newNode);
                            if (targetNode->type == "INPUT" || targetNode->type == "OUTPUT") {
                                targetNode->gateDelay = 0;
                            } else {
                                targetNode->gateDelay = targetNode->adjacencyList.size();
                            }
                        }
                    }
                }
            }
            cout << line << endl;
        }
    }

    inputFile.close();

    djikstraMinDelay::GateNode* sourceGate = djikstraMinDelay::findGateNodeByName(nodes, argv[2]);
    djikstraMinDelay::GateNode* destinationGate = djikstraMinDelay::findGateNodeByName(nodes, argv[3]);

    if (sourceGate == nullptr) {
        cout << "Input Signal not found in " << benchFileName << " file." << endl;
        return 1;
    } else {
        if (sourceGate->type != "INPUT") {
            cout << "Signal " << argv[2] << " is not an input signal" << endl;
            return 1;
        }
    }

    if (destinationGate == nullptr) {
        cout << "Output Signal not found in " << benchFileName << " file." << endl;
        return 1;
    } else {
        if (destinationGate->type != "OUTPUT") {
            cout << "Signal " << argv[3] << " is not an output signal" << endl;
            return 1;
        }
    }

    // Djikstra Algorithm
    map<string, int> delayDistance;
    map<string, string> predecessors;
    for (const auto& vertex : nodes) {
        delayDistance[vertex->name] = numeric_limits<int>::max();
        predecessors[vertex->name] = "";
    }

    priority_queue<djikstraMinDelay::Pair, vector<djikstraMinDelay::Pair>, function<bool(djikstraMinDelay::Pair, djikstraMinDelay::Pair)>> minHeap(
        [](djikstraMinDelay::Pair p1, djikstraMinDelay::Pair p2) {
            return p1.delayWeight > p2.delayWeight;
        }
    );

    djikstraMinDelay::GateNode* sourceNode = djikstraMinDelay::findGateNodeByName(nodes, "G2gat");

    delayDistance[sourceNode->name] = 0;
    minHeap.push(djikstraMinDelay::Pair(sourceNode, 0));

    while (!minHeap.empty()) {
        djikstraMinDelay::Pair currentNode = minHeap.top();
        minHeap.pop();
        int dis = delayDistance[currentNode.gate->name];
        if (currentNode.delayWeight > dis) {
            continue;
        }

        for (auto& neighbour : currentNode.gate->adjacencyList) {
            int newDistance = dis + djikstraMinDelay::findGateNodeByName(nodes, neighbour->name)->gateDelay;
            delayDistance[neighbour->name] = newDistance;
            predecessors[neighbour->name] = currentNode.gate->name;
            minHeap.push(djikstraMinDelay::Pair(neighbour, newDistance));
        }
    }

    // Delay from Source to every other Node
    for (const auto& entry : delayDistance) {
        string vertex = entry.first;
        int delay = entry.second;
        cout << "Gate: " << vertex << ", Delay: " << delay << endl;
    }

    list<string> path = djikstraMinDelay::getPath(predecessors, "G23gat");
    for (const auto& vertex : path) {
        cout << vertex << " ";
    }
    cout << endl;

    return 0;
}
