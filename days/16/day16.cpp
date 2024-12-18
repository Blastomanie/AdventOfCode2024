#include <utils.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <set>
#include <limits>
#include <queue>
#include <map>

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

struct node {
    coords pos;
    coords dir;
    int64_t dist;
    std::vector<coords> prev;

    bool operator==(const node& a) const {
        return dist == a.dist;
    }

    bool operator<(const node& a) const {
        return dist < a.dist;
    }

    bool operator>(const node& a) const {
        return dist > a.dist;
    }
};


std::vector<coords> directionVector {
    {0, 1},
    {1, 0},  
    {0, -1},
    {-1, 0}
};

std::pair<int64_t, int64_t> dijkstra(const std::vector<std::string>& grid) {
    std::priority_queue<node, std::vector<node>, std::greater<node>> Q;
    std::vector<int64_t> dist((int64_t) (grid.size() * grid[0].length() * 4), std::numeric_limits<int64_t>::max());
    int64_t best = std::numeric_limits<int64_t>::max();
    std::set<coords> tiles;
    
    dist[grid[0].length() * (grid.size() - 2) * 4 + 4] = 0;

    Q.push({{(int64_t) grid.size() - 2, 1}, {0, 1}, 0});

    while (!Q.empty()) {
        node min = Q.top();
        Q.pop();

        min.prev.push_back(min.pos);

        if (min.pos == coords{1, (int64_t) grid[0].length() - 2} && min.dist <= best) {
            for (coords b : min.prev) {
                tiles.insert(b);
            }
            best = min.dist;
        }

        int i = 0;
        for (coords d : directionVector) {
            if (min.dir != d * (-1)) {
                coords step = min.pos + d;
                if (step.row >= 0 && step.row < grid.size() && step.col >= 0 && step.col < grid[0].length()) {
                    if (grid[step.row][step.col] != '#') {
                        int64_t temp = min.dir == d ? min.dist + 1 : min.dist + 1001;
                        if (temp <= dist[step.row * grid[0].length() * 4 + step.col * 4 + i]) {
                            dist[step.row * grid[0].length() * 4 + step.col * 4 + i] = temp;
                            Q.push({step, d, temp, min.prev});
                        }
                    }
                }
            }
            i++;
        }
    }

    return {best, tiles.size()};
}

int64_t part_1(std::vector<std::string> grid) {    
    return dijkstra(grid).first;
}

// PART 2

int64_t part_2(std::vector<std::string> grid) {    
    return dijkstra(grid).second;
}

int main() {
    std::vector<std::string> input = aoc::parseInput("input", parse);
    std::vector<std::string> test = aoc::parseInput("test", parse);

    // PART 1 

    aoc::checkTest(part_1, 11048, test);
    aoc::showResult(part_1, 1, input);

    // PART 2

    aoc::checkTest(part_2, 64, test);
    aoc::showResult(part_2, 2, input);
}