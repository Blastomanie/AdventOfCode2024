#include <utils.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <queue>
#include <algorithm>

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


coords parse(std::string line) {
    return {std::stoll(line.substr(line.find(",") + 1)), std::stoll(line.substr(0, line.find(",")))};
}

// PART 1

struct node {
    coords pos;
    coords dir;
    int64_t dist;
    std::vector<coords> prev;

    bool operator<(const node& a) const {
        return dist < a.dist;
    }

    bool operator>(const node& a) const {
        return dist > a.dist;
    }

    bool operator==(const node& a) const {
        return dist == a.dist;
    }
};

std::vector<coords> directionVector {
    {0, 1},
    {1, 0},  
    {0, -1},
    {-1, 0}
};

std::pair<int64_t, std::vector<coords>> dijkstra(const std::vector<std::string>& grid) {
    std::priority_queue<node, std::vector<node>, std::greater<node>> Q;
    std::vector<int64_t> dist(grid.size() * grid[0].length(), std::numeric_limits<int64_t>::max());

    dist[0] = 0;

    Q.push({{0, 0}, {0, 0}, 0, {}});

    while (!Q.empty()) {
        node min = Q.top();
        Q.pop();

        min.prev.push_back(min.pos);

        if (min.pos == (coords) {(int64_t) grid.size() - 1, (int64_t) grid[0].length() - 1}) {
            return {min.dist, min.prev};
        }

        for (coords d : directionVector) {
            if (min.dir != d * (-1)) {
                coords step = min.pos + d;
                if (step.row >= 0 && step.row < grid.size() && step.col >= 0 && step.col < grid[0].length()) {
                    if (grid[step.row][step.col] != '#') {
                        int64_t temp = min.dist + 1;
                        if (temp < dist[step.row * grid[0].length() + step.col]) {
                            dist[step.row * grid[0].length() + step.col] = temp;
                            Q.push({step, d, temp, min.prev});
                        }
                    }
                }
            }
        }
    }
    return {-1, {}};
}

std::vector<std::string> createGrid(const std::vector<coords>& bytesPosition, int n, int bytes) {
    std::vector<std::string> grid;
    for (int i = 0; i < n; i++) {
        std::string s(n, '.');
        grid.push_back(s);
    }
    for (int i = 0; i < bytes; i++) {
        grid[bytesPosition[i].row][bytesPosition[i].col] = '#';
    }
    return grid;
}

int64_t part_1(std::vector<coords> bytesPosition, int n, int bytes) {  
    std::vector<std::string> grid = createGrid(bytesPosition, n, bytes);
    return dijkstra(grid).first;
}

// PART 2

std::string part_2(std::vector<coords> bytesPosition, int n, int bytes) {    
    std::vector<std::string> grid = createGrid(bytesPosition, n, bytes);
    std::pair<int64_t, std::vector<coords>> path = dijkstra(grid);
    for (int i = bytes + 1; i < bytesPosition.size(); i++) {
        grid[bytesPosition[i].row][bytesPosition[i].col] = '#';
        if (std::find(path.second.begin(), path.second.end(), bytesPosition[i]) != path.second.end()) {
            path = dijkstra(grid);
            if (path.first == -1) {
                return std::to_string(bytesPosition[i].col) + ',' + std::to_string(bytesPosition[i].row);
            }
        }
    }
    return "error";
}

int main() {
    std::vector<coords> test = aoc::parseInput("test", parse);
    std::vector<coords> input = aoc::parseInput("input", parse);

    // PART 1 

    aoc::checkTest(part_1, 22, test, 7, 12);
    aoc::showResult(part_1, 1, input, 71, 1024);

    // PART 2

    aoc::checkTest(part_2, "6,1", test, 7, 12);
    aoc::showResult(part_2, 1, input, 71, 1024);
}