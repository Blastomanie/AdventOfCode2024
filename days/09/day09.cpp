#include <utils.h>
#include <iostream>
#include <string>
#include <vector>

void parse(std::string line, std::pair<std::vector<int64_t>, std::vector<int64_t>>& output) {
    for (int i = 0; i < line.length(); i++) {
        if (i % 2 == 0) {
            output.first.push_back((int64_t) (line[i] - '0'));
        } else {
            output.second.push_back((int64_t) (line[i] - '0'));
        }
    }
}

// PART 1

int64_t part_1(std::pair<std::vector<int64_t>, std::vector<int64_t>> diskMap) {    
    int64_t result = 0;
    std::vector<int64_t> files = diskMap.first;
    std::vector<int64_t> freeSpace = diskMap.second;

    int64_t position = 0;
    int64_t maxPosition = aoc::sumv(files);
    int64_t id = 0;
    int64_t maxId = files.size() - 1;
    int64_t freeSpaceCursor = 0;

    bool isMoving;
    while (position != maxPosition) {
        isMoving = true;
        for (int64_t l = 0; l < files[id]; l++) {
            result += (position * id);
            position++;
            if (position == maxPosition) break;
        }

        if (position == maxPosition) break;

        id++;
        if (freeSpaceCursor < freeSpace.size() && freeSpace[freeSpaceCursor] == 0) {
            freeSpaceCursor++;
            isMoving = false;
        }
        if (isMoving && freeSpaceCursor < freeSpace.size()) {
            for (int l = 0; l < freeSpace[freeSpaceCursor]; l++) {
                while (files[maxId] == 0) {
                    maxId--;
                }
                result += (position * maxId);
                files[maxId]--;
                position++;
                if (position == maxPosition) break;
            }
            freeSpaceCursor++;
        }
    }
    return result;
}

// PART 2

int64_t part_2(std::pair<std::vector<int64_t>, std::vector<int64_t>> diskMap) {
    int64_t result = 0;
    std::vector<int64_t> files = diskMap.first;
    std::vector<int64_t> freeSpace = diskMap.second;
    std::vector<int64_t> movingBlocks(freeSpace.size(), 0);

    int64_t maxId = files.size() - 1;
    if (freeSpace.size() == files.size()) freeSpace.erase(freeSpace.begin() + freeSpace.size() - 1);
    int64_t maxPosition = aoc::sumv(files) + aoc::sumv(freeSpace) - 1;
    bool isMoving;
    int freeSpaceCursor;
    int position;
    while (maxId > 0) {
        freeSpaceCursor = 0;
        isMoving = false;
        for (; freeSpaceCursor < maxId; freeSpaceCursor++) {
            if (files[maxId] <= freeSpace[freeSpaceCursor]) {
                isMoving = true;
                break;
            }
        }
        if (!isMoving) {
            for (int i = 0; i < files[maxId]; i++) {
                result += maxPosition * maxId;
                maxPosition--;
            }
        } else {
            freeSpace[freeSpaceCursor] -= files[maxId];
            position = files[0] + movingBlocks[0];
            for (int i = 1; i < freeSpaceCursor + 1; i++) {
                position += files[i] + movingBlocks[i] + freeSpace[i - 1];
            }
            movingBlocks[freeSpaceCursor] += files[maxId];
            for (int i = 0; i < files[maxId]; i++) {
                result += position * maxId;
                position++;
                maxPosition--;
            }
        }
        for (int i = 0; i < freeSpace[maxId - 1] + movingBlocks[maxId - 1]; i++) {
            maxPosition--;
        }
        maxId--;
    }
    return result;
}

int main() {
    std::pair<std::vector<int64_t>, std::vector<int64_t>> input = aoc::parseInput("input", parse);
    std::pair<std::vector<int64_t>, std::vector<int64_t>> test = aoc::parseInput("test", parse);

    // PART 1 

    aoc::checkTest(part_1, 1928, test);
    aoc::showResult(part_1, 1, input);

    // PART 2

    aoc::checkTest(part_2, 2858, test);
    aoc::showResult(part_2, 2, input);
}