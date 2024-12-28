#include <utils.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

// PART 2 can be optimized with unordered set & map

void parse(std::string line, std::vector<int64_t>& output) {
    output.push_back(std::stoll(line));
}

// PART 1

int64_t operation(int64_t number) {
    number = (number ^ (number * 64)) % 16777216;
    number = (number ^ (number / 32)) % 16777216;
    number = (number ^ (number * 2048)) % 16777216;
    return number;
}

int64_t part_1(std::vector<int64_t> numbers) { 
    int64_t result = 0;
    for (int64_t number : numbers) {
        for (int i = 0; i < 2000; i++) {
            number = operation(number);
        }
        result += number;
    }
    return result;
}

// PART 2

void move(std::vector<int64_t>& vec, int64_t val) {
    int i = 0;
    for (; i < vec.size() - 1; i++) {
        vec[i] = vec[i + 1];
    }
    vec[i] = val;
}

int64_t part_2(std::vector<int64_t> numbers) {  
    int64_t result = 0;
    std::map<std::vector<int64_t>, int64_t> prices;
    for (int k = 0; k < numbers.size(); k++) {
        int64_t number = numbers[k];
        int64_t digit = number % 10;
        std::vector<int64_t> sequence;
        std::set<std::vector<int64_t>> sequences;
        for (int i = 0; i < 2000; i++) {
            number = operation(number);
            int64_t delta = number % 10 - digit;
            digit = number % 10;

            if (sequence.size() < 4) {
                sequence.push_back(delta);
            } else {
                move(sequence, delta);
                if (sequences.insert(sequence).second) {
                    prices[sequence] += digit;
                }
            }
        }
    }

    for (std::map<std::vector<int64_t>, int64_t>::iterator it = prices.begin(); it != prices.end(); it++) {
        if (it->second > result) result = it->second;
    }
    return result;
}

int main() {
    std::vector<int64_t> test = aoc::parseInput("test", parse);
    std::vector<int64_t> input = aoc::parseInput("input", parse);

    // PART 1 

    aoc::checkTest(part_1, 37327623, test);
    aoc::showResult(part_1, 1, input);

    // PART 2

    std::vector<int64_t> test2 = aoc::parseInput("test2", parse);

    aoc::checkTest(part_2, 23, test2);
    aoc::showResult(part_2, 1, input);

}