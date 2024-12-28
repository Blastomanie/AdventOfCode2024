#include <utils.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

void parse(std::string line, std::vector<std::pair<std::string, std::string>>& output) {
    output.push_back({line.substr(0, line.find("-")), line.substr(line.find("-") + 1)});
}

// PART 1

int64_t part_1(std::vector<std::pair<std::string, std::string>> connections) { 
    int64_t result = 0;
    std::map<std::string, std::set<std::string>> graph;
    for (std::pair<std::string, std::string> connection : connections) {
        graph[connection.first].insert(connection.second);
        graph[connection.second].insert(connection.first);
    }
    
    std::set<std::set<std::string>> setOfConnectedComputers;

    for (std::map<std::string, std::set<std::string>>::iterator it = graph.begin(); it != graph.end(); it++) {
        for (std::set<std::string>::iterator setit = it->second.begin(); setit != it->second.end(); setit++) {
            for (std::set<std::string>::iterator setit_next = std::next(setit); setit_next != it->second.end(); setit_next++) {
                if (graph[*setit].find(*setit_next) != graph[*setit].end()) {
                    if (setOfConnectedComputers.insert({it->first, *setit, *setit_next}).second) {
                        if (it->first[0] == 't' || (*setit)[0] == 't' || (*setit_next)[0] == 't') result++;
                    }
                }
            }
        }
    }
    return result;
}

// PART 2

void BronKerbosch(const std::map<std::string, std::set<std::string>>& graph, std::set<std::string>& r, std::set<std::string>& p, std::set<std::string>& x, std::set<std::string>& largest) {
    if (p.empty() && x.empty()) {
        if (r.size() > largest.size()) {
            largest = r;
        }
        return;
    }

    std::set<std::string> pnu;
    std::string pivot = p.empty() ? *x.begin() : *p.begin();

    for (std::string v : p) {
        if (std::find(graph.at(pivot).begin(), graph.at(pivot).end(), v) == graph.at(pivot).end()) {
            pnu.insert(v);
        }
    }

    for (std::string v : pnu) {
        std::set<std::string> new_p;
        std::set<std::string> new_x;
        std::set<std::string> new_r = r;
        new_r.insert(v);

        for (std::string NV : graph.at(v)) {
            if (p.find(NV) != p.end()) {
                new_p.insert(NV);
            }
            if (x.find(NV) != x.end()) {
                new_x.insert(NV);
            }
        }
        BronKerbosch(graph, new_r, new_p, new_x, largest);
        p.erase(v);
        x.insert(v);
    }
}

std::string part_2(std::vector<std::pair<std::string, std::string>> connections) {  
    std::set<std::string> r;
    std::set<std::string> p;
    std::set<std::string> x;
    std::set<std::string> largest;

    std::map<std::string, std::set<std::string>> groups;
    for (std::pair<std::string, std::string> connection : connections) {
        groups[connection.first].insert(connection.second);
        groups[connection.second].insert(connection.first);
        p.insert(connection.first);
        p.insert(connection.second);
    }

    BronKerbosch(groups, r, p, x, largest);
    return aoc::join(largest, ',');
}

int main() {
    std::vector<std::pair<std::string, std::string>> test = aoc::parseInput("test", parse);
    std::vector<std::pair<std::string, std::string>> input = aoc::parseInput("input", parse);

    // PART 1 

    aoc::checkTest(part_1, 7, test);
    aoc::showResult(part_1, 1, input);

    // PART 2

    aoc::checkTest(part_2, "co,de,ka,ta", test);
    aoc::showResult(part_2, 1, input);

}