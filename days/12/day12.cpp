#include <utils.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

std::string parse(std::string line) {
    return line;
}

// PART 1

std::vector<std::pair<int, int>> directions{
    {-1, 0},
    {1, 0},
    {0, -1},
    {0, 1}
};

int64_t calculateFences(const std::vector<std::string>& grid, std::set<std::pair<int, int>>& visited, int row, int col) {
    int64_t result = 0;
    for (std::pair<int, int> dir : directions) {
        if (row + dir.first >= 0 && row + dir.first < grid.size() && col + dir.second >= 0 && col + dir.second < grid[0].length()) {
            if (grid[row + dir.first][col + dir.second] == grid[row][col]) {
                if (visited.insert({row + dir.first, col + dir.second}).second) {
                    result += calculateFences(grid, visited, row + dir.first, col + dir.second);
                }
            } else {
                result++;
            }
        } else {
            result++;
        }
    }
    return result;
}

int64_t part_1(std::vector<std::string> grid) {    
    int64_t result = 0;
    std::set<std::pair<int, int>> visited;
    int visitedNumber = 0;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].length(); j++) {
            if (visited.insert({i, j}).second) {
                result += calculateFences(grid, visited, i, j) * (visited.size() - visitedNumber);
                visitedNumber = visited.size();
            }
        }
    }
    return result;
}

// PART 2

void findFences(const std::vector<std::string>& grid, std::set<std::pair<int, int>>& visited, int row, int col, std::map<int, std::set<std::pair<int, int>>>& horizontalFences, std::map<int, std::set<std::pair<int, int>>>& verticalFences) {
    for (std::pair<int, int> dir : directions) {
        if (row + dir.first >= 0 && row + dir.first < grid.size() && col + dir.second >= 0 && col + dir.second < grid[0].length()) {
            if (grid[row + dir.first][col + dir.second] == grid[row][col]) {
                if (visited.insert({row + dir.first, col + dir.second}).second) {
                    findFences(grid, visited, row + dir.first, col + dir.second, horizontalFences, verticalFences);
                }
            } else {
                if (dir.first == 0) verticalFences[dir.second].insert({col + dir.second, row + dir.first}); else horizontalFences[dir.first].insert({row + dir.first, col + dir.second});
            }
        } else {
            if (dir.first == 0) verticalFences[dir.second].insert({col + dir.second, row + dir.first}); else horizontalFences[dir.first].insert({row + dir.first, col + dir.second});
        }
    }
}

int64_t calculateSides(const std::map<int, std::set<std::pair<int, int>>>& path) {
    int64_t number = 0;
    for (auto orientationFence = path.begin(); orientationFence != path.end(); orientationFence++) {
        number++;
        auto prev = orientationFence->second.begin();
        for (auto it = std::next(orientationFence->second.begin()); it != orientationFence->second.end(); it++) {
            if (it->first == prev->first) {
                if (it->second != prev->second + 1) {
                    number++;
                }
            } else {
                number++;
            }
            prev++;
        }
    }
    return number;
}

int64_t part_2(std::vector<std::string> grid) {
    int64_t result = 0;
    std::set<std::pair<int, int>> visited;
    int visitedNumber = 0;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].length(); j++) {
            if (visited.insert({i, j}).second) {
                std::map<int, std::set<std::pair<int, int>>> horizontalFences;
                std::map<int, std::set<std::pair<int, int>>> verticalFences;
                findFences(grid, visited, i, j, verticalFences, horizontalFences);
                result += (calculateSides(horizontalFences) + calculateSides(horizontalFences)) * (visited.size() - visitedNumber);
                visitedNumber = visited.size();
            }
        }
    }
    return result;
}

int main() {
    std::vector<std::string> input = aoc::parseInput("input", parse);
    std::vector<std::string> test = aoc::parseInput("test", parse);

    // PART 1 

    aoc::checkTest(part_1, 1930, test);
    aoc::showResult(part_1, 1, input);

    // PART 2

    aoc::checkTest(part_2, 1206, test);
    aoc::showResult(part_2, 2, input);
}