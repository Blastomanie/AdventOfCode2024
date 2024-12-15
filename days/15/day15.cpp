#include <utils.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <ostream>
#include <algorithm>
#include <SDL2/SDL.h>
#include <imgui.h>
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/backends/imgui_impl_sdlrenderer2.h"


// Initial approach for part 2: I first considered storing boxes and walls in a vector<coords>
// For some inexplicable reason, I did a complete 180 and ended up with this mess
// (I also threw together an ugly visualization)

std::string parse(std::string line) {
    return line;
}

// PART 1

struct coords {
    int64_t row, col;

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

std::map<char, coords> dir {
    {'<', {0, -1}},
    {'>', {0, 1}},
    {'v', {1, 0}},
    {'^', {-1, 0}}
};

coords findRobot(const std::vector<std::string>& grid) {
    for (int64_t row = 0; row < grid.size(); row++) {
        for (int64_t col = 0; col < grid[row].length(); col++) {
            if (grid[row][col] == '@') {
                return {row, col};
            }
        }
    }
    return {-1, -1};
}

int64_t sumGPSCoordinates(const std::vector<std::string>& grid, const char box) {
    int64_t sum = 0;
    for (int64_t row = 0; row < grid.size(); row++) {
        for (int64_t col = 0; col < grid[row].length(); col++) {
            if (grid[row][col] == box) {
                sum += 100 * row + col;
            }
        }
    }
    return sum;
}

int64_t part_1(std::vector<std::string> grid, std::vector<std::string> instructions) {    
    coords robotPosition = findRobot(grid);
    grid[robotPosition.row][robotPosition.col] = '.';
    for (std::string instructionRow : instructions) {
        for (char instruction : instructionRow) {
            coords step = robotPosition + dir[instruction];
            if (grid[step.row][step.col] == 'O') {
                int k = 1;
                while (grid[step.row + dir[instruction].row * k][step.col + dir[instruction].col * k] == 'O') {
                    k++;
                }
                if (grid[step.row + dir[instruction].row * k][step.col  + dir[instruction].col * k] == '.') {
                    grid[step.row + dir[instruction].row * k][step.col + dir[instruction].col * k] = 'O';
                    grid[step.row][step.col] = '.';
                    robotPosition = step;
                }
            } else if (grid[step.row][step.col] == '.') {
                robotPosition = robotPosition + dir[instruction];
            }
        }
    }
    return sumGPSCoordinates(grid, 'O');
}

// PART 2

void newGrid(std::vector<std::string>& grid) {
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].length(); j += 2) {
            if (grid[i][j] == '#') {
                grid[i].insert(j + 1, "#");
            } else if (grid[i][j] == '.') {
                grid[i].insert(j + 1, ".");
            } else if (grid[i][j] == '@') {
                grid[i].insert(j + 1, ".");
            } else {
                grid[i][j] = '[';
                grid[i].insert(j + 1, "]");
            }
        }
    }
}

std::string find = ".#";

std::map<char, char> swap {
    {'[', ']'},
    {']', '['},
    {'<', '['},
    {'>', ']'}
};

std::map<char, int> partBox {
    {'[', 1},
    {']', -1}
};

bool move(std::vector<std::string>& grid, coords step, const char instruction) {
    if (instruction == '<' || instruction == '>') {
        int k = 1;
        while (find.find(grid[step.row][step.col + dir[instruction].col * k]) == std::string::npos) {
            k++;
        }
        if (grid[step.row][step.col + dir[instruction].col * k] == '.') {
            for (int i = 1; i < k; i++) {
                grid[step.row][step.col + dir[instruction].col * i] = swap[grid[step.row][step.col + dir[instruction].col * i]];
            }
            grid[step.row][step.col + dir[instruction].col * k] = swap[instruction];
            grid[step.row][step.col] = '.';
            return true;
        }

    } else {
        if ((grid[step.row + dir[instruction].row][step.col] == grid[step.row][step.col] && grid[step.row + dir[instruction].row][step.col + partBox[grid[step.row][step.col]]] == grid[step.row][step.col + partBox[grid[step.row][step.col]]]) || (grid[step.row + dir[instruction].row][step.col] == '.' && grid[step.row + dir[instruction].row][step.col + partBox[grid[step.row][step.col]]] == '.')) {
            int k = 1;
            while (find.find(grid[step.row + dir[instruction].row * k][step.col]) == std::string::npos && find.find(grid[step.row + dir[instruction].row * k][step.col + partBox[grid[step.row][step.col]]]) == std::string::npos) {
                if (grid[step.row + dir[instruction].row * k][step.col] != grid[step.row + dir[instruction].row * (k + 1)][step.col]) {
                    break;
                }
                k++;
            }
            if (grid[step.row + dir[instruction].row * k][step.col] == '.' && grid[step.row + dir[instruction].row * k][step.col + partBox[grid[step.row][step.col]]] == '.') {
                char truc = grid[step.row][step.col];
                for (int i = 1; i <= k; i++) {
                    grid[step.row + dir[instruction].row * i][step.col] = truc;
                    grid[step.row + dir[instruction].row * i][step.col + partBox[truc]] = swap[truc];
                }
                grid[step.row][step.col] = '.';
                grid[step.row][step.col + partBox[truc]] = '.';
                return true;
            } else {
                std::vector<std::string> temp = grid;
                char truc = grid[step.row][step.col];
                if (grid[step.row + dir[instruction].row * (k + 1)][step.col] == '#' || grid[step.row + dir[instruction].row * (k + 1)][step.col + partBox[grid[step.row][step.col]]] == '#') {
                    return false;
                }
                if (find.find(grid[step.row + dir[instruction].row * (k + 1)][step.col]) == std::string::npos) {
                    if (!move(temp, {step.row + dir[instruction].row * (k + 1), step.col}, instruction)) return false;
                }
                if (find.find(grid[step.row + dir[instruction].row * (k + 1)][step.col + partBox[grid[step.row][step.col]]]) == std::string::npos) {
                    if (!move(temp, {step.row + dir[instruction].row * (k + 1), step.col + partBox[grid[step.row][step.col]]}, instruction)) return false;
                }
                if (find.find(grid[step.row + dir[instruction].row * (k + 1)][step.col]) == std::string::npos) {
                    move(grid, {step.row + dir[instruction].row * (k + 1), step.col}, instruction);
                }
                if (find.find(grid[step.row + dir[instruction].row * (k + 1)][step.col + partBox[grid[step.row][step.col]]]) == std::string::npos) {
                    move(grid, {step.row + dir[instruction].row * (k + 1), step.col + partBox[grid[step.row][step.col]]}, instruction);
                }
                for (int i = 1; i <= k + 1; i++) {
                    grid[step.row + dir[instruction].row * i][step.col] = truc;
                    grid[step.row + dir[instruction].row * i][step.col + partBox[truc]] = swap[truc];
                }
                grid[step.row][step.col] = '.';
                grid[step.row][step.col + partBox[truc]] = '.';
                return true;
            }
        } else {
            std::vector<std::string> temp = grid;
            if (grid[step.row + dir[instruction].row][step.col] == '#' || grid[step.row + dir[instruction].row][step.col + partBox[grid[step.row][step.col]]] == '#') {
                return false;
            }
            if (find.find(grid[step.row + dir[instruction].row][step.col]) == std::string::npos) {
                if (!move(temp, step + dir[instruction], instruction)) return false;
            }
            if (find.find(grid[step.row + dir[instruction].row][step.col + partBox[grid[step.row][step.col]]]) == std::string::npos) {
                if (!move(temp, {step.row + dir[instruction].row, step.col + partBox[grid[step.row][step.col]]}, instruction)) return false;
            }
            if (find.find(grid[step.row + dir[instruction].row][step.col]) == std::string::npos) {
                move(grid, step + dir[instruction], instruction);
            }
            if (find.find(grid[step.row + dir[instruction].row][step.col + partBox[grid[step.row][step.col]]]) == std::string::npos) {
                move(grid, {step.row + dir[instruction].row, step.col + partBox[grid[step.row][step.col]]}, instruction);
            }
            grid[step.row + dir[instruction].row][step.col] = grid[step.row][step.col];
            grid[step.row + dir[instruction].row][step.col + partBox[grid[step.row][step.col]]] = grid[step.row][step.col + partBox[grid[step.row][step.col]]];
            grid[step.row][step.col + partBox[grid[step.row][step.col]]] = '.';
            grid[step.row][step.col] = '.';
            return true;
        }
    }
    return false;
}

int64_t part_2(std::vector<std::string> grid, std::vector<std::string> instructions) {    
    newGrid(grid);
    coords robotPosition = findRobot(grid);
    grid[robotPosition.row][robotPosition.col] = '.';
    for (std::string instructionRow : instructions) {
        for (char instruction : instructionRow) {
            coords step = robotPosition + dir[instruction];
            if (grid[step.row][step.col] == '[' || grid[step.row][step.col] == ']') {
                if (move(grid, step, instruction)) {
                    robotPosition = step;
                }
            } else if (grid[step.row][step.col] == '.') {
                robotPosition = robotPosition + dir[instruction];
            }
        }
    }
    return sumGPSCoordinates(grid, '[');
}

void move_animation(std::vector<std::string>& grid, coords& robotPosition, const char instruction, SDL_Renderer* renderer, const bool isMoving) {    
    if (isMoving) {
        grid[robotPosition.row][robotPosition.col] = '.';
        coords step = robotPosition + dir[instruction];
        if (grid[step.row][step.col] == '[' || grid[step.row][step.col] == ']') {
            if (move(grid, step, instruction)) {
                robotPosition = step;
            }
        } else if (grid[step.row][step.col] == '.') {
            robotPosition = robotPosition + dir[instruction];
        }
    }

    grid[robotPosition.row][robotPosition.col] = '@';
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].length(); j++) {
            if (grid[i][j] == '[') {
                SDL_SetRenderDrawColor(renderer, 112, 8, 203, 255);
            } else if (grid[i][j] == ']') {
                SDL_SetRenderDrawColor(renderer, 21, 203, 8, 255);
            } else if (grid[i][j] == '#') {
                SDL_SetRenderDrawColor(renderer, 141, 141, 141, 255);
            } else if (grid[i][j] == '@') {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            }
            SDL_RenderDrawPoint(renderer, j, i);
        }
    }
}

void part_2_visualization(std::vector<std::string> grid, std::vector<std::string> instructions) {
    newGrid(grid);
    coords robotPosition = findRobot(grid);
    
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("AdventOfCode2024 - Day 15 - Part 2 Visualization", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, grid[0].length() * 8, grid.size() * 8 + 8 * 10, window_flags);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    SDL_Event event;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);
    bool showMenu = true;

    bool done = false;
    int64_t instruction = 0;
    bool isMoving = false;

    while (!done) {
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            switch(event.type) {
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym) {
                        case SDLK_m:
                            showMenu = !showMenu;
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_QUIT:
                    done = true;
                    break;
            }
        }

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        if (showMenu) {
            ImGui::Begin("Config (Press M to hide/show)");
            if (ImGui::Button("START/STOP")) {
                isMoving = !isMoving;
            }
            ImGui::Text("Move: %c %ld / %ld", instructions[instruction / instructions[0].length()][instruction % instructions[0].length()], instruction + 1, instructions.size() * instructions[0].length());
            ImGui::End();
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderSetScale(renderer, 8, 8);

        if (instruction < instructions.size() * instructions[0].length() - 1)
        instruction += isMoving;
        move_animation(grid, robotPosition, instructions[instruction / instructions[0].length()][instruction % instructions[0].length()], renderer, isMoving);
        SDL_RenderSetScale(renderer, 1, 1);

        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char** argv) {
    std::vector<std::string> grid_test = aoc::parseInput("test", parse);
    std::vector<std::string> instructions_test = aoc::parseInput("test", parse, false);
    std::vector<std::string> grid_input = aoc::parseInput("input", parse);
    std::vector<std::string> instructions_input = aoc::parseInput("input", parse, false);

    // PART 1 

    aoc::checkTest(part_1, 10092, grid_test, instructions_test);
    aoc::showResult(part_1, 1, grid_input, instructions_input);

    // PART 2

    aoc::checkTest(part_2, 9021, grid_test, instructions_test);
    aoc::showResult(part_2, 2, grid_input, instructions_input);

    if (*argv[1] == '1') part_2_visualization(grid_input, instructions_input);
}