#include <iostream>
#include <fstream>
#include <unordered_map>

using namespace std;

struct AdjacencyListNode {
  string node;
  AdjacencyListNode* next;
};

struct AdjacencyList {
  unordered_map<string, AdjacencyListNode*> nodes;
};

void addEdge(AdjacencyList* adjList, string src, string dst) {
  AdjacencyListNode* newNode = new AdjacencyListNode();
  newNode->node = dst;
  newNode->next = adjList->nodes[src];
  adjList->nodes[src] = newNode;
}

AdjacencyList* readBenchFile(string filename) {
  ifstream benchFile(filename);

  if (!benchFile.is_open()) {
    cerr << "Error opening bench file" << endl;
    return NULL;
  }

  AdjacencyList* adjList = new AdjacencyList();

  string line;
  while (getline(benchFile, line)) {
    // Ignore lines starting with '#'
    if (line[0] != '#') {
      // Split the line into two parts: source and destination
      string src = line.substr(0, line.find(' '));
      string dst = line.substr(line.find(' ') + 1);

      // Add the edge to the adjacency list
      addEdge(adjList, src, dst);
    }
  }

  benchFile.close();

  return adjList;
}

void printAdjacencyList(AdjacencyList* adjList) {
  for (auto it = adjList->nodes.begin(); it != adjList->nodes.end(); it++) {
    cout << it->first << ": ";

    AdjacencyListNode* node = it->second;
    while (node != NULL) {
      cout << node->node << " ";
      node = node->next;
    }

    cout << endl;
  }
}

int main() {
  // Read the bench file and create an adjacency list
  AdjacencyList* adjList = readBenchFile("c17.bench");

  // Print the adjacency list
  printAdjacencyList(adjList);

  return 0;
}
