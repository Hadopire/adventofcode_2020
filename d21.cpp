// so messy lmao atleast it's fast

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

typedef std::unordered_map<std::string, std::list<std::string>> Candidates;

void prune(Candidates& candidates, const std::string& fieldName)
{
    for (auto& pair : candidates) {
        auto& clist = pair.second;
        if (clist.size() == 1)
            continue;

        auto elem = clist.begin();
        while (elem != clist.end()) {
            if (fieldName == *elem) {
                elem = clist.erase(elem);
            } else {
                ++elem;
            }
        }

        if (clist.size() == 1) {
            prune(candidates, clist.front());
        }
    }
}

int main()
{
    std::fstream file("d21.txt");
    std::string line;

    std::unordered_map<std::string, int> ingredients;
    Candidates candidates;
    std::list<std::string> allergens;
    std::unordered_map<std::string, std::string> dangerous;
    size_t first = 0;

    while (std::getline(file, line)) {
        std::list<std::string> curList;
        size_t allergenPos = line.find('(');
        size_t next = line.find(' ');
        size_t pos = 0;
        while (next < allergenPos) {
            std::string ingredient = line.substr(pos, next - pos);
            curList.push_back(ingredient);
            ingredients[ingredient] += 1;
            pos = next + 1;
            next = line.find(' ', pos);
        }

        pos = next + 1;
        next = line.find(',');
        if (next == std::string::npos)
            next = line.find(')', pos);
        while (pos < line.size()) {
            std::string allergen = line.substr(pos, next - pos);

            if (candidates.count(allergen)) {
                auto& clist = candidates[allergen];
                auto it = clist.begin();
                while (it != clist.end()) {
                    if (std::find(curList.begin(), curList.end(), *it) == curList.end()) {
                        it = clist.erase(it);
                    } else {
                        ++it;
                    }
                }
            } else {
                candidates[allergen] = curList;
            }

            pos = next + 2;
            next = line.find(',', pos);
            if (next == std::string::npos)
                next = line.find(')', pos);
        }
    }

    for (auto& pair : candidates) {
        auto& clist = pair.second;
        if (clist.size() == 1) {
            prune(candidates, clist.front());
        }
    }

    for (auto& pair : candidates) {
        auto& clist = pair.second;
        allergens.push_back(clist.front());
        dangerous[clist.front()] = pair.first;
    }

    for (auto& pair : ingredients) {
        auto& name = pair.first;
        if (std::find(allergens.begin(), allergens.end(), name) == allergens.end()) {
            first += pair.second;
        }
    }

    allergens.sort(
        [&dangerous](const std::string& a, const std::string& b) {
            return dangerous[a] <= dangerous[b];
        });

    std::cout << "1: " << first << std::endl;

    for (auto it = allergens.begin(); it != allergens.end(); ++it) {
        if (it != allergens.begin())
            std::cout << ",";
        std::cout << *it;
    }
    std::cout << std::endl;

    return 0;
}