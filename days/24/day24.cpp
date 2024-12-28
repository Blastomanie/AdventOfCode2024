#include <utils.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <sstream>
#include <iomanip>
#include <cmath>

struct operation {
    std::string a;
    std::string op;
    std::string b;
};

void parseState(std::string line, std::map<std::string, bool>& output) {
    output[line.substr(0, line.find(":"))] = std::stoi(line.substr(line.find(":") + 2));
}

void parseOperation(std::string line, std::map<std::string, operation>& output) {
    std::string a = line.substr(0, line.find(" "));
    line = line.substr(line.find(" ") + 1);
    std::string operation = line.substr(0, line.find(" "));
    line = line.substr(line.find(" ") + 1);
    std::string b = line.substr(0, line.find(" "));
    output[line.substr(line.find(">") + 2)] = {a, operation, b};
}

// PART 1

bool operate(std::map<std::string, bool>& state, std::map<std::string, operation>& operations, std::string output) {
    operation logic = operations[output];
    if (state.find(logic.a) == state.end()) {
        state[logic.a] = operate(state, operations, logic.a);
    }
    if (state.find(logic.b) == state.end()) {
        state[logic.b] = operate(state, operations, logic.b);
    }
    if (logic.op == "AND") {
        state[output] = state[logic.a] && state[logic.b];
        return state[output];
    } else if (logic.op == "OR") {
        state[output] = state[logic.a] || state[logic.b];
        return state[output];
    } else {
        state[output] = state[logic.a] ^ state[logic.b];
        return state[output];
    }
}

int64_t part_1(std::map<std::string, bool> state, std::map<std::string, operation> operations) { 
    int64_t result = 0;
    int i = 0;
    std::ostringstream ni;
    ni << std::setw(2) << std::setfill('0') << i;

    while (operations.find("z" + ni.str()) != operations.end()) {
        result += operate(state, operations, "z" + ni.str()) * std::pow(2, i);
        i++;
        ni.str("");
        ni << std::setw(2) << std::setfill('0') << i;
    }

    return result;
}

// PART 2

std::string part_2(std::map<std::string, bool> state, std::map<std::string, operation> operations) {  
    int64_t x = 0;
    int64_t y = 0;
    for (std::map<std::string, bool>::iterator it = state.begin(); it != state.end(); it++) {
        if (it->first[0] == 'x') {
            x += it->second * std::pow(2, std::stoi(it->first.substr(1)));
        } else if (it->first[0] == 'y') {
            y += it->second * std::pow(2, std::stoi(it->first.substr(1)));
        }
    }
    int64_t sum = x + y;
    std::set<std::string> swaps;
    std::string xyz = "xyz";
    for (std::map<std::string, operation>::iterator it = operations.begin(); it != operations.end(); it++) {
        std::string output = it->first;
        operation operation = it->second;
        if (output[0] == 'z' && operation.op != "XOR" && output != "z45") {
            swaps.insert(output);
        }
        if (operation.op == "XOR" && (xyz.find(operation.a[0]) == std::string::npos) && (xyz.find(operation.b[0]) == std::string::npos) && (xyz.find(output[0]) == std::string::npos)) {
            swaps.insert(output);
        }
        if (operation.op == "AND" && "x00" != operation.a && "x00" != operation.b) {
            for (auto a = operations.begin(); a != operations.end(); a++) {
                if ((output == a->second.a || output == a->second.b) && a->second.op != "OR") {
                    swaps.insert(output);
                    break;
                } 
            }
        }
        if (operation.op == "XOR") {
            for (auto a = operations.begin(); a != operations.end(); a++) {
                if ((output == a->second.a || output == a->second.b) && a->second.op == "OR") {
                    swaps.insert(output);
                    break;
                } 
            }
        }
    }
    return aoc::join(swaps, ',');
}

int main() {
    std::map<std::string, bool> state_test = aoc::parseInput("test", parseState);
    std::map<std::string, operation> operations_test = aoc::parseInput("test", parseOperation, false);
    std::map<std::string, bool> state_input = aoc::parseInput("input", parseState);
    std::map<std::string, operation> operations_input = aoc::parseInput("input", parseOperation, false);

    // PART 1 

    aoc::checkTest(part_1, 2024, state_test, operations_test);
    aoc::showResult(part_1, 1, state_input, operations_input);

    // PART 2

    aoc::showResult(part_2, 2, state_input, operations_input);
}