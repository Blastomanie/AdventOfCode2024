#include <utils.h>
#include <iostream>
#include <string>
#include <vector>

std::string parse(std::string line) {
    return line;
}

// PART 1

struct coords {
    int64_t row, col;

    bool operator==(const coords& a) const {
        return row == a.row && col == a.col;
    }

    bool operator<(const coords& a) const {
        if (row == a.row) return col < a.col;
        return row < a.row;
    }

    bool operator!=(const coords& a) const {
        return row != a.row || col != a.col;
    }

    coords operator+(const coords& a) const {
        return {row + a.row, col + a.col};
    }

    coords operator-(const coords& a) const {
        return {row - a.row, col - a.col};
    }

    coords operator*(const int a) const {
        return {row * a, col * a};
    }
};


std::vector<coords> directionVector {
    {0, 1},
    {0, -1},
    {1, 0},
    {-1, 0}
};

coords findES(const std::vector<std::string>& map, char ES) {
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[0].length(); j++) {
            if (map[i][j] == ES) {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}

std::map<coords, int64_t> findCorrectTiles(const std::vector<std::string>& map, coords startPosition, coords endPosition) {
    coords currentDir{0, 0};
    std::map<coords, int64_t> tiles{{startPosition, 0}};
    int64_t num_step = 0;
    while (startPosition != endPosition) {
        for (coords dir : directionVector) {
            if (dir != currentDir * (-1)) {
                coords step = startPosition + dir;
                if (step.row >= 0 && step.row < map.size() && step.col >= 0 && step.col < map[0].length()) {
                    if (map[step.row][step.col] != '#') {
                        startPosition = step;
                        currentDir = dir;
                        num_step++;
                        break;
                    }
                }
            }
        }
        tiles[startPosition] = num_step;
    }
    return tiles;
}

int64_t part_1(std::vector<std::string> map) { 
    int64_t result = 0;
    coords E = findES(map, 'E');
    coords S = findES(map, 'S');
    std::map<coords, int64_t> correctTiles = findCorrectTiles(map, S, E);
    for (std::map<coords, int64_t>::iterator tile = correctTiles.begin(); tile != correctTiles.end(); tile++) {
        for (coords dir : directionVector) {
            coords step = tile->first + dir * 2;
            if (step.row >= 0 && step.row < map.size() && step.col >= 0 && step.col < map[0].length()) {
                if (map[step.row][step.col] != '#' && map[step.row - dir.row][step.col - dir.col] == '#') {
                    if (std::abs(tile->second - correctTiles[step]) - 2 >= 100) {
                        result++;
                    }
                }
            }
        }
    }
    return result / 2;
}

// PART 2

int64_t part_2(std::vector<std::string> map) {  
    int64_t result = 0;
    coords E = findES(map, 'E');
    coords S = findES(map, 'S');
    std::map<coords, int64_t> correctTiles = findCorrectTiles(map, S, E);
    for (std::map<coords, int64_t>::iterator tile = correctTiles.begin(); tile != correctTiles.end(); tile++) {
        for (std::map<coords, int64_t>::iterator nextTile = std::next(tile); nextTile != correctTiles.end(); nextTile++) {
            int64_t dist = std::abs(tile->first.row - nextTile->first.row) + std::abs(tile->first.col - nextTile->first.col);
            if (dist < 21 && std::abs(tile->second - nextTile->second) - dist >= 100) {
                result++;
            }
        }
    }
    return result;
}

int main() {
    std::vector<std::string> input = aoc::parseInput("input", parse);

    // PART 1 

    aoc::showResult(part_1, 1, input);

    // PART 2

    aoc::showResult(part_2, 2, input);
}