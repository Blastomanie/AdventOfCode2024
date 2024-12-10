#include <utils.h>
#include <iostream>
#include <string>
#include <vector>
#include <set>

std::vector<int64_t> parse(std::string line) {
    std::vector<int64_t> output;
    for (char number : line) {
        output.push_back((int64_t) (number - '0'));
    }
    return output;
}

// PART 1

void setTrailheads(const std::vector<std::vector<int64_t>>& grid, std::set<std::pair<int, int>>& paths, int i, int j) {
    if (grid[i][j] == 9) {
        paths.insert({i, j});
    }
    for (int dx = -1; dx < 2; dx += 2) {
        if (i + dx >= 0 && i + dx < grid.size()) {
            if (grid[i][j] + 1 == grid[i + dx][j]) {
                setTrailheads(grid, paths, i + dx, j);
            }
        }
        if (j + dx >= 0 && j + dx < grid[0].size()) {
            if (grid[i][j] + 1 == grid[i][j + dx]) {
                setTrailheads(grid, paths, i, j + dx);
            } 
        }
    }
}

int64_t part_1(std::vector<std::vector<int64_t>> grid) {    
    int64_t result = 0;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (grid[i][j] == 0) {
                std::set<std::pair<int, int>> paths;
                setTrailheads(grid, paths, i, j);
                result += paths.size();
            }
        }
    }
    return result;
}

// PART 2

int64_t setTrailheads(const std::vector<std::vector<int64_t>>& grid, int i, int j) {
    int64_t distinctTrailheads = 0;
    if (grid[i][j] == 9) {
        return 1;
    }
    for (int dx = -1; dx < 2; dx += 2) {
        if (i + dx >= 0 && i + dx < grid.size()) {
            if (grid[i][j] + 1 == grid[i + dx][j]) {
                distinctTrailheads += setTrailheads(grid, i + dx, j);
            }
        }
        if (j + dx >= 0 && j + dx < grid[0].size()) {
            if (grid[i][j] + 1 == grid[i][j + dx]) {
                distinctTrailheads += setTrailheads(grid, i, j + dx);
            } 
        }
    }
    return distinctTrailheads;
}

int64_t part_2(std::vector<std::vector<int64_t>> grid) {
    int64_t result = 0;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (grid[i][j] == 0) {
                result += setTrailheads(grid, i, j);
            }
        }
    }
    return result;
}

int main() {
    std::vector<std::vector<int64_t>> input = aoc::parseInput("input", parse);
    std::vector<std::vector<int64_t>> test = aoc::parseInput("test", parse);

    // PART 1 

    aoc::checkTest(part_1, 36, test);
    aoc::showResult(part_1, 1, input);

    // PART 2

    aoc::checkTest(part_2, 81, test);
    aoc::showResult(part_2, 2, input);
}