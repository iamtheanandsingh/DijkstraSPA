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
        std::string type;
        std::list<GateNode*> adjacencyList;
        int gateDelay;
        std::string name;

        GateNode(std::string type, std::list<GateNode*> adjacencyList, int gateDelay, std::string name) :
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

    static std::list<std::string> getPath(std::map<std::string, std::string>& predecessors, std::string destination) {
        std::list<std::string> path;
        while (!destination.empty()) {
            path.push_back(destination);
            destination = predecessors[destination];
        }
        path.reverse();
        return path;
    }

    static GateNode* findGateNodeByName(std::vector<std::unique_ptr<GateNode>>& nodes, std::string nodeName) {
        for (const auto& node : nodes) {
            if (node->name == nodeName) {
                return node.get();
            }
        }
        return nullptr;
    }
};

std::vector<std::string> benchFiles = { "c17.bench", "c432.bench", "c499.bench", "c880.bench", "c1355.bench",
                                        "c1908.bench", "c2670.bench", "c3540.bench", "c5315.bench", "c6288.bench", "c7552.bench" };

int main(int argc, char* argv[]) {

    if (arg) {
        std::cout << "Incorrect number of arguments" << std::endl;
        return 1;
    }

    std::string benchFileName = argv[1];
    if (std::find(benchFiles.begin(), benchFiles.end(), benchFileName) == benchFiles.end()) {
        std::cout << "Wrong File Name. File does not Exist" << std::endl;
        return 1;
    }

    djikstraMinDelay outerInstance;
    std::vector<std::unique_ptr<djikstraMinDelay::GateNode>> nodes;
    std::string filePath = benchFileName;

    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        if (!line.empty()) {
            if (line[0] == '#') {
                continue;
            }
            if (line.find("INPUT") != std::string::npos) {
                std::list<djikstraMinDelay::GateNode*> adjacencyList;
                size_t startIndex = line.find('(');
                size_t endIndex = line.find(')');
                std::string inputName = line.substr(startIndex + 1, endIndex - startIndex - 1);
                auto node1 = std::make_unique<djikstraMinDelay::GateNode>("INPUT", adjacencyList, 0, inputName);
                nodes.push_back(std::move(node1));
            } else if (line.find("OUTPUT") != std::string::npos) {
                std::list<djikstraMinDelay::GateNode*> adjacencyList;
                size_t startIndex = line.find('(');
                size_t endIndex = line.find(')');
                std::string name = line.substr(startIndex + 1, endIndex - startIndex - 1);
                auto node = std::make_unique<djikstraMinDelay::GateNode>("OUTPUT", adjacencyList, 0, name);
                nodes.push_back(std::move(node));
            } else {
                std::list<djikstraMinDelay::GateNode*> adjacencyList;
                std::string input = line;
                std::string innerString = " = ";
                size_t index = input.find(innerString);

                std::string intermediateGateName = input.substr(0, index);
                std::string inputString = input.substr(index + innerString.length());

                djikstraMinDelay::GateNode* node;
                djikstraMinDelay::GateNode* currIntNode = djikstraMinDelay::findGateNodeByName(nodes, intermediateGateName);
                if (currIntNode == nullptr) {
                    node = new djikstraMinDelay::GateNode("INTERMEDIATE", adjacencyList, 0, intermediateGateName);
                    nodes.push_back(std::unique_ptr<djikstraMinDelay::GateNode>(node));
                } else {
                    node = currIntNode;
                }

                size_t startIndex = inputString.find('(');
                size_t endIndex = inputString.find(')');

                if (startIndex != std::string::npos && endIndex != std::string::npos && startIndex < endIndex) {
                    std::string contentInsideBrackets = inputString.substr(startIndex + 1, endIndex - startIndex - 1);
                    std::replace(contentInsideBrackets.begin(), contentInsideBrackets.end(), ',', ' ');

                    std::istringstream iss(contentInsideBrackets);
                    std::vector<std::string> intermediateInputGates{ std::istream_iterator<std::string>{iss},
                                                                    std::istream_iterator<std::string>{} };

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
            std::cout << line << std::endl;
        }
    }

    inputFile.close();

    djikstraMinDelay::GateNode* sourceGate = djikstraMinDelay::findGateNodeByName(nodes, argv[2]);
    djikstraMinDelay::GateNode* destinationGate = djikstraMinDelay::findGateNodeByName(nodes, argv[3]);

    if (sourceGate == nullptr) {
        std::cout << "Input Signal not found in " << benchFileName << " file." << std::endl;
        return 1;
    } else {
        if (sourceGate->type != "INPUT") {
            std::cout << "Signal " << argv[2] << " is not an input signal" << std::endl;
            return 1;
        }
    }

    if (destinationGate == nullptr) {
        std::cout << "Output Signal not found in " << benchFileName << " file." << std::endl;
        return 1;
    } else {
        if (destinationGate->type != "OUTPUT") {
            std::cout << "Signal " << argv[3] << " is not an output signal" << std::endl;
            return 1;
        }
    }

    // Djikstra Algorithm
    std::map<std::string, int> delayDistance;
    std::map<std::string, std::string> predecessors;
    for (const auto& vertex : nodes) {
        delayDistance[vertex->name] = std::numeric_limits<int>::max();
        predecessors[vertex->name] = "";
    }

    std::priority_queue<djikstraMinDelay::Pair, std::vector<djikstraMinDelay::Pair>, std::function<bool(djikstraMinDelay::Pair, djikstraMinDelay::Pair)>> minHeap(
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
        std::string vertex = entry.first;
        int delay = entry.second;
        std::cout << "Gate: " << vertex << ", Delay: " << delay << std::endl;
    }

    std::list<std::string> path = djikstraMinDelay::getPath(predecessors, "G23gat");
    for (const auto& vertex : path) {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;

    return 0;
}
