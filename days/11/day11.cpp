#include <utils.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

void parse(std::string line, std::map<int64_t, int64_t>& output) {
    for (int64_t elem : aoc::splitvi(line, ' ')) {
        output[elem]++;
    }
}

// PART 1

int64_t naiveStonesCalculation(int64_t stone, int n = 0) {
    if (n == 25) return 1;
    if (stone == 0) {
        return naiveStonesCalculation(1, n + 1);
    } else if (std::to_string(stone).length() % 2 == 0) {
        std::string t = std::to_string(stone);
        return naiveStonesCalculation(std::stoll(t.substr(0, t.length() / 2)), n + 1) + naiveStonesCalculation(std::stoll(t.substr(t.length() / 2)), n + 1);
    } else {
        return naiveStonesCalculation(stone * 2024, n + 1);
    }
}

int64_t stonesCalculation(std::map<int64_t, int64_t> stonesMap, const int blinks, int n = 1) {
    std::map<int64_t, int64_t> newStonesMap;
    int64_t result = 0;
    for (auto it = stonesMap.begin(); it != stonesMap.end(); it++) {
        if (it->first == 0) {
            newStonesMap[1] += it->second;
            if (n == blinks) result += it->second;
        } else if (std::to_string(it->first).length() % 2 == 0) {
            std::string t = std::to_string(it->first);
            newStonesMap[std::stoll(t.substr(0, t.length() / 2))] += it->second;
            newStonesMap[std::stoll(t.substr(t.length() / 2))] += it->second;
            if (n == blinks) result += 2 * it->second;
        } else {
            newStonesMap[it->first * 2024] += it->second;
            if (n == blinks) result += it->second;
        }
    }
    if (n == blinks) return result;
    return stonesCalculation(newStonesMap, blinks, n + 1);
}

int64_t part_1(std::map<int64_t, int64_t> stones) {    
    return stonesCalculation(stones, 25);
}

// PART 2

int64_t part_2(std::map<int64_t, int64_t> stones) {
    return stonesCalculation(stones, 75);
}

int main() {
    std::map<int64_t, int64_t> input = aoc::parseInput("input", parse);
    std::map<int64_t, int64_t> test = aoc::parseInput("test", parse);

    // PART 1 

    aoc::checkTest(part_1, 55312, test);
    aoc::showResult(part_1, 1, input);

    // PART 2

    aoc::showResult(part_2, 2, input);
}