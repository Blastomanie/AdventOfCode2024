#include <utils.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

int64_t parseRegisters(std::string line) {
    return std::stoll(line.substr(line.find(":") + 2));
}

void parseInstructions(std::string line, std::vector<int64_t>& output) {
    output = aoc::splitvi(line.substr(line.find(":") + 2), ',');
}

// PART 1

int64_t comboOperand(const int64_t operand, const std::vector<int64_t>& registers) {
    if (operand < 4 || operand == 7) return operand;
    return registers[operand - 4];
}

std::vector<int64_t> runProgram(std::vector<int64_t> registers, const std::vector<int64_t>& instructions) {
    std::vector<int64_t> output;
    for (int i = 0; i < instructions.size() - 1; i += 2) {
        switch(instructions[i]) {
            case 0:
                registers[0] /= pow(2, comboOperand(instructions[i + 1], registers));
                break;
            case 1:
                registers[1] ^= instructions[i + 1];
                break;
            case 2:
                registers[1] = comboOperand(instructions[i + 1], registers) % 8;
                break;
            case 3:
                if (registers[0] != 0) {
                    i = instructions[i + 1] - 2; 
                }
                break;
            case 4:
                registers[1] ^= registers[2];
                break;
            case 5:
                output.push_back(comboOperand(instructions[i + 1], registers) % 8);
                break;
            case 6:
                registers[1] = registers[0] / pow(2, comboOperand(instructions[i + 1], registers));
                break;
            case 7:
                registers[2] = registers[0] / pow(2, comboOperand(instructions[i + 1], registers));
                break;
        }
    }
    return output;
}

std::string part_1(std::vector<int64_t> registers, std::vector<int64_t> instructions) {  
    std::string result;
    for (auto elem : runProgram(registers, instructions)) {
        result += std::to_string(elem) + ',';
    }
    result.pop_back();
    return result;
}

// PART 2

int64_t findA(int64_t a, const std::vector<int64_t>& instructions, int64_t len) {
    if (len == instructions.size()) return a;
    for (int i = 0; i < 8; i++) {
        std::vector<int64_t> out = runProgram({a * 8 + i, 0, 0}, instructions);
        if (out[0] == instructions[instructions.size() - len - 1]) {
            int64_t result = findA(a * 8 + i, instructions, len + 1);
            if (result > 0) return result;
        }
    }
    return -1;
}

int64_t part_2(std::vector<int64_t> registers, std::vector<int64_t> instructions) {    
    return findA(0, instructions, 0);
}

int main() {
    std::vector<int64_t> registers_test = aoc::parseInput("test", parseRegisters);
    std::vector<int64_t> instructions_test = aoc::parseInput("test", parseInstructions, false);
    std::vector<int64_t> registers_input = aoc::parseInput("input", parseRegisters);
    std::vector<int64_t> instructions_input = aoc::parseInput("input", parseInstructions, false);

    // PART 1 

    aoc::checkTest(part_1, "4,6,3,5,6,3,5,2,1,0", registers_test, instructions_test);
    aoc::showResult(part_1, 1, registers_input, instructions_input);

    // PART 2

    std::vector<int64_t> registers_test2 = aoc::parseInput("test2", parseRegisters);
    std::vector<int64_t> instructions_test2 = aoc::parseInput("test2", parseInstructions, false);

    aoc::checkTest(part_2, 117440, registers_test2, instructions_test2);
    aoc::showResult(part_2, 2, registers_input, instructions_input);
}