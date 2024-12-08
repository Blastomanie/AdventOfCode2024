#include <utils.h>
#include <iostream>
#include <string>
#include <vector>

std::vector<int64_t> parse(std::string line) {
    return aoc::splitvi(line, ' ');
}

// PART 1

bool checkSingle(const std::vector<int64_t>& report) {
    int64_t isIncreasing = report[0] > report[1] ? 1 : -1;
    for (int i = 0; i < report.size() - 1; i++) {
        int64_t diff = report[i] - report[i + 1];
        if ((std::abs(diff) < 1) || (std::abs(diff) > 3) || (diff * isIncreasing < 0)) {
            return false;
        }
    }
    return true;
}

int64_t part_1(std::vector<std::vector<int64_t>> reports) {
    int64_t safes = 0;
    for (std::vector<int64_t> report : reports) {
        if (checkSingle(report)) safes++;
    }
    return safes;
}

// PART 2

int64_t part_2(std::vector<std::vector<int64_t>> reports) {
    int64_t safes = 0;
    for (std::vector<int64_t> report : reports) {
        int64_t isIncreasing = report[0] > report[1] ? 1 : -1;
        bool unsafe = false;
        for (int i = 0; i < report.size() - 1; i++) {
            int64_t diff = report[i] - report[i + 1];
            if ((std::abs(diff) < 1) || (std::abs(diff) > 3) || (diff * isIncreasing < 0)) {
                unsafe = true;
                for (int j = 0; j < report.size(); j++) {
                    std::vector<int64_t> reportSingle = report;
                    reportSingle.erase(reportSingle.begin() + j);
                    if (checkSingle(reportSingle)) {
                        unsafe = false;
                        break;
                    }
                }
                break;
            }
        }
        if (!unsafe) safes++;
    }
    return safes;
}

int main() {
    std::vector<std::vector<int64_t>> input = aoc::parseInput("input", parse);
    std::vector<std::vector<int64_t>> test = aoc::parseInput("test", parse);

    // PART 1 

    aoc::checkTest(part_1, 2, test);
    aoc::showResult(part_1, 1, input);

    // PART 2

    aoc::checkTest(part_2, 4, test);
    aoc::showResult(part_2, 2, input);
}