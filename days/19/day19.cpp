#include <utils.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

std::string parsePatterns(std::string line) {
    return line;
}

void parseTowels(std::string line, std::vector<std::string>& output) {
    line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
    std::stringstream ss(line);
    std::string tmp;
    while(std::getline(ss, tmp, ',')) {
        output.push_back(tmp);
    }
}

// PART 1

bool isPatternPossible(const std::vector<std::string>& towels, std::string pattern, std::set<std::string>& cache) {
    if (pattern.size() <= 0) return true;
    bool isValid = false;
    for (std::string towel : towels) {
        if (pattern.find(towel) == 0) {
            if (cache.insert(pattern.substr(towel.size())).second) {
                isValid = isPatternPossible(towels, pattern.substr(towel.size()), cache);
            }
        }
        if (isValid) return true;
    }
    return false;
}

int64_t part_1(std::vector<std::string> towels, std::vector<std::string> patterns) { 
    int64_t result = 0;
    for (std::string pattern : patterns) {
        std::set<std::string> cache;
        result += isPatternPossible(towels, pattern, cache);
    }
    return result;
}

// PART 2

int64_t numberArrangementsPattern(const std::vector<std::string>& towels, std::string pattern, std::map<std::string, int64_t>& cache) {
    int64_t result = 0;
    if (pattern.size() <= 0) return 1;
    for (std::string towel : towels) {
        if (pattern.find(towel) == 0) {
            if (cache.insert({pattern.substr(towel.size()), 0}).second) {
                int64_t t = numberArrangementsPattern(towels, pattern.substr(towel.size()), cache);
                if (t > 0) cache[pattern.substr(towel.size())] += t;
                result += t;
            } else {
                result += cache.at(pattern.substr(towel.size()));
            }
        }
    }
    return result;
}

int64_t part_2(std::vector<std::string> towels, std::vector<std::string> patterns) {  
    int64_t result = 0;
    for (std::string pattern : patterns) {
        std::map<std::string, int64_t> cache;
        result += numberArrangementsPattern(towels, pattern, cache);
    }
    return result;
}

int main() {
    std::vector<std::string> pattern_test = aoc::parseInput("test", parsePatterns, false);
    std::vector<std::string> towels_test = aoc::parseInput("test", parseTowels);
    std::vector<std::string> pattern_input = aoc::parseInput("input", parsePatterns, false);
    std::vector<std::string> towels_input = aoc::parseInput("input", parseTowels);

    // PART 1 

    aoc::checkTest(part_1, 6, towels_test, pattern_test);
    aoc::showResult(part_1, 1, towels_input, pattern_input);

    // PART 2

    aoc::checkTest(part_2, 16, towels_test, pattern_test);
    aoc::showResult(part_2, 2, towels_input, pattern_input);
}