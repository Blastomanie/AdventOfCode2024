#include <utils.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

void parseOrderingRules(std::string line, std::map<int64_t, std::vector<int64_t>>& rules) {
    std::vector<int64_t> numbers = aoc::splitvi(line, '|');
    if (rules.find(numbers[0]) == rules.end()) {
        rules[numbers[0]] = {numbers[1]};
    } else {
        rules[numbers[0]].push_back(numbers[1]);
    }
}

std::vector<int64_t> parseUpdates(std::string line) {
    return aoc::splitvi(line, ',');
}

// PART 1

bool checkUpdate(const std::vector<int64_t>& update, const std::map<int64_t, std::vector<int64_t>>& orderingRules) {
    for (int i = update.size() - 1; i > 0; i--) {
        if (orderingRules.find(update[i]) != orderingRules.end()) {
            std::vector<int64_t> rules = orderingRules.at(update[i]);
            for (int j = 0; j < i; j++) {
                if (std::find(rules.begin(), rules.end(), update[j]) != rules.end()) {
                    return false;
                }
            }
        }
    }
    return true;
}

int64_t part_1(std::map<int64_t, std::vector<int64_t>> orderingRules, std::vector<std::vector<int64_t>> updates) {    
    int64_t result = 0;
    for (std::vector<int64_t> update : updates) {
        if (checkUpdate(update, orderingRules)) result += update[update.size() / 2];
    }
    return result;
}

// PART 2

std::vector<int64_t> orderedUpdate(std::vector<int64_t> update, const std::map<int64_t, std::vector<int64_t>>& orderingRules) {
    for (int i = update.size() - 1; i > 0; i--) {
        if (orderingRules.find(update[i]) != orderingRules.end()) {
            std::vector<int64_t> rules = orderingRules.at(update[i]);
            for (int j = 0; j < i; j++) {
                if (std::find(rules.begin(), rules.end(), update[j]) != rules.end()) {
                    std::iter_swap(update.begin() + i, update.begin() + j);
                    i++;
                    break;
                }
            }
        }
    }
    return update;
}

int64_t part_2(std::map<int64_t, std::vector<int64_t>> orderingRules, std::vector<std::vector<int64_t>> updates) {
    int64_t result = 0;
    for (std::vector<int64_t> update : updates) {
        if (!checkUpdate(update, orderingRules)) {
            std::vector<int64_t> ordered = orderedUpdate(update, orderingRules);
            result += ordered[ordered.size() / 2];
        }
    }
    return result;
}

// FIRST VERSION
int64_t part_21(std::map<int64_t, std::vector<int64_t>> orderingRules, std::vector<std::vector<int64_t>> updates) {
    int64_t result = 0;
    for (std::vector<int64_t> update : updates) {
        if (!checkUpdate(update, orderingRules)) {
            std::vector<int64_t> orderedUpdate;
            while (orderedUpdate.size() != update.size() / 2 + 1) {
                for (int i = 0; i < update.size(); i++) {
                    if (std::find(orderedUpdate.begin(), orderedUpdate.end(), update[i]) == orderedUpdate.end()) {
                        std::vector<int64_t> rules = orderingRules[update[i]];
                        bool correct = true;
                        for (int j = 0; j < update.size(); j++) {
                            if (i != j && std::find(orderedUpdate.begin(), orderedUpdate.end(), update[j]) == orderedUpdate.end()) {
                                if (std::find(rules.begin(), rules.end(), update[j]) != rules.end()) {
                                    correct = false;
                                    break;
                                }  
                            }
                        }
                        if (correct) {
                            orderedUpdate.push_back(update[i]);
                            break;
                        } 
                    }
                }
            }
            result += orderedUpdate.back();
        }
    }
    return result;
}


int main() {
    std::map<int64_t, std::vector<int64_t>> orderingRules_input = aoc::parseInput("input", parseOrderingRules);
    std::vector<std::vector<int64_t>> updates_input = aoc::parseInput("input", parseUpdates, false);
    std::map<int64_t, std::vector<int64_t>> orderingRules_test = aoc::parseInput("test", parseOrderingRules);
    std::vector<std::vector<int64_t>> updates_test = aoc::parseInput("test", parseUpdates, false);

    // PART 1 

    aoc::checkTest(part_1, 143, orderingRules_test, updates_test);
    aoc::showResult(part_1, 1, orderingRules_input, updates_input);

    // PART 2

    aoc::checkTest(part_2, 123, orderingRules_test, updates_test);
    aoc::showResult(part_2, 2, orderingRules_input, updates_input);
}