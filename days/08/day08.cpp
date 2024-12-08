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

    coords operator-(const coords& a) const {
        return {row - a.row, col - a.col};
    }
};

// PART 1

std::map<char, std::vector<coords>> getAntenasCoords(const std::vector<std::string>& grid) {
    std::map<char, std::vector<coords>> antennas;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].length(); j++) {
            if (grid[i][j] != '.') {
                antennas[grid[i][j]].push_back({i, j});
            }
        } 
    }
    return antennas;
}

int64_t part_1(std::vector<std::string> grid) {    
    int64_t result = 0;
    std::map<char, std::vector<coords>> antennas = getAntenasCoords(grid);
    std::set<coords> list;
    for (auto const& antenna : antennas) {
        for (int i = 0; i < antenna.second.size() - 1; i++) {
            for (int j = i + 1; j < antenna.second.size(); j++) {
                coords vec = antenna.second[i] - antenna.second[j];
                coords a = antenna.second[i] + vec;
                if (a.row >= 0 && a.row < grid.size() && a.col >= 0 && a.col < grid[0].length()) {
                    list.insert(a);
                }
                coords b = antenna.second[j] - vec;
                if (b.row >= 0 && b.row < grid.size() && b.col >= 0 && b.col < grid[0].length()) {
                    list.insert(b);
                }
            }
        }
    }
    return list.size();
}

// PART 2

int64_t part_2(std::vector<std::string> grid) {
    int64_t result = 0;
    std::map<char, std::vector<coords>> antennas = getAntenasCoords(grid);
    std::set<coords> list;
    for (auto const& antenna : antennas) {
        for (int i = 0; i < antenna.second.size() - 1; i++) {
            for (int j = i + 1; j < antenna.second.size(); j++) {
                coords vec = antenna.second[i] - antenna.second[j];
                coords a = antenna.second[i];
                while (a.row >= 0 && a.row < grid.size() && a.col >= 0 && a.col < grid[0].length()) {
                    list.insert(a);
                    a = a + vec;
                }
                coords b = antenna.second[j];
                while (b.row >= 0 && b.row < grid.size() && b.col >= 0 && b.col < grid[0].length()) {
                    list.insert(b);
                    b = b - vec;
                }
            }
        }
    }
    return list.size();
}

int main() {
    std::vector<std::string> input = aoc::parseInput("input", parse);
    std::vector<std::string> test = aoc::parseInput("test", parse);

    // PART 1 

    aoc::checkTest(part_1, 14, test);
    aoc::showResult(part_1, 1, input);

    // PART 2

    aoc::checkTest(part_2, 34, test);
    aoc::showResult(part_2, 2, input);
}