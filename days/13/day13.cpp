#include <utils.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

struct coords {
    int64_t x, y;
};

std::vector<coords> parse(std::vector<std::string> lines) {
    std::vector<coords> output;
    output.reserve(3);
    for (std::string line : lines) {
        coords pos;
        pos.x = std::stoll(line.substr(line.find_first_of("X") + 2, line.find_first_of(",")));
        pos.y = std::stoll(line.substr(line.find_first_of("Y") + 2));
        output.push_back(pos);
    }
    return output;
}

// PART 1

coords getSolution(const std::vector<coords>& a) {
    coords b;
    if ((a[0].x * a[1].y - a[1].x * a[0].y) == 0) {
        return {-1, -1};
    }
    b.x = a[2].x * a[1].y - a[1].x * a[2].y;
    b.y = - a[2].x * a[0].y + a[2].y * a[0].x;
    b.x /= (a[0].x * a[1].y - a[1].x * a[0].y);
    b.y /= (a[0].x * a[1].y - a[1].x * a[0].y);
    return b;
}

int64_t part_1(std::vector<std::vector<coords>> machines) {    
    int64_t result = 0;
    for (auto machine : machines) {
        coords mov = getSolution(machine);
        if (!(mov.x > 100 || mov.y > 100)) {
            if (mov.x * machine[0].x + mov.y * machine[1].x == machine[2].x && mov.x * machine[0].y + mov.y * machine[1].y == machine[2].y) {
                result += 3 * mov.x + mov.y;
            }
        }
    }
    return result;
}

// PART 2

int64_t part_2(std::vector<std::vector<coords>> machines) {
    int64_t result = 0;
    for (auto machine : machines) {
        machine[2].x = machine[2].x + 10000000000000;
        machine[2].y = machine[2].y + 10000000000000;
        coords mov = getSolution(machine);
        if (mov.x * machine[0].x + mov.y * machine[1].x == machine[2].x && mov.x * machine[0].y + mov.y * machine[1].y == machine[2].y) {
            result += 3 * mov.x + mov.y;
        }
    }
    return result;
}

int main() {
    std::vector<std::vector<coords>> input = aoc::parseInput("input", parse, 3);
    std::vector<std::vector<coords>> test = aoc::parseInput("test", parse, 3);

    // PART 1 

    aoc::checkTest(part_1, 480, test);
    aoc::showResult(part_1, 1, input);

    // PART 2

    aoc::showResult(part_2, 2, input);
}