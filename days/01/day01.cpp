#include <utils.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

int64_t parseLeft(std::string line) {
    return std::stoll(line.substr(0, line.find(' ')));
}

int64_t parseRight(std::string line) {
    return std::stoll(line.substr(line.find(' '), line.size()));;
}

// PART 1

int64_t part_1(std::vector<int64_t> leftList, std::vector<int64_t> rightList) {
    int64_t total = 0;
    std::sort(leftList.begin(), leftList.end());
    std::sort(rightList.begin(), rightList.end());
    for (int i = 0; i < leftList.size(); i++) {
        total += std::abs(leftList[i] - rightList[i]);
    }
    return total;
}

// PART 2

int64_t part_2(std::vector<int64_t> leftList, std::vector<int64_t> rightList) {
    int64_t total = 0;
    for (int i = 0; i < leftList.size(); i++) {
        total += leftList[i] * std::count (rightList.begin(), rightList.end(), leftList[i]);
    }
    return total;
}

int main() {
    std::vector<int64_t> leftList = aoc::parseInput("input", parseLeft);
    std::vector<int64_t> rightList = aoc::parseInput("input", parseRight);

    std::vector<int64_t> leftList_test = aoc::parseInput("test", parseLeft);
    std::vector<int64_t> rightList_test = aoc::parseInput("test", parseRight);
    
    // PART 1 

    aoc::checkTest(part_1, 11, leftList_test, rightList_test);
    aoc::showResult(part_1, 1, leftList, rightList);

    // PART 2

    aoc::checkTest(part_2, 31, leftList_test, rightList_test);
    aoc::showResult(part_2, 2, leftList, rightList);
}