#include <fstream>
#include <iostream>
#include <list>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

typedef std::vector<std::list<std::string>> Candidates;

struct field {
    int min[2];
    int max[2];
};

bool isValid(int n, const field& f)
{
    return ((n >= f.min[0] && n <= f.max[0]) || (n >= f.min[1] && n <= f.max[1]));
}

void prune(Candidates& candidates, const std::string& fieldName)
{
    for (int i = 0; i < candidates.size(); ++i) {
        if (candidates[i].size() == 1)
            continue;

        auto elem = candidates[i].begin();
        while (elem != candidates[i].end()) {
            if (fieldName == *elem) {
                elem = candidates[i].erase(elem);
            } else {
                ++elem;
            }
        }

        if (candidates[i].size() == 1) {
            prune(candidates, candidates[i].front());
        }
    }
}

int main()
{
    std::fstream file("d16.txt");
    std::string line;

    std::unordered_map<std::string, field> fields;
    std::list<std::string> names;
    std::vector<int> myTicket;
    Candidates candidates;
    size_t invalidSum = 0;
    size_t departureProduct = 1;

    std::regex regex("(.+): ([0-9]+)-([0-9]+) or ([0-9]+)-([0-9]+)");
    std::smatch match;
    while (std::getline(file, line)) {
        if (!line[0])
            break;

        std::regex_match(line, match, regex);
        field& f = fields[match[1].str()];
        names.push_back(match[1].str());
        f.min[0] = std::stoi(match[2].str());
        f.max[0] = std::stoi(match[3].str());
        f.min[1] = std::stoi(match[4].str());
        f.max[1] = std::stoi(match[5].str());
    }

    std::getline(file, line);
    std::getline(file, line);
    for (auto it = line.begin(); it < line.end(); ++it) {
        // my ticket
        size_t idx;
        int n = std::stoi(std::string(it, line.end()), &idx);
        it += idx;

        myTicket.push_back(n);
        candidates.push_back(names);
    }

    std::getline(file, line);
    std::getline(file, line);
    while (std::getline(file, line)) {
        // nearby tickets
        size_t fieldn = 0;
        for (auto it = line.begin(); it < line.end(); ++it, ++fieldn) {
            size_t idx;
            int n = std::stoi(std::string(it, line.end()), &idx);
            it += idx;

            bool valid = false;
            for (const auto& pair : fields) {
                const field& f = pair.second;
                if (isValid(n, f)) {
                    valid = true;
                    break;
                }
            }

            if (valid) {
                // if the ticket is valid, we try to deduce which field correspond to which position
                std::list<std::string>& fieldCandidates = candidates[fieldn];
                if (fieldCandidates.size() == 1) {
                    continue;
                }

                auto elem = fieldCandidates.begin();
                while (elem != fieldCandidates.end()) {
                    const field& f = fields[*elem];
                    if (!isValid(n, f)) {
                        elem = fieldCandidates.erase(elem);
                    } else {
                        ++elem;
                    }
                }
            } else {
                invalidSum += n;
            }
        }
    }

    // remove successfully deduced fields from the other candidates' list
    for (int i = 0; i < candidates.size(); ++i) {
        if (candidates[i].size() == 1) {
            prune(candidates, candidates[i].front());
        }
    }

    for (int i = 0; i < myTicket.size(); ++i) {
        std::string& fieldName = candidates[i].front();
        if (fieldName.rfind("departure", 0) == 0) {
            departureProduct *= myTicket[i];
        }
    }

    std::cout << "1: " << invalidSum << std::endl;
    std::cout << "2: " << departureProduct << std::endl;

    return 0;
}