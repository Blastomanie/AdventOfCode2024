#include <utils.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <SDL2/SDL.h>
#include <imgui.h>
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/backends/imgui_impl_sdlrenderer2.h"

struct coords {
    int64_t x, y;
};

std::pair<coords, coords> parse(std::string line) {
    coords p;
    coords v;

    p.x = std::stoll(line.substr(2, line.find_first_of(",") - 2));
    p.y = std::stoll(line.substr(line.find_first_of(",") + 1, line.find_first_of(" ")));
    line = line.substr(line.find_first_of("v"));
    v.x = std::stoll(line.substr(2, line.find_first_of(",") - 2));
    v.y = std::stoll(line.substr(line.find_first_of(",") + 1, line.find_first_of(" ")));
    return {p, v};
}

// PART 1

int64_t part_1(std::vector<std::pair<coords, coords>> machines, const int64_t wide, const int64_t tall) {    
    std::vector<int64_t> quadrants{0, 0, 0, 0};
    for (int i = 0; i < machines.size(); i++) {
        machines[i].first.x = (machines[i].first.x + (machines[i].second.x * 100)) % wide;
        machines[i].first.y = (machines[i].first.y + (machines[i].second.y * 100)) % tall;

        if (machines[i].first.x < 0) machines[i].first.x += wide;
        if (machines[i].first.y < 0) machines[i].first.y += tall;

        int64_t midW = wide / 2;
        int64_t midT = tall / 2;

        if (machines[i].first.x < midW && machines[i].first.y < midT) quadrants[0]++;
        else if (machines[i].first.x > midW && machines[i].first.y < midT) quadrants[1]++;
        else if (machines[i].first.x < midW && machines[i].first.y > midT) quadrants[2]++;
        else if (machines[i].first.x > midW && machines[i].first.y > midT) quadrants[3]++;
    }
    return quadrants[0] * quadrants[1] * quadrants[2] * quadrants[3];
}

// PART 2

struct coordsFloat {
    double x, y;
};

std::pair<coordsFloat, coordsFloat> parseVisualization(std::string line) {
    coordsFloat p;
    coordsFloat v;

    p.x = std::stod(line.substr(2, line.find_first_of(",") - 2));
    p.y = std::stod(line.substr(line.find_first_of(",") + 1, line.find_first_of(" ")));
    line = line.substr(line.find_first_of("v"));
    v.x = std::stod(line.substr(2, line.find_first_of(",") - 2));
    v.y = std::stod(line.substr(line.find_first_of(",") + 1, line.find_first_of(" ")));
    return {p, v};
}

std::vector<std::vector<int>> getChristmasTree(const std::vector<std::pair<coordsFloat, coordsFloat>>& machines, double wide = 101, double tall = 103, double step = 7051) {
    std::vector<std::vector<int>> tree{{}, {}, {}};
    for (int i = 0; i < machines.size(); i++) {
        double x = std::fmod(machines[i].first.x + step * machines[i].second.x, wide);
        double y = std::fmod(machines[i].first.y + step * machines[i].second.y, tall);

        if (x < 0.0f) x += wide;
        if (y < 0.0f) y += tall;

        if (x > 34 && x < 67 && y > 29 && y < 65) {
            if (x > 48 && x < 55 && y > 54 && y < 59) {
                tree[0].push_back(i);
            } else if (x > 38 && x < 62 && y > 33 && y < 59) {
                tree[1].push_back(i);
            } else {
                tree[2].push_back(i);
            }
        }
    }
    return tree;
}

void draw(const std::vector<std::pair<coordsFloat, coordsFloat>>& machines, SDL_Renderer* renderer, double wide, double tall, double step) {
    std::vector<std::vector<int>> tree = getChristmasTree(machines);
    for (int i = 0; i < machines.size(); i++) {
        double x = std::fmod(machines[i].first.x + step * machines[i].second.x, wide);
        double y = std::fmod(machines[i].first.y + step * machines[i].second.y, tall);

        if (x < 0.0f) x += wide;
        if (y < 0.0f) y += tall;


        if (std::find(tree[1].begin(), tree[1].end(), i) != tree[1].end()) {
            SDL_SetRenderDrawColor(renderer, 15, 175, 10, 255);
        } else if (std::find(tree[0].begin(), tree[0].end(), i) != tree[0].end()) {
            SDL_SetRenderDrawColor(renderer, 102, 51, 0, 255);
        } else if (std::find(tree[2].begin(), tree[2].end(), i) != tree[2].end()) {
            SDL_SetRenderDrawColor(renderer, 105, 53, 26, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
        SDL_RenderDrawPointF(renderer, x, y);
    }
}

void part_2_visualization(std::vector<std::pair<coordsFloat, coordsFloat>> machines) {
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("AdventOfCode2024 - Day 14 Visualization", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 101*8, 103*8, window_flags);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    SDL_Event event;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);
    bool showMenu = true;

    bool done = false;
    double counter = 0;
    int precision = 0;
    while (!done) {
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            switch(event.type) {
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym) {
                        case SDLK_RIGHT:
                            if (counter < 7051.0) {
                                counter = std::round((counter + std::pow(10, precision)) * std::pow(10, -precision)) / std::pow(10, -precision);
                            }
                            break;
                        case SDLK_LEFT:
                            if (counter > 0.0) {
                                counter = std::round((counter - std::pow(10, precision)) * std::pow(10, -precision)) / std::pow(10, -precision);
                            }
                            break;
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
            ImGui::SliderInt("Precision", &precision, -5, 5);
            ImGui::Text("Time elapsed: %f", counter);
            ImGui::End();
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderSetScale(renderer, 8, 8);
        draw(machines, renderer, 101, 103, counter);
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
    std::vector<std::pair<coords, coords>> input = aoc::parseInput("input", parse);
    std::vector<std::pair<coords, coords>> test = aoc::parseInput("test", parse);

    // PART 1 

    aoc::checkTest(part_1, 12, test, (int64_t) 11, (int64_t) 7);
    aoc::showResult(part_1, 1, input, (int64_t) 101, (int64_t) 103);

    // PART 2

    std::vector<std::pair<coordsFloat, coordsFloat>> inputVisualization = aoc::parseInput("input", parseVisualization);

    if (*argv[1] == '1') part_2_visualization(inputVisualization);
}