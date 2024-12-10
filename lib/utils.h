#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <chrono>

#ifndef NDEBUG
#   define assert(condition, message) \
    do { \
        if (! (condition)) { \
            std::cerr << "Assertion `" #condition "` failed " << message << std::endl; \
            std::terminate(); \
        } \
    } while (false)
#else
#   define assert(condition, message) do { } while (false)
#endif


namespace aoc {

    template <typename T>
    std::vector<T> parseInput(std::string inputName, T (*func)(std::string), bool firstPart = true) {
        std::fstream input;
        input.open(inputName + ".txt", std::ios::in);
        std::vector<T> output;
        std::string line;
        if (input.is_open()) {
            while (std::getline(input, line)) {
                if (firstPart) {
                    if (line.empty()) break;
                    output.push_back(func(line));
                } else {
                    if (firstPart) output.push_back(func(line));
                    if (line.empty()) firstPart = !firstPart;
                }
            }
        }
        input.close();
        return output;
    }

    template <typename T1, typename T2>
    std::map<T1, std::vector<T2>> parseInput(std::string inputName, void (*func)(std::string, std::map<T1, std::vector<T2>>&), bool firstPart = true) {
        std::fstream input;
        input.open(inputName + ".txt", std::ios::in);
        std::map<T1, std::vector<T2>> output;
        std::string line;
        if (input.is_open()) {
            while (std::getline(input, line)) {
                if (firstPart) {
                    if (line.empty()) break;
                    if (firstPart) func(line, output);
                } else {
                    if (firstPart) func(line, output);
                    if (line.empty()) firstPart = !firstPart;
                }

            }
        }
        input.close();
        return output;
    }

    template <typename T1, typename T2>
    std::pair<std::vector<T1>, std::vector<T2>> parseInput(std::string inputName, void (*func)(std::string, std::pair<std::vector<T1>, std::vector<T2>>&), bool firstPart = true) {
        std::fstream input;
        input.open(inputName + ".txt", std::ios::in);
        std::pair<std::vector<T1>, std::vector<T2>> output;
        std::string line;
        if (input.is_open()) {
            while (std::getline(input, line)) {
                if (firstPart) {
                    if (line.empty()) break;
                    if (firstPart) func(line, output);
                } else {
                    if (firstPart) func(line, output);
                    if (line.empty()) firstPart = !firstPart;
                }

            }
        }
        input.close();
        return output;
    }

    template <typename T, typename... Args>
    void showResult(T (*part)(Args...), int num, Args... args) {
        auto start = std::chrono::high_resolution_clock::now();
        std::cout << "==============================================================" << std::endl;
        std::cout << "The solution of part "<< num << " is : " << part(args...) << std::endl;
        auto stop = std::chrono::high_resolution_clock::now();
        std::cout << "Time taken: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << " ms" << std::endl;
        std::cout << "==============================================================" << std::endl;
    }

    template <typename T, typename... Args>
    void checkTest(T (*part)(Args...), int64_t value, Args... args) {
        T output = part(args...);
        assert(output == value, "excepted " << value << " but get " << output);
    }

    std::vector<int64_t> splitvi(std::string line, char delimiter) {
        std::vector<int64_t> output;
        std::stringstream ss(line);
        std::string tmp;
        while(std::getline(ss, tmp, delimiter)) {
            output.push_back(std::stoll(tmp));
        }
        return output;
    }

    std::pair<int64_t, int64_t> splitpi(std::string line, char delimiter) {
        std::pair<int64_t, int64_t> output;
        std::stringstream ss(line);
        std::string tmp;
        bool first = true;
        while(std::getline(ss, tmp, delimiter)) {
            if (first) {
                output.first = std::stoll(tmp);
                first = !first;
            } else {
                output.second = std::stoll(tmp);
            }
        }
        return output;
    }

    template <typename T>
    void printv(std::vector<T> vec) {
        std::string out = "vector : ";
        for (T elem : vec) {
            out += std::to_string(elem) + ",";
        }
        out.pop_back();
        std::cout << out << std::endl;
    }

    template <typename T>
    T sumv(std::vector<T> vec) {
        T sum = 0;
        for (int64_t val : vec) {
            sum += val;
        }
        return sum;
    }

    template <typename T>
    void printm(std::vector<std::vector<T>> matrix) {
        std::string out = "matrix : \n";
        for (std::vector<T> row : matrix) {
            for (T elem : row) {
                out += std::to_string(elem) + ",";
            }
            out.pop_back();
            out += "\n";
        }
        std::cout << out;
    }

}

#endif