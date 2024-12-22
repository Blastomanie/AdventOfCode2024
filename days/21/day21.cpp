#include <utils.h>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

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
    int64_t num;
    std::string prev;
    int64_t num_dir;

    bool operator==(const node& a) const {
        return num == a.num;
    }

    bool operator<(const node& a) const {
        return num < a.num;
    }

    bool operator>(const node& a) const {
        return num > a.num;
    }
};

std::vector<coords> directionVector {
    {0, 1},
    {0, -1},
    {-1, 0},
    {1, 0}
};

char trad(coords a) {
    if (a == (coords) {0, 1}) return '>';
    if (a == (coords) {0, -1}) return '<';
    if (a == (coords) {1, 0}) return '^';
    return 'v';
}

coords numKeypad(char a) {
    switch(a) {
        case 'A': return {0, 2};
        case '0': return {0, 1};
        case '1': return {1, 0};
        case '2': return {1, 1};
        case '3': return {1, 2};
        case '4': return {2, 0};
        case '5': return {2, 1};
        case '6': return {2, 2};
        case '7': return {3, 0};
        case '8': return {3, 1};
        case '9': return {3, 2};
        default: return {0, 0};
    }
}

coords dirKeypad(char a) {
    switch(a) {
        case '<': return {0, 0};
        case '>': return {0, 2};
        case 'v': return {0, 1};
        case '^': return {1, 1};
        case 'A': return {1, 2};
        default: return {1, 0};
    }
}

std::vector<std::string> getShortestPath(coords start, char destination, coords (*keypad)(char), int64_t maxRow, coords gap) {
    std::priority_queue<node, std::vector<node>, std::greater<node>> Q;
    std::vector<node> paths;

    coords stop = keypad(destination);
    int64_t mini = std::abs(stop.row - start.row) + std::abs(stop.col - start.col);
    Q.push({start, {0, 0}, 0, {}, 0});

    while (!Q.empty()) {
        node min = Q.top();
        Q.pop();

        if (min.dir != (coords) {0, 0}) min.prev.push_back(trad(min.dir));

        if (min.pos == stop && min.num == mini) {
            min.prev.push_back('A');
            paths.push_back(min);
            continue;
        }

        for (coords d : directionVector) {
            if (min.dir != d * (-1)) {
                coords step = min.pos + d;
                if (step.row >= 0 && step.row < maxRow && step.col >= 0 && step.col < 3 && step != gap) {
                    if (min.num + 1 <= mini) {
                        Q.push({step, d, min.num + 1, min.prev, min.dir != d ? min.num_dir + 1 : min.num_dir});
                    }
                }
            }
        }
    }

    int64_t min = std::numeric_limits<int64_t>::max();
    for (node path : paths) {
        if (min > path.num_dir) min = path.num_dir;
    }
    std::vector<std::string> smallestPaths;
    for (node path : paths) {
        if (min == path.num_dir)  {
            smallestPaths.push_back(path.prev);
        }
    }
    return smallestPaths;
}

int64_t getLengthPath(coords start, std::string code, int n, std::map<std::pair<std::pair<coords, int64_t>, std::string>, int64_t>& cache, std::map<int64_t, coords>& startPosition) {
    if (n == 0) return code.length();
    std::vector<std::string> paths = getShortestPath(startPosition[n], code[0], dirKeypad, 2, {1, 0});  
    std::vector<int64_t> lengthPaths;
    int64_t minLength = 0;
    for (std::string path : paths) {
        if (cache.insert({{{startPosition[n], n }, path}, 0}).second) {
            cache[{{startPosition[n], n}, path}] = getLengthPath(start, path, n - 1, cache, startPosition);
        }
        lengthPaths.push_back(cache[{{startPosition[n], n}, path}]);
    }
    startPosition[n] = dirKeypad(code[0]);

    if (code.length() > 1) {
        minLength += (*std::min_element(lengthPaths.begin(), lengthPaths.end()) + getLengthPath(startPosition[n], code.substr(1), n, cache, startPosition));
    } else {
        minLength += *std::min_element(lengthPaths.begin(), lengthPaths.end());
    }
    return minLength;
}

std::vector<std::string> getNumKeypadPaths(coords start, std::string code, std::vector<std::string> patterns) {
    if (code.length() <= 0) return patterns;
    std::vector<std::string> test = getShortestPath(start, code[0], numKeypad, 4, {0, 0});
    std::vector<std::string> a;
    for (int i = 0; i < patterns.size(); i++) {
        for (auto mot : test) {
            a.push_back(patterns[i] + mot);
        }
    }
    return getNumKeypadPaths(numKeypad(code[0]), code.substr(1), a);
}

int64_t part_1(std::vector<std::string> codes) { 
    int64_t result = 0;
    for (int i = 0; i < codes.size(); i++) {
        std::map<std::pair<std::pair<coords, int64_t>, std::string>, int64_t> cache;
        std::map<int64_t, coords> robotsPosition;
        for (int j = 1; j <= 2; j++) {
            robotsPosition[j] = {1, 2};
        }
        std::vector<std::string> numKeypadPaths = getNumKeypadPaths(numKeypad(codes[i][0]), codes[i].substr(1), getShortestPath({0, 2}, codes[i][0], numKeypad, 4, {0, 0}));
        int64_t min = std::numeric_limits<int64_t>::max();
        for (std::string numKeypadPath : numKeypadPaths) {
            int64_t a = getLengthPath({1, 2}, numKeypadPath, 2, cache, robotsPosition);
            if (a < min) min = a; 
        }
        result += min * std::stoll(codes[i].substr(0, 3));
    }
    return result;
}

// PART 2

int64_t part_2(std::vector<std::string> codes) {  
    int64_t result = 0;
    for (int i = 0; i < codes.size(); i++) {
        std::map<std::pair<std::pair<coords, int64_t>, std::string>, int64_t> cache;
        std::map<int64_t, coords> robotsPosition;
        for (int j = 1; j <= 25; j++) {
            robotsPosition[j] = {1, 2};
        }
        std::vector<std::string> numKeypadPaths = getNumKeypadPaths(numKeypad(codes[i][0]), codes[i].substr(1), getShortestPath({0, 2}, codes[i][0], numKeypad, 4, {0, 0}));
        int64_t min = std::numeric_limits<int64_t>::max();
        for (std::string numKeypadPath : numKeypadPaths) {
            int64_t a = getLengthPath({1, 2}, numKeypadPath, 25, cache, robotsPosition);
            if (a < min) min = a; 
        }
        result += min * std::stoll(codes[i].substr(0, 3));
    }
    return result;
}

int main() {
    std::vector<std::string> input = aoc::parseInput("input", parse);
    std::vector<std::string> test = aoc::parseInput("test", parse);

    // PART 1 

    aoc::checkTest(part_1, 126384, test);
    aoc::showResult(part_1, 1, input);

    // PART 2

    aoc::showResult(part_2, 2, input);
}