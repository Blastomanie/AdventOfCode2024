#include <utils.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

std::string parse(std::string line) {
    return line;
}

// PART 1

bool isDigit(const std::string& s) {
    std::string::const_iterator it = s.begin();
    while ((it != s.end()) && (std::isdigit(*it))) {
        ++it;
    }
    return !s.empty() && it == s.end();
}

int64_t part_1(std::vector<std::string> memories) {
    int64_t mul = 0;
    for (std::string memory : memories) {
        std::size_t found = memory.find("mul(");
        std::size_t begin = 0;
        while (found != std::string::npos) {
            found += 4;
            std::size_t comma = memory.substr(begin + found).find(',');
            std::string n1 = memory.substr(found + begin, comma);
            std::size_t pare = memory.substr(found + begin + comma).find(')');
            std::string n2 = memory.substr(found + begin + comma + 1, pare - 1);
            if (isDigit(n1) && isDigit(n2)) {
                mul += std::stoll(n1) * std::stoll(n2);
            }
            begin += found;
            found = memory.substr(begin).find("mul(");
        }
    }
    return mul;
}

// PART 2

std::vector<std::pair<std::size_t, std::string>> findInstructions(const std::string& memory, const std::string& instruction) {
    std::vector<std::pair<std::size_t, std::string>> instructionsList;
    std::size_t found = memory.find(instruction);
    std::size_t begin = 0;
    while (found != std::string::npos) {
        instructionsList.push_back({begin + found, instruction});
        begin += (found + 1);
        found = memory.substr(begin).find(instruction);
    }
    return instructionsList;
}

int64_t part_2(std::vector<std::string> memories) {
    int64_t mul = 0;
    bool isDo = true;
    for (std::string memory : memories) {
        std::size_t found = memory.find("mul(");
        std::size_t begin = 0;
        std::vector<std::pair<std::size_t, std::string>> instructionDo = findInstructions(memory, "do()");
        std::vector<std::pair<std::size_t, std::string>> instructionDont = findInstructions(memory, "don't()");
        std::vector<std::pair<std::size_t, std::string>> instructions = instructionDo;
        instructions.insert(instructions.end(), instructionDont.begin(), instructionDont.end());
        std::sort(instructions.begin(), instructions.end());
        
        int i = 0;
        while (found != std::string::npos) {
            found += 4;
            while (begin + found > instructions[i].first && i < instructions.size()) {
                isDo = instructions[i].second == "do()";
                i++;
            }
            if (isDo) {
                std::size_t comma = memory.substr(begin + found).find(',');
                std::string n1 = memory.substr(found + begin, comma);
                std::size_t pare = memory.substr(found + begin + comma).find(')');
                std::string n2 = memory.substr(found + begin + comma + 1, pare - 1);
                if (isDigit(n1) && isDigit(n2)) {
                    mul += std::stoll(n1) * std::stoll(n2);
                }
            }
            begin += found;
            found = memory.substr(begin).find("mul(");
        }
    }
    return mul;
}

int main() {
    std::vector<std::string> input = aoc::parseInput("input", parse);
    std::vector<std::string> test1 = aoc::parseInput("test1", parse);
    std::vector<std::string> test2 = aoc::parseInput("test2", parse);

    // PART 1 

    aoc::checkTest(part_1, 161, test1);
    aoc::showResult(part_1, 1, input);

    // PART 2

    aoc::checkTest(part_2, 48, test2);
    aoc::showResult(part_2, 2, input);
}