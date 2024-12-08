#include <utils.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

std::string parse(std::string line) {
    return line;
}

struct coords {
    int row, col;

    bool operator==(const coords& a) const {
        return (row == a.row && col == a.col);
    }

    bool operator<(const coords& a) const {
        if (row == a.row) return col < a.col;
        return row < a.row;
    }

    coords operator+(const coords& a) const {
        return {row + a.row, col + a.col};
    }
};

std::map<coords, coords> turnRight {
    {{-1, 0}, {0, 1}},
    {{0, 1}, {1, 0}},
    {{1, 0}, {0, -1}},
    {{0, -1}, {-1, 0}}
};


// PART 1

void getCases(const std::vector<std::string>& grid, std::set<coords>& cases, coords heading, const coords lastPosition) {
    if (!(lastPosition.row == 0 || lastPosition.row == grid.size() - 1 || lastPosition.col == 0 || lastPosition.col == grid[0].length() - 1)) {
        coords step = lastPosition + heading;
        if (grid[step.row][step.col] == '.' || grid[step.row][step.col] == '^') {
            cases.insert(step);
            getCases(grid, cases, heading, step);
        } else if (grid[step.row][step.col] == '#') {
            heading = turnRight[heading];
            getCases(grid, cases, heading, lastPosition);
        }
    }
}

coords getStartCoords(const std::vector<std::string>& grid, const char character) {
    for (int i = 0; i < grid.size(); i++) {
        int j = grid[i].find(character);
        if (j != std::string::npos) {
            return {i, j};
        }
    }
    return {-1, -1};
}

int64_t part_1(std::vector<std::string> grid) {    
    coords startPosition = getStartCoords(grid, '^');
    std::set<coords> cases{startPosition};
    getCases(grid, cases, {-1, 0}, startPosition);
    int64_t result = cases.size();
    return result;
}

// PART 2

bool getLoop(const std::vector<std::string>& grid, std::set<std::pair<coords, coords>>& cases, coords heading, const coords lastPosition) {
    if (lastPosition.row == 0 || lastPosition.row == grid.size() - 1 || lastPosition.col == 0 || lastPosition.col == grid[0].length() - 1) {
        return false;
    }
    coords step = lastPosition + heading;
    if (grid[step.row][step.col] == '.' || grid[step.row][step.col] == '^') {
        if (!cases.insert({step, heading}).second) return true;
        return getLoop(grid, cases, heading, step);
    } else if (grid[step.row][step.col] == '#') {
        heading = turnRight[heading];
        return getLoop(grid, cases, heading, lastPosition);
    }
    return false;
}

bool getLoop(const std::vector<std::string>& grid, coords position) {
    coords heading = {-1, 0};
    std::set<std::pair<coords, coords>> visited{{position, heading}};
    while (true) {
        if (position.row == 0 || position.row == grid.size() - 1 || position.col == 0 || position.col == grid[0].length() - 1) {
            return false;
        }
        coords step = position + heading;
        if (grid[step.row][step.col] == '.' || grid[step.row][step.col] == '^') {
            if (!visited.insert({step, heading}).second) {
                return true;
            }
            position = step;
        } else {
            heading = turnRight[heading];
        }
    }
}

int64_t part_2(std::vector<std::string> grid) {
    int64_t result = 0;
    coords startPosition = getStartCoords(grid, '^');
    std::set<coords> cases{startPosition};
    getCases(grid, cases, {-1, 0}, startPosition);
    int i = 0;
    for (coords casem : cases) {
        if (!(casem == startPosition)) {
            // std::vector<std::string> newGrid = grid;
            grid[casem.row].replace(casem.col, 1, "#");
            std::set<std::pair<coords, coords>> newCase{{startPosition, {-1, 0}}};
            // result += getLoop(newGrid, startPosition);
            result += getLoop(grid, newCase, {-1, 0}, startPosition);
            grid[casem.row].replace(casem.col, 1, ".");
        }
    }
    return result;
}

int main() {
    std::vector<std::string> input = aoc::parseInput("input", parse);
    std::vector<std::string> test = aoc::parseInput("test", parse);

    // PART 1 

    aoc::checkTest(part_1, 41, test);
    aoc::showResult(part_1, 1, input);

    // PART 2

    aoc::checkTest(part_2, 6, test);
    aoc::showResult(part_2, 2, input);
}