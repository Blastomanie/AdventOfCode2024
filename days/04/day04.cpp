#include <utils.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

std::string parse(std::string line) {
    return line;
}

// PART 1

int64_t part_1(std::vector<std::string> words) {
    int64_t counter = 0;
    for (int i = 0; i < words.size(); i++) {
        for (int j  = 0; j < words[i].length(); j++) {
            if (words[i][j] == 'X') {
                if (j < words[i].length() - 3) {
                    if (words[i][j + 1] == 'M' && words[i][j + 2] == 'A' && words[i][j + 3] == 'S') {
                        counter++;
                    }
                    if (i < words.size() - 3) {
                        if (words[i + 1][j + 1] == 'M' && words[i + 2][j + 2] == 'A' && words[i + 3][j + 3] == 'S') {
                            counter++;
                        }
                    }
                    if (i > 2) {
                        if (words[i - 1][j + 1] == 'M' && words[i - 2][j + 2] == 'A' && words[i - 3][j + 3] == 'S') {
                            counter++;
                        }
                    }
                }
                if (i < words.size() - 3) {
                    if (words[i + 1][j] == 'M' && words[i + 2][j] == 'A' && words[i + 3][j] == 'S') {
                        counter++;
                    }
                }
                if (j > 2) {
                    if (words[i][j - 1] == 'M' && words[i][j - 2] == 'A' && words[i][j - 3] == 'S') {
                        counter++;
                    }
                    if (i < words.size() - 3) {
                        if (words[i + 1][j - 1] == 'M' && words[i + 2][j - 2] == 'A' && words[i + 3][j - 3] == 'S') {
                            counter++;
                        }
                    }
                    if (i > 2) {
                        if (words[i - 1][j - 1] == 'M' && words[i - 2][j - 2] == 'A' && words[i - 3][j - 3] == 'S') {
                            counter++;
                        }
                    }
                }
                if (i > 2) {
                    if (words[i - 1][j] == 'M' && words[i - 2][j] == 'A' && words[i - 3][j] == 'S') {
                        counter++;
                    }
                }
            }
        }
    }
    return counter;
}

// PART 2

int64_t part_2(std::vector<std::string> words) {
    int64_t counter = 0;
    for (int i = 1; i < words.size() - 1; i++) {
        for (int j  = 1; j < words[i].length() - 1; j++) {
            if (words[i][j] == 'A') {
                std::string word = std::string(1, words[i - 1][j - 1]) + words[i - 1][j + 1] + words[i + 1][j - 1] + words[i + 1][j + 1];
                if (std::count(word.begin(), word.end(), 'M') == 2 && std::count(word.begin(), word.end(), 'S') == 2) {
                    if (((words[i + 1][j + 1] == 'M') && (words[i - 1][j - 1] != 'M')) || ((words[i + 1][j + 1] == 'S') && (words[i - 1][j - 1] != 'S'))) {
                        counter++;
                    }
                }
            }
        }
    }
    return counter;
}

int main() {
    std::vector<std::string> input = aoc::parseInput("input", parse);
    std::vector<std::string> test = aoc::parseInput("test", parse);

    // PART 1 

    aoc::checkTest(part_1, 18, test);
    aoc::showResult(part_1, 1, input);

    // PART 2

    aoc::checkTest(part_2, 9, test);
    aoc::showResult(part_2, 2, input);
}