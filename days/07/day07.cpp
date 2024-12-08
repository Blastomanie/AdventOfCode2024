#include <utils.h>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>

std::vector<int64_t> parse(std::string line) {
    std::vector<int64_t> output = aoc::splitvi(line.substr(line.find_first_of(":") + 2), ' ');
    output.insert(output.begin(), std::stoll(line.substr(0, line.find_first_of(":"))));
    return output;
}

// PART 1

int64_t isValid(const std::vector<int64_t>& equation) {
    int64_t result = equation[0];
    int operationNumber = equation.size() - 2;
    for (int i = 0; i < pow(2, operationNumber); i++) {
        int64_t counter = equation[1];
        int64_t mult = i;
        for (int j = 2; j < equation.size(); j++) {
            if ((mult & 1) == 0) {
                counter += equation[j];
            } else {
                counter *= equation[j];
            }
            mult = mult >> 1;
            if (counter > result) break;
        }
        if (counter == result) return result;
    }
    return 0;
}

int64_t part_1(std::vector<std::vector<int64_t>> equations) {    
    int64_t result = 0;
    for (std::vector<int64_t> equation : equations) {
        result += isValid(equation);
    }
    return result;
}

// PART 2

int64_t isValid2(const std::vector<int64_t>& equation) {
    int64_t result = equation[0];
    int operationNumber = equation.size() - 2;
    for (int i = 0; i < pow(3, operationNumber); i++) {
        int64_t counter = equation[1];
        int64_t mult = i;
        for (int j = 2; j < equation.size(); j++) {
            if (mult % 3 == 0) {
                counter += equation[j];
            } else if (mult % 3 == 1) {
                counter = std::stoll(std::to_string(counter) + std::to_string(equation[j]));
            } else {
                counter *= equation[j];
            }
            mult = mult / 3;
            if (counter > result) break;
        }
        if (counter == result) return result;
    }
    return 0;
}

int64_t part_2(std::vector<std::vector<int64_t>> equations) {
    int64_t result = 0;
    std::vector<std::vector<int64_t>> badEquations;
    for (std::vector<int64_t> equation : equations) {
        if (isValid(equation) == 0) {
            badEquations.push_back(equation);
        } else {
            result += isValid(equation);
        }
    }
    for (std::vector<int64_t> equation : badEquations) {
        result += isValid2(equation);
    }
    return result;
}

int main() {
    std::vector<std::vector<int64_t>> input = aoc::parseInput("input", parse);
    std::vector<std::vector<int64_t>> test = aoc::parseInput("test", parse);

    // PART 1 

    aoc::checkTest(part_1, 3749, test);
    aoc::showResult(part_1, 1, input);

    // PART 2

    aoc::checkTest(part_2, 11387, test);
    aoc::showResult(part_2, 2, input);
}