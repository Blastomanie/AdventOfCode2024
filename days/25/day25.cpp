#include <utils.h>
#include <iostream>
#include <string>
#include <vector>

void parse(std::vector<std::string> lines, std::pair<std::vector<std::vector<int64_t>>, std::vector<std::vector<int64_t>>>& output) {
    std::vector<int64_t> heights = {-1, -1, -1, -1, -1};
    for (std::string line : lines) {
        for (int i = 0; i < line.length(); i++) {
            if (line[i] == '#') heights[i]++;
        }
    }
    if (lines[0] == "#####") {
        output.second.push_back(heights);
    } else {
        output.first.push_back(heights);
    }
}

// PART 1

bool isOk(std::vector<int64_t>& key, std::vector<int64_t>& lock) {
    for (int i = 0; i < lock.size(); i++) {
        if (key[i] > 5 - lock[i]) {
            return false;
        }
    } 
    return true;
}

int64_t part_1(std::pair<std::vector<std::vector<int64_t>>, std::vector<std::vector<int64_t>>> keysLocks) { 
    int64_t result = 0;
    for (auto lock : keysLocks.second) {
        for (auto key : keysLocks.first) {
            result += isOk(key, lock);
        }
    }
    return result;
}

int main() {
    std::pair<std::vector<std::vector<int64_t>>, std::vector<std::vector<int64_t>>> test = aoc::parseInput("test", parse, 7);
    std::pair<std::vector<std::vector<int64_t>>, std::vector<std::vector<int64_t>>> input = aoc::parseInput("input", parse, 7);

    // PART 1 

    aoc::checkTest(part_1, 3, test);
    aoc::showResult(part_1, 1, input);
}