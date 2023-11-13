#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

struct LogicGate {
    std::string type;
    std::string output;
    std::vector<std::string> inputs;
};

int main() {
    std::string filename = "c17.bench"; // Replace with the actual .bench file name
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Unable to open the file: " << filename << std::endl;
        return 1;
    }

    std::vector<LogicGate> logicGates;
    std::string line;

    while (std::getline(file, line)) {
        // Skip comment lines (lines starting with '#')
        if (line.empty() || line[0] == '#') {
            continue;
        }

        LogicGate gate;
        std::stringstream lineStream(line);
        std::string token;

        lineStream >> gate.output; // The first token is the output signal
        lineStream >> gate.type;   // The second token is the gate type

        while (lineStream >> token) {
            // Additional tokens are input signals
            gate.inputs.push_back(token);
        }

        logicGates.push_back(gate);
    }

    file.close();

    // Process the logic gates as needed
    for (const LogicGate& gate : logicGates) {
        std::cout << "Gate Type: " << gate.type << std::endl;
        std::cout << "Output Signal: " << gate.output << std::endl;
        std::cout << "Input Signals:";
        for (const std::string& input : gate.inputs) {
            std::cout << " " << input;
        }
        std::cout << std::endl;
    }

    return 0;
}
